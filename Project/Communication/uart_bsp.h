/*
* @brief USART数据包处理
* @note  使用CRC16校验
*/
#pragma once
#include "main.h"
#include "usart.h"

typedef uint16_t    UartFrame_Head_t;      // 帧头
typedef uint16_t    UartFrame_Len_t;       // 数据长度
typedef uint8_t     UartFrame_Data_t[128]; // 数据
typedef uint16_t    UartFrame_CRC_t;       // CRC校验
typedef uint16_t    UartFrame_Tail_t;      // 帧尾

typedef struct 
{
    UartFrame_Head_t  head;
    UartFrame_Len_t   len;
    UartFrame_Data_t  data;
    UartFrame_CRC_t   crc;
    UartFrame_Tail_t  tail;
} UsartFrame_t; // 数据包

typedef struct 
{
    UART_HandleTypeDef* huart;
    UartFrame_Head_t frameHead;
    UartFrame_Tail_t frameTail;
    UsartFrame_t frame;
    enum    // 接收状态机 
    {
        UARTBSP_STATE_HEAD,         //接收包头
        UARTBSP_STATE_LEN,          //接收长度
        UARTBSP_STATE_DATA,         //接收数据
        UARTBSP_STATE_CRC,          //接收CRC
        UARTBSP_STATE_TAIL          //接收包尾
    } state;
    //包处理函数
    void (*dataProcess)(uint8_t* data, uint16_t len);
    //错误处理函数
    void (*errorProcess)(uint16_t errorCode);
} UartStream_t; // 串口数据流处理
//错误码
typedef enum 
{
    //包头不匹配
    UARTBSP_ERROR_HEADUNMACH,
    //包尾不匹配
    UARTBSP_ERROR_TAILUNMACH,
    //CRC校验失败
    UARTBSP_ERROR_CRCFAIL,
    //数据包长度超出范围
    UARTBSP_ERROR_LENOUTRANGE,
} UartStream_ErrorCode_t;

// 串口数据流处理初始化
void UartStream_Init(UartStream_t* uartStream);
// 串口数据流处理接收中断
void UartStream_ReceiveIT_Callback(UART_HandleTypeDef *huart,UartStream_t* uartFrame);
// 串口包发送
void UartStream_Send(UartStream_t* uartStream, uint8_t* data, uint16_t len);


