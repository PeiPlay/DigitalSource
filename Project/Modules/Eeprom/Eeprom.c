#include "Eeprom.h"
#include "cmsis_os.h"
#include "DwtClock.h"

#define _EEPROM_WRITE_ADDR(x) (x)
#define _EEPROM_READ_ADDR(x) ((x) | 1)

void _Eeprom_DelayMs(uint32_t ms)
{
#if defined(osCMSIS)
    osDelay(ms);
#elif defined(DWT_CLOCK)
    DwtClock_DelayMs(ms);
#else
    HAL_Delay(ms);
#endif
}

uint8_t _Eeprom_RequestValid(Eeprom_t* eeprom, uint16_t address, uint32_t size)
{
    if(eeprom == NULL)                      return 0;
    if(eeprom->i2c_handle == NULL)          return 0;
    if(eeprom->i2c_address == 0)            return 0;
    if(eeprom->total_size == 0)             return 0;
    if(eeprom->page_size == 0)              return 0;
    if(address + size > eeprom->total_size) return 0;
    return 1;
}

uint8_t _Eeprom_IsFifoFull(Eeprom_t* eeprom)
{
    return (eeprom->fifo.head + 1) % EEPROM_FIFO_QUEUE_SIZE == eeprom->fifo.tail;
}
uint8_t _Eeprom_IsFifoEmpty(Eeprom_t* eeprom)
{
    return eeprom->fifo.head == eeprom->fifo.tail;
}
void _Eeprom_FifoPush(Eeprom_t* eeprom, struct EepromRequest_t request)
{
    eeprom->fifo.queue[eeprom->fifo.head] = request;
    eeprom->fifo.head = (eeprom->fifo.head + 1) % EEPROM_FIFO_QUEUE_SIZE;
}
struct EepromRequest_t _Eeprom_FifoPop(Eeprom_t* eeprom)
{
    struct EepromRequest_t request = eeprom->fifo.queue[eeprom->fifo.tail];
    eeprom->fifo.tail = (eeprom->fifo.tail + 1) % EEPROM_FIFO_QUEUE_SIZE;
    return request;
}

uint8_t _Eeprom_CheckConnect(Eeprom_t* eeprom)
{
    return HAL_I2C_IsDeviceReady(eeprom->i2c_handle, _EEPROM_READ_ADDR(eeprom->i2c_address), 1, 10) == HAL_OK;
}
// 初始化
uint8_t Eeprom_Init(Eeprom_t* eeprom)
{
    if (eeprom == NULL)                     return 0;
    if (eeprom->i2c_handle == NULL)         return 0;
    if (eeprom->i2c_address == 0)           return 0;
    if (eeprom->total_size == 0)            return 0;
    if (eeprom->page_size == 0)             return 0;
    return _Eeprom_CheckConnect(eeprom);
}
void _Eeprom_AddRequest(EepromRequestState_t* request_state, Eeprom_t* eeprom, uint16_t address, uint8_t* data, uint32_t size)
{
    //检查请求是否有效
    if(!_Eeprom_RequestValid(eeprom, address, size))
    {
        request_state->exception = EXCEPTION_INVALID_REQUEST;
        return;
    }
    //等待互斥锁解锁或触发超时
    double createTime = DwtClock_ConvertCnt2Time(request_state->requestId.tick);
    while(eeprom->fifo.mutex != FIFO_UNLOCKED)
    {
        if(DwtClock_GetDwtTime() - createTime > EEPROM_FIFO_REQUEST_OVERTIME_MS * 1000.0)
        {
            request_state->exception = EXCEPTION_OVERTIME;
            return;
        }
    }
    //检查队列是否已满
    if(_Eeprom_IsFifoFull(eeprom))
    {
        request_state->exception = EXCEPTION_QUEUE_FULL;
        return;
    }
    //添加请求
    eeprom->fifo.mutex = FIFO_LOCKED;
    _Eeprom_FifoPush(eeprom, (struct EepromRequest_t) {
        .address = address, 
        .data = data, 
        .size = size, 
        .requestId = request_state->requestId
    });
    eeprom->fifo.mutex = FIFO_UNLOCKED;
}
// 添加读请求
EepromRequestState_t Eeprom_AddRequest_Read(Eeprom_t* eeprom, uint16_t address, uint8_t* data, uint32_t size)
{
    EepromRequestState_t request_state = {
        .eeprom = eeprom, 
        .exception = EXCEPTION_NONE, 
        .requestId = {.tick = DwtClock_GetDwtCnt(), .type = EEPROM_REQUEST_READ}
    };
    _Eeprom_AddRequest(&request_state, eeprom, address, data, size);
    return request_state;
}
// 添加写请求
EepromRequestState_t Eeprom_AddRequest_Write(Eeprom_t* eeprom, uint16_t address, uint8_t* data, uint32_t size)
{
    EepromRequestState_t request_state = {
        .eeprom = eeprom, 
        .exception = EXCEPTION_NONE, 
        .requestId = {.tick = DwtClock_GetDwtCnt(), .type = EEPROM_REQUEST_WRITE}
    };
    _Eeprom_AddRequest(&request_state, eeprom, address, data, size);
    return request_state;
}
// 线程安全Fifo队列处理函数，需要在循环线程中调用
void Eeprom_FifoProcess(Eeprom_t* eeprom)
{
#define __ProcessHalAssert_Read(x) if((x) != HAL_OK)                        \
    {                                                                       \
        if(eeprom->flags.resultReadFailed == RESULT_READ_FAILED_ENABLED){   \
            Eeprom_RequestResultHandler(result);                            \
        }                                                                   \
        return;                                                             \
    }
#define __ProcessHalAssert_Write(x) if((x) != HAL_OK)                       \
    {                                                                       \
        if(eeprom->flags.resultWriteFailed == RESULT_WRITE_FAILED_ENABLED){ \
            Eeprom_RequestResultHandler(result);                            \
        }                                                                   \
        return;                                                             \
    }
    if(eeprom == NULL || _Eeprom_IsFifoEmpty(eeprom)) return;
    EepromRequestResult_t result = { .eeprom = eeprom, .status = REQUEST_FAILED, .request = _Eeprom_FifoPop(eeprom) };
    uint16_t data_in_rom_address = result.request.address;
    uint32_t size = result.request.size;
    uint8_t* data = result.request.data;
    uint16_t byte_per_page = eeprom->page_size;

    if(result.request.requestId.type == EEPROM_REQUEST_WRITE)
    {
        //检查是否跨页
        if((uint32_t)(data_in_rom_address + size) / byte_per_page == (uint32_t)data_in_rom_address / byte_per_page)
        {
            __ProcessHalAssert_Write(
                HAL_I2C_Mem_Write(
                    eeprom->i2c_handle, 
                    _EEPROM_WRITE_ADDR(eeprom->i2c_address), 
                    data_in_rom_address, 
                    I2C_MEMADD_SIZE_16BIT, data, size, 0xff)
            );
            _Eeprom_DelayMs(3);
            return;
        }
        uint16_t address_head   = byte_per_page - (data_in_rom_address % byte_per_page);
        uint16_t address_tail   = (data_in_rom_address + size) % byte_per_page;
        uint32_t page_cnt       = (size > address_head) ? (size - address_head) / byte_per_page : 0;
        uint16_t address_offest = 0;
        //写入头
        if(address_head != 0)
        {
            __ProcessHalAssert_Write(
                HAL_I2C_Mem_Write(
                    eeprom->i2c_handle, 
                    _EEPROM_WRITE_ADDR(eeprom->i2c_address), 
                    data_in_rom_address, 
                    I2C_MEMADD_SIZE_16BIT, 
                    data + address_offest, address_head, 0xff)
            );
            _Eeprom_DelayMs(3);
            address_offest += address_head;
        }
        //写入中间
        for(uint32_t i = 0; i < page_cnt; i++)
        {
            __ProcessHalAssert_Write(
                HAL_I2C_Mem_Write(
                    eeprom->i2c_handle, 
                    _EEPROM_WRITE_ADDR(eeprom->i2c_address), 
                    data_in_rom_address + address_offest, 
                    I2C_MEMADD_SIZE_16BIT, 
                    data + address_offest, byte_per_page, 0xff)
            );
            _Eeprom_DelayMs(3);
            address_offest += byte_per_page;
        }
        //写入尾
        if(address_tail != 0)
        {
            __ProcessHalAssert_Write(
                HAL_I2C_Mem_Write(
                    eeprom->i2c_handle, 
                    _EEPROM_WRITE_ADDR(eeprom->i2c_address), 
                    data_in_rom_address + address_offest, 
                    I2C_MEMADD_SIZE_16BIT, 
                    data + address_offest, address_tail, 0xff)
            );
            _Eeprom_DelayMs(3);
        }
        goto Eeprom_FifoProcess_WriteFinished;
    }
    else 
    {
        __ProcessHalAssert_Read(
            HAL_I2C_Mem_Read(
                eeprom->i2c_handle, 
                _EEPROM_READ_ADDR(eeprom->i2c_address), 
                data_in_rom_address, 
                I2C_MEMADD_SIZE_16BIT, 
                data, size, 0xff)
        );
        _Eeprom_DelayMs(3);
        goto Eeprom_FifoProcess_ReadFinished;
    }


Eeprom_FifoProcess_ReadFinished:
    if(eeprom->flags.resultReadFinished == RESULT_READ_FINISHED_ENABLED)
        Eeprom_RequestResultHandler(result);
    return;

Eeprom_FifoProcess_WriteFinished:
    if(eeprom->flags.resultWriteFinished == RESULT_WRITE_FINISHED_ENABLED)
        Eeprom_RequestResultHandler(result);
    return;

#undef __ProcessHalAssert_Read
#undef __ProcessHalAssert_Write
}

// 弱定义的读写请求结果处理函数，允许重写
__weak void Eeprom_RequestResultHandler(EepromRequestResult_t result)
{
    UNUSED(result);
}
