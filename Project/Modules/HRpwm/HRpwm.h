#pragma once 
#include "main.h"
#include "hrtim.h"

typedef struct 
{
    HRTIM_HandleTypeDef* hhrtim;
    uint32_t timerIdentifier; //HRTIM_TIMERID_TIMER_X
    uint32_t _timerIndex; //HRTIM_TIMERINDEX_TIMER_X
    uint32_t _outputChannel; 
    uint8_t isInversed;
    float dutyLimit;
    float defaultDuty;  //初始化时设置的占空比
    uint32_t period;
    float duty;
} HRpwm_t;

void HRpwm_Init(HRpwm_t* hrpwm);

void HRpwm_SetDuty(HRpwm_t* hrpwm, float duty);
float HRpwm_GetDuty(HRpwm_t* hrpwm);
