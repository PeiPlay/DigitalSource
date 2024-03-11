#pragma once 
#include "HRpwm.h"
#include "Sampler.h"
#include "Pid.h"
#include <stdbool.h>

typedef struct
{
    struct {
        HRpwm_t buck;
        HRpwm_t boost;
    } transform;
    struct {
        Sampler_t source_voltage;
        Sampler_t output_voltage;
    } sample;
    Pid_t pid;
} Channel_t;

//通道初始化
void Channel_Init(Channel_t* channel);
//周期性控制，需要在定时中断中调用
void Channel_Callback_TIM(Channel_t* channel);

//设置输出电压
void Channel_SetTargetVoltage(Channel_t* channel, float voltage);
//设置采样器映射
void Channel_SetSourceSamplerMap(Channel_t* channel, float k, float b);
void Channel_SetOutputSamplerMap(Channel_t* channel, float k, float b);
//设置PID参数
void Channel_SetTransformPid(Channel_t* channel, Pid_t pid);
