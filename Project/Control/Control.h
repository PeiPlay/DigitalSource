#pragma once
#include "main.h"
#include "Channel.h"
#include "DwtClock.h"
#include "HRpwm.h"
#include "Pid.h"
#include "TimedInterrupt.h"


//控制初始化
void Control_Init(void);
//周期执行的控制函数，用于各个通道的输出控制
void Control_PeriodicExecute(void);
//通过从0开始的索引获取对应的通道
Channel_t* Control_GetChannel(uint32_t index);
//获取当前电源的
uint32_t Control_GetChannelNum(void);

extern TimedInterrupt_t ControlStream;

