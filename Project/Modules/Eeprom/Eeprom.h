
/*
* @brief 线程安全的Eeprom驱动，使用FIFO队列处理读写请求
* @note  1. 读写请求的处理函数需要在某个循环线程中调用，若使用循环的中断触发，
*           则NVIC中的Time base: System tick timer的优先级必须高于该中断的优先级
*/

#pragma once
#include "main.h"
#include "DwtClock.h"
#define EEPROM_FIFO_QUEUE_SIZE  20
#define EEPROM_FIFO_REQUEST_OVERTIME_MS 1

// 读写请求类型
typedef enum { EEPROM_REQUEST_READ, EEPROM_REQUEST_WRITE } EepromRequestType_t;

// 读写请求标识符，其对应请求时的系统时钟滴答数
typedef struct 
{
    EepromRequestType_t type;
    uint64_t tick;
} EepromRequestId_t;

//读写请求处理队列
typedef struct {
    struct EepromRequest_t
    {
        EepromRequestId_t requestId;                // 请求标识符
        uint16_t address;                           // Eeprom地址
        uint8_t* data;                              // 数据指针，指向要读写的数据
        uint32_t size;                              // 数据大小
    } queue[EEPROM_FIFO_QUEUE_SIZE];
    // 读写请求队列的头尾指针
    uint16_t head;
    uint16_t tail;
    // 入队互斥锁
    enum { FIFO_LOCKED, FIFO_UNLOCKED } mutex;
} EepromRequestFifo_t;

// Eeprom标志位
typedef struct 
{
    // 是否在取请求完成成功时触发请求结果处理函数
    enum { RESULT_READ_FINISHED_ENABLED, RESULT_READ_FINISHED_DISABLED } resultReadFinished;
    // 是否在写入请求完成成功时触发请求结果处理函数
    enum { RESULT_WRITE_FINISHED_ENABLED, RESULT_WRITE_FINISHED_DISABLED } resultWriteFinished;
    // 是否在取请求完成失败时触发请求结果处理函数
    enum { RESULT_READ_FAILED_ENABLED, RESULT_READ_FAILED_DISABLED } resultReadFailed;
    // 是否在写入请求完成失败时触发请求结果处理函数
    enum { RESULT_WRITE_FAILED_ENABLED, RESULT_WRITE_FAILED_DISABLED } resultWriteFailed;
} EepromFlags_t;


typedef struct {

    I2C_HandleTypeDef* i2c_handle;  // Eeprom设备对应的I2C句柄
    uint16_t i2c_address;           // Eeprom设备的I2C地址
    uint16_t page_size;             // 每页大小 (字节)
    uint16_t total_size;            // 总容量   (字节)
    EepromFlags_t flags;            // Eeprom标志位
    EepromRequestFifo_t fifo;       // 读写请求队列
} Eeprom_t;
//请求处理异常
typedef struct
{
    Eeprom_t* eeprom;                   //Eeprom设备
    enum {
        EXCEPTION_NONE,                 //请求添加成功    
        EXCEPTION_QUEUE_FULL,           //队列已满
        EXCEPTION_INVALID_REQUEST,      //无效请求
        EXCEPTION_OVERTIME              //超时
    } exception;
    EepromRequestId_t requestId;        //请求标识符
} EepromRequestState_t;                 //请求处理状态
//请求处理结果
typedef struct
{
    Eeprom_t* eeprom;
    enum { REQUEST_FAILED, REQUEST_FINISHED } status;
    struct EepromRequest_t request;
} EepromRequestResult_t;                //请求处理结果


// 初始化，成功返回1，失败返回0
uint8_t Eeprom_Init(Eeprom_t* eeprom);
// 添加读请求，并返回请求受理状态
EepromRequestState_t Eeprom_AddRequest_Read(Eeprom_t* eeprom, uint16_t address, uint8_t* data, uint32_t size);
// 添加写请求，并返回请求受理状态
EepromRequestState_t Eeprom_AddRequest_Write(Eeprom_t* eeprom, uint16_t address, uint8_t* data, uint32_t size);
// Fifo队列处理函数，需要在某个循环线程中调用
void Eeprom_FifoProcess(Eeprom_t* eeprom);
// 弱定义的读写请求结果处理函数，允许重写
__weak void Eeprom_RequestResultHandler(EepromRequestResult_t result);

