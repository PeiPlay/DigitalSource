#include "Control.h"
#include "Config.h" 


void Control_Init(void)
{
    //初始化ADC
    bsp_adc1 = BSP_ADC1_INIT;
    bsp_adc2 = BSP_ADC2_INIT;
    AdcBsp_Init(&bsp_adc1);
    AdcBsp_Init(&bsp_adc2);
    //初始化通道
    
#ifdef CONTROL_CHANNEL0_INIT
    channels[0] = CONTROL_CHANNEL0_INIT;
#endif
#ifdef CONTROL_CHANNEL1_INIT
    channels[1] = CONTROL_CHANNEL1_INIT;
#endif
#ifdef CONTROL_CHANNEL2_INIT
    channels[2] = CONTROL_CHANNEL2_INIT;
#endif
#ifdef CONTROL_CHANNEL3_INIT
    channels[3] = CONTROL_CHANNEL3_INIT;
#endif
    for(uint32_t i = 0; i < CONTROL_CHANNELS_NUM; i++)
    {
        Channel_Init(&channels[i]);
    }
	return;
	
    //初始化定时中断
    controlStream = CONTROL_STREAM_INIT;
    controlStream.taskCount = 0;
    TimedInterrupt_AddTask(&controlStream, Control_Callback_TIM);
    TimedInterrupt_AddTask(&controlStream, DwtClock_OverflowProtect);
    TimedInterrupt_Init(&controlStream);
}

void Control_Callback_TIM(void)
{
    for(uint32_t i = 0; i < CONTROL_CHANNELS_NUM; i++)
        Channel_Callback_TIM(&channels[i]);
}

Channel_t* Control_GetChannel(uint32_t ch_index)
{
    return (ch_index < CONTROL_CHANNELS_NUM) ? &channels[ch_index] : NULL;
}
