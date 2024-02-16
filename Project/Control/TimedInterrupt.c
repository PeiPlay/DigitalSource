#include "TimedInterrupt.h"
#include <stdio.h>

typedef enum 
{
    TimedInterrupt_ErrorCode_UndefinedTimer = 0,
    TimedInterrupt_ErrorCode_IntPeriodError,
} TimedInterrupt_ErrorCode_t;

void TimedInterrupt_ErrorHandler(TimedInterrupt_ErrorCode_t error_code)
{
    Error_Handler();
}
//得到定时器计数器的频率
uint32_t _TimedInterrupt_GetCounterFreq(TimedInterrupt_t* timedInterrupt)
{
    TIM_TypeDef* tim = timedInterrupt->htim->Instance;
#if defined(TIM1)
    if(tim == TIM1)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM2)
    if(tim == TIM2)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM3)
    if(tim == TIM3)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM4)
    if(tim == TIM4)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM5)
    if(tim == TIM5)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM6)
    if(tim == TIM6)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM7)
    if(tim == TIM7)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM8)
    if(tim == TIM8)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM9)
    if(tim == TIM9)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM10)
    if(tim == TIM10)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM11)
    if(tim == TIM11)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM12)
    if(tim == TIM12)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM13)
    if(tim == TIM13)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM14)
    if(tim == TIM14)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM15)
    if(tim == TIM15)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM16)
    if(tim == TIM16)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM17)
    if(tim == TIM17)
        goto _TimedInterrupt_APB2_TIM_FREQ;
#endif
#if defined(TIM18)
    if(tim == TIM18)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM19)
    if(tim == TIM19)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
#if defined(TIM20)
    if(tim == TIM20)
        goto _TimedInterrupt_APB1_TIM_FREQ;
#endif
    TimedInterrupt_ErrorHandler(TimedInterrupt_ErrorCode_UndefinedTimer);

	uint32_t pclk1, pclk2;
//计算对应定时器的APB? timer clock的频率
_TimedInterrupt_APB1_TIM_FREQ://APB1 timer clock的频率
    pclk1 = HAL_RCC_GetPCLK1Freq();
    if((RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) == 0)
        return (pclk1);
    else
        return(2 * pclk1);
_TimedInterrupt_APB2_TIM_FREQ://APB2 timer clock的频率
    pclk2 = HAL_RCC_GetPCLK2Freq();
    if((RCC->D2CFGR & RCC_D2CFGR_D2PPRE2) == 0)
        return (pclk2);
    else
        return(2 * pclk2);
}
//返回中断周期的计数值
uint64_t _TimedInterrupt_GetIntPeriodCnt(TimedInterrupt_t* timedInterrupt)
{
    return ((uint64_t)timedInterrupt->intPeriod * (uint64_t)(_TimedInterrupt_GetCounterFreq(timedInterrupt) / 1000000));
}
void _TimedInterrupt_Shutdown(TimedInterrupt_t* timedInterrupt)
{
    TimedInterrupt_Timer_t htim = timedInterrupt->htim;
    HAL_TIM_Base_Stop_IT(htim);
    HAL_TIM_Base_Stop(htim);
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
    __HAL_TIM_SET_COUNTER(htim, 0);
}
void _TimedInterrupt_Start(TimedInterrupt_t* timedInterrupt)
{
    _TimedInterrupt_Shutdown(timedInterrupt);
    TimedInterrupt_Timer_t htim = timedInterrupt->htim;
    HAL_TIM_Base_Start_IT(htim);
    HAL_TIM_Base_Start(htim);
}

void TimedInterrupt_Init(TimedInterrupt_t* timedInterrupt)
{
    uint64_t intPeriodCnt = _TimedInterrupt_GetIntPeriodCnt(timedInterrupt);
    if(intPeriodCnt == 0 || intPeriodCnt > 0x100000000)
        TimedInterrupt_ErrorHandler(TimedInterrupt_ErrorCode_IntPeriodError);
    //计算定时器的分频系数和自动重装值
    uint32_t prescaler, period;
    uint64_t best_product = 0;
    for (uint32_t i = 1; i <= 65536; ++i) {
        uint32_t j = intPeriodCnt / i;
        if (j > 65536) continue;
        uint64_t product = i * j;
        if (product > best_product) {
            best_product = product;
            prescaler = i;
            period = j;
        }
		else if(product == intPeriodCnt)
			break;
    }
    //初始化定时器
    TimedInterrupt_Timer_t htim = timedInterrupt->htim;
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    HAL_TIM_Base_Stop_IT(htim);
    HAL_TIM_Base_Stop(htim);
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
    __HAL_TIM_SET_COUNTER(htim, 0);

    HAL_TIM_Base_DeInit(htim);
    htim->Init.Prescaler = prescaler - 1; //设置分频系数
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;        //向上计数
    htim->Init.Period = period - 1;                              //设置自动重装载值
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;  //关闭自动重装载
    if (HAL_TIM_Base_Init(htim) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    _TimedInterrupt_Start(timedInterrupt);
}

void TimedInterrupt_Callback_TIM(TimedInterrupt_t* timedInterrupt, TimedInterrupt_Timer_t htim)
{
    if(htim->Instance != timedInterrupt->htim->Instance)
        return;
    if(timedInterrupt->mode == TimedInterrupt_Mode_Obstructed)
    {
        _TimedInterrupt_Shutdown(timedInterrupt);
        for(uint16_t i = 0; i < timedInterrupt->taskCount; ++i)
            timedInterrupt->taskBuff[i]();
        _TimedInterrupt_Start(timedInterrupt);
    }
    else
    {
        _TimedInterrupt_Start(timedInterrupt);
        for(uint16_t i = 0; i < timedInterrupt->taskCount; ++i)
            timedInterrupt->taskBuff[i]();
    }
}
void TimedInterrupt_AddTask(TimedInterrupt_t* timedInterrupt, TimedInterrupt_TaskFunction_t task)
{
    if(timedInterrupt->taskCount >= TIMED_INTERRUPT_MAX_TASKS)
        Error_Handler();
    timedInterrupt->taskBuff[timedInterrupt->taskCount++] = task;
}
void TimedInterrupt_RemoveTask(TimedInterrupt_t* timedInterrupt, TimedInterrupt_TaskFunction_t task)
{
    for(uint16_t i = 0; i < timedInterrupt->taskCount; ++i)
    {
        if(timedInterrupt->taskBuff[i] == task)
        {
            for(uint16_t j = i; j < timedInterrupt->taskCount - 1; ++j)
                timedInterrupt->taskBuff[j] = timedInterrupt->taskBuff[j + 1];
            --timedInterrupt->taskCount;
            return;
        }
    }
}
