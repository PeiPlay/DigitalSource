#pragma once
#include "main.h"
#include "HRpwm.h"
#include "Sampling.h"
#include "Pid.h"
//通道检测端口

typedef struct 
{
    struct Control_Transform_t
    {    
        HRpwm_t buck;
        HRpwm_t boost;
    } transform;
    struct Control_Sample_t
    {
        Sampling_t source_voltage;
        Sampling_t output_voltage;
    } sample;
    //输出电压调节Pid
    Pid_t pid;
} Channel_t;
//通道初始化
void Channel_Init(Channel_t* channel);
//设置输出电压
void Channel_SetTargetVoltage(Channel_t* channel, float voltage);
//周期性控制，需要在定时中断中调用
void Channel_PeriodicExecute(Channel_t* channel);


