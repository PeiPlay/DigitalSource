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
}
void Control_HrtimIT_Callback(void)
{
    for(uint32_t i = 0; i < CONTROL_CHANNELS_NUM; i++)
        Channel_HrtimIT_Callback(&channels[i]);
}

Channel_t* Control_GetChannel(uint32_t ch_index)
{
    return (ch_index < CONTROL_CHANNELS_NUM) ? &channels[ch_index] : NULL;
}


