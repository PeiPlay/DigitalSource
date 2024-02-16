#include "Control.h"
#include "Config.h"

// 数控电源输出通道数
#ifndef CONTROL_CHANNEL_NUM // 如果没有定义数控电源输出通道数,则默认为0（不输出）
    #define CONTROL_CHANNEL_NUM 0
#endif
Channel_t Channels[CONTROL_CHANNEL_NUM];
TimedInterrupt_t ControlStream;


void Control_Init(void)
{
#ifdef CONTROL_CHANNEL1_INIT
    Channels[0] = CONTROL_CHANNEL1_INIT;
#endif
#ifdef CONTROL_CHANNEL2_INIT
    Channels[1] = CONTROL_CHANNEL2_INIT;
#endif
#ifdef CONTROL_CHANNEL3_INIT
    Channels[2] = CONTROL_CHANNEL3_INIT;
#endif
#ifdef CONTROL_CHANNEL4_INIT
    Channels[3] = CONTROL_CHANNEL4_INIT;
#endif
	for(uint32_t i = 0; i < CONTROL_CHANNEL_NUM; i++)
		Channel_Init(&Channels[i]);
    //ControlStream = CONTROL_STREAM_INIT;
    //TimedInterrupt_AddTask(&ControlStream, Control_PeriodicExecute);
    //TimedInterrupt_AddTask(&ControlStream, DwtClock_OverflowProtect);
    //TimedInterrupt_Init(&ControlStream);
}
// 通过从0开始的索引获取对应的通道
Channel_t *Control_GetChannel(uint32_t index)
{
    if (index >= CONTROL_CHANNEL_NUM)
        return NULL;
    return &Channels[index];
}
uint32_t Control_GetChannelNum(void)
{
    return CONTROL_CHANNEL_NUM;
}
void Control_PeriodicExecute(void)
{
    for (uint32_t i = 0; i < CONTROL_CHANNEL_NUM; i++)
        Channel_PeriodicExecute(&Channels[i]);
}



