#include "Channel.h"

//通道初始化
void Channel_Init(Channel_t* channel)
{
    HRpwm_Init(&channel->transform.buck);
	HRpwm_Init(&channel->transform.boost);
}

void Channel_Callback_TIM(Channel_t* channel)
{
    float ratio = 
        Sampler_GetValueMapped(&channel->sample.source_voltage) / 
        Pid_Calculate(&channel->pid, Sampler_GetValueMapped(&channel->sample.output_voltage));
    float delta = 0.5f * (ratio - 1.0f)/(ratio + 1.0f);
    HRpwm_SetDuty(&channel->transform.buck, 0.5f - delta);
    HRpwm_SetDuty(&channel->transform.boost, 0.5f + delta);
}

//设置输出电压
void Channel_SetTargetVoltage(Channel_t* channel, float voltage)
{
    Pid_SetTarget(&channel->pid, voltage);
}
//设置采样器映射
void Channel_SetSourceSamplerMap(Channel_t* channel, float k, float b)
{
    channel->sample.source_voltage.map_k = k;
    channel->sample.source_voltage.map_b = b;
}
void Channel_SetOutputSamplerMap(Channel_t* channel, float k, float b)
{
    channel->sample.output_voltage.map_k = k;
    channel->sample.output_voltage.map_b = b;
}
//设置PID参数
void Channel_SetTransformPid(Channel_t* channel, Pid_t pid)
{
    channel->pid = pid;
}
