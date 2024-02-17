/*
* @file uart_bsp
* @brief 
*/

#pragma once 

#include "main.h"
#include "usart.h"


typedef uint16_t    UartBspFrame_Head_t;      // 帧头
typedef uint16_t    UartBspFrame_Len_t;       // 数据长度
typedef uint8_t     UartBspFrame_Data_t[128]; // 数据
typedef uint16_t    UartBspFrame_CRC_t;       // CRC校验
typedef uint16_t    UartBspFrame_Tail_t;      // 帧尾

typedef struct 
{
    UartBspFrame_Head_t  head;
    UartBspFrame_Len_t   len;
    UartBspFrame_Data_t  data;
    UartBspFrame_CRC_t   crc;
    UartBspFrame_Tail_t  tail;
} UartBspFrame_t; // 数据包

typedef struct 
{
    UART_HandleTypeDef* huart;
    UartBspFrame_Head_t frameHead;
    UartBspFrame_Tail_t frameTail;
    UartBspFrame_t      frame;
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
} UartBsp_t; // 串口数据流处理

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
} UartBsp_ErrorCode_t;

// 串口数据流处理初始化
void UartBsp_Init(UartBsp_t* uartBsp);
// 串口数据流处理接收中断
void UartBsp_ReceiveIT_Callback(UART_HandleTypeDef *huart, UartBsp_t* uartBsp);
// 串口包发送
void UartBsp_Send(UartBsp_t* uartBsp, uint8_t* data, uint16_t len);
