#pragma once
#include "main.h"
#include "uart_bsp.h"
#include "flash_bsp.h"

enum
{
    UPPERCOMPUTER_CMD_HEARTBEAT = 0,        //心跳响应
    UPPERCOMPUTER_CMD_READFLASH = 1,        //读取Flash中存储的参数
    UPPERCOMPUTER_CMD_SETPARAM = 2,         //设置参数
    UPPERCOMPUTER_CMD_DOWNLOADFLASH = 3,    //将参数写入Flash
    UPPERCOMPUTER_CMD_MAPPING = 4           //映射整定完成
};
typedef struct __attribute__((packed))
{
    float target;
    float kp;                   //比例系数
    float ki;                   //积分系数
    float kd;                   //微分系数
    float integralStartzone;    //积分分离
    float integralDeadband;     //误差死区
    float integralMax;          //积分上限
    float integralMin;          //积分下限
    float outputMax;            //输出上限
    float outputMin;            //输出下限

    float mapping_inputK;
    float mapping_inputB;
    float mapping_outputK;
    float mapping_outputB;
} UpperComputer_ParamInfo_t;

typedef struct __attribute__((packed))
{    
    float targetVoltage;    //目标电压
    float currentVoltage;   //当前电压
} UpperComputer_ChannelState_t;

typedef struct __attribute__((packed))
{
    uint8_t cmd;            //命令
    uint8_t channel;        //当前通道
    uint32_t synCode;       //同步码: 由上位机产生一个随机数，下发给下位机。下位机收到后，将此随机数取反后返回给上位机。
} UpperComputer_Lable_t; //上位机通信-包标签，用于命令识别

//心跳包
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
} UpperComputer_HeartbeatM_t; //上位机通信-心跳包-Master发送
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
    UpperComputer_ChannelState_t channelState;
} UpperComputer_HeartbeatS_t; //上位机通信-心跳包-Slave回传
//falsh读取包
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
} UpperComputer_ReadFlashM_t; //上位机通信-读取Flash-Master发送
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
    UpperComputer_ParamInfo_t paramInfo;
    UpperComputer_ChannelState_t channelState;
} UpperComputer_ReadFlashS_t; //上位机通信-读取Flash-Slave回传
//参数设置包
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
    UpperComputer_ParamInfo_t paramInfo;
} UpperComputer_SetParamM_t; //上位机通信-设置参数-Master发送
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
    UpperComputer_ChannelState_t channelState;
} UpperComputer_SetParamS_t; //上位机通信-设置参数-Slave回传
//falsh下载包
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
    UpperComputer_ParamInfo_t paramInfo;
} UpperComputer_DownloadFlashM_t; //上位机通信-下载Flash-Master发送
typedef struct __attribute__((packed))
{
    UpperComputer_Lable_t lable;
    UpperComputer_ChannelState_t channelState;
} UpperComputer_DownloadFlashS_t; //上位机通信-下载Flash-Slave回传

typedef struct 
{	
    FlashBsp_t flashmemory;
    UartBsp_t uartstream;
} UpperComputer_t;

void UpperComputer_Init(void);
void UpperComputer_DataProcess(uint8_t* data, uint16_t len);

