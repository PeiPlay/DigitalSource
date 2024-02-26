#include "uart_bsp.h"
#include "math_bsp.h"

void UartBsp_Init(UartBsp_t* uartBsp)
{
    uartBsp->state = UARTBSP_STATE_HEAD;
    HAL_UART_Receive_IT(uartBsp->huart, (uint8_t*)&uartBsp->frame.head, sizeof(UartBspFrame_Head_t));
}

void UartBsp_ReceiveIT_Callback(UART_HandleTypeDef *huart, UartBsp_t* uartBsp)
{
#define __ProcessAssert_CallError(errorCode) {if(uartBsp->errorProcess != NULL) { uartBsp->errorProcess(errorCode); }}

    if(uartBsp->huart->Instance != huart->Instance)
        return;

    switch (uartBsp->state)
    {
    case UARTBSP_STATE_HEAD://接收到包头
        if(uartBsp->frameHead != uartBsp->frame.head)
            __ProcessAssert_CallError(UARTBSP_ERROR_HEADUNMACH)
        else
        {
            uartBsp->state = UARTBSP_STATE_LEN;
            HAL_UART_Receive_IT(uartBsp->huart, (uint8_t*)&uartBsp->frame.len, sizeof(UartBspFrame_Len_t));
        }
        break;
    case UARTBSP_STATE_LEN://接收到长度
        if(uartBsp->frame.len > sizeof(UartBspFrame_Data_t))
            __ProcessAssert_CallError(UARTBSP_ERROR_LENOUTRANGE)
        else
        {
            uartBsp->state = UARTBSP_STATE_DATA;
            HAL_UART_Receive_IT(uartBsp->huart, (uint8_t*)&uartBsp->frame.data, uartBsp->frame.len);
        }
        break;
    case UARTBSP_STATE_DATA://接收到数据
        uartBsp->state = UARTBSP_STATE_CRC;
        HAL_UART_Receive_IT(uartBsp->huart, (uint8_t*)&uartBsp->frame.crc, sizeof(UartBspFrame_CRC_t));
        break;
    case UARTBSP_STATE_CRC://接收到CRC校验码
        if(uartBsp->frame.crc != MathBsp_Crc16(uartBsp->frame.data, uartBsp->frame.len))
            __ProcessAssert_CallError(UARTBSP_ERROR_CRCFAIL)
        else
        {
            uartBsp->state = UARTBSP_STATE_TAIL;
            HAL_UART_Receive_IT(uartBsp->huart, (uint8_t*)&uartBsp->frame.tail, sizeof(UartBspFrame_Tail_t));
        }
        break;
    case UARTBSP_STATE_TAIL://接收到包尾
        if(uartBsp->frameTail != uartBsp->frame.tail)
            __ProcessAssert_CallError(UARTBSP_ERROR_TAILUNMACH)
        else
        {
            if(uartBsp->dataProcess != NULL)
                uartBsp->dataProcess(uartBsp->frame.data, uartBsp->frame.len);
            uartBsp->state = UARTBSP_STATE_HEAD;
            HAL_UART_Receive_IT(uartBsp->huart, (uint8_t*)&uartBsp->frame.head, sizeof(UartBspFrame_Head_t));
        }
        break;
    }
#undef __ProcessAssert_CallError
}

void UartBsp_Send(UartBsp_t* uartBsp, uint8_t* data, uint16_t len)
{
    //发送帧头
    HAL_UART_Transmit(uartBsp->huart, (uint8_t*)&uartBsp->frameHead, sizeof(UartBspFrame_Head_t), 0xFF);
    //发送数据长度
    HAL_UART_Transmit(uartBsp->huart, (uint8_t*)&len, sizeof(UartBspFrame_Len_t), 0xFF);
    //发送数据
    HAL_UART_Transmit(uartBsp->huart, data, len, 0xFF);
    //发送CRC校验码
    uint16_t crc = MathBsp_Crc16(data, len);
    HAL_UART_Transmit(uartBsp->huart, (uint8_t*)&crc, sizeof(UartBspFrame_CRC_t), 0xFF);
    //发送帧尾
    HAL_UART_Transmit(uartBsp->huart, (uint8_t*)&uartBsp->frameTail, sizeof(UartBspFrame_Tail_t), 0xFF);
}
