#include "Channel.h"
#include "Config.h" 
#include <stdio.h>

//通道初始化
void Channel_Init(Channel_t* channel)
{
    //初始化通道
	HRpwm_Init(&channel->transform.buck);
	HRpwm_Init(&channel->transform.boost);
}
//设置输出电压
void Channel_SetTargetVoltage(Channel_t* channel, float voltage)
{
    Pid_SetTarget(&channel->pid, voltage);
}
//周期性控制，需要在定时中断中调用
void Channel_PeriodicExecute(Channel_t* channel)
{
    float ratio = 
        Sampling_GetValueMapped(&channel->sample.source_voltage) / 
        Pid_Calculate(&channel->pid, Sampling_GetValueMapped(&channel->sample.output_voltage));
    //float delta = 0.5f * (ratio - 1.0f)/(ratio + 1.0f);

	
    //HRpwm_SetDuty(&channel->transform.buck, 0.5f - delta);
    //HRpwm_SetDuty(&channel->transform.boost, 0.5f + delta);
}
