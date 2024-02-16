#include "RGB.h"
#include "tim.h"
#include "stm32h750xx.h"


uint32_t _RGB_GetCounterFreq(RGB_t* rgb)
{
    TIM_TypeDef* tim = rgb->pwm.htim->Instance;
#if defined(TIM1)
    if(tim == TIM1)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM2)
    if(tim == TIM2)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM3)
    if(tim == TIM3)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM4)
    if(tim == TIM4)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM5)
    if(tim == TIM5)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM6)
    if(tim == TIM6)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM7)
    if(tim == TIM7)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM8)
    if(tim == TIM8)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM9)
    if(tim == TIM9)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM10)
    if(tim == TIM10)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM11)
    if(tim == TIM11)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM12)
    if(tim == TIM12)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM13)
    if(tim == TIM13)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM14)
    if(tim == TIM14)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM15)
    if(tim == TIM15)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM16)
    if(tim == TIM16)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM17)
    if(tim == TIM17)
        goto _RGB_APB2_TIM_FREQ;
#endif
#if defined(TIM18)
    if(tim == TIM18)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM19)
    if(tim == TIM19)
        goto _RGB_APB1_TIM_FREQ;
#endif
#if defined(TIM20)
    if(tim == TIM20)
        goto _RGB_APB1_TIM_FREQ;
#endif
    Error_Handler();

	uint32_t pclk1, pclk2;
//计算对应定时器的APB? timer clock的频率
_RGB_APB1_TIM_FREQ://APB1 timer clock的频率
    pclk1 = HAL_RCC_GetPCLK1Freq();
    if((RCC->D2CFGR & RCC_D2CFGR_D2PPRE1) == 0)
        return (pclk1);
    else
        return(2 * pclk1);
_RGB_APB2_TIM_FREQ://APB2 timer clock的频率
    pclk2 = HAL_RCC_GetPCLK2Freq();
    if((RCC->D2CFGR & RCC_D2CFGR_D2PPRE2) == 0)
        return (pclk2);
    else
        return(2 * pclk2);
}
void _RGB_Refresh(RGB_t* rgb)
{
    if(rgb->is_shutDown)
    {
        __HAL_TIM_SetCompare(rgb->pwm.htim, rgb->pwm.red_channel, 0);
        __HAL_TIM_SetCompare(rgb->pwm.htim, rgb->pwm.green_channel, 0);
        __HAL_TIM_SetCompare(rgb->pwm.htim, rgb->pwm.blue_channel, 0);
    }
    else
    {
        float ratio = ((float)rgb->color.red / 255.0f) * 1000.0f;
        __HAL_TIM_SetCompare(rgb->pwm.htim, rgb->pwm.red_channel, (uint32_t)ratio);
        ratio = ((float)rgb->color.green / 255.0f) * 1000.0f;
        __HAL_TIM_SetCompare(rgb->pwm.htim, rgb->pwm.green_channel, (uint32_t)ratio);
        ratio = ((float)rgb->color.blue / 255.0f) * 1000.0f;
        __HAL_TIM_SetCompare(rgb->pwm.htim, rgb->pwm.blue_channel, (uint32_t)ratio);
    }

}
void RGB_Init(RGB_t* rgb)
{
    uint32_t clk_freq = _RGB_GetCounterFreq(rgb);
    uint32_t prescaler = (clk_freq / (1000000)) - 1;  // 预分频器
    // 初始化定时器 __HAL_TIM_SET_PRESCALER(&htim, prescaler);
    __HAL_TIM_SET_PRESCALER(rgb->pwm.htim, prescaler);
    __HAL_TIM_SetAutoreload(rgb->pwm.htim, 1000);
    _RGB_Refresh(rgb);
    HAL_TIM_PWM_Start(rgb->pwm.htim, rgb->pwm.red_channel);
    HAL_TIM_PWM_Start(rgb->pwm.htim, rgb->pwm.green_channel);
    HAL_TIM_PWM_Start(rgb->pwm.htim, rgb->pwm.blue_channel);
}
void RGB_SetColorRGB(RGB_t* rgb, uint8_t red, uint8_t green, uint8_t blue)
{
    rgb->color.red = red;
    rgb->color.green = green;
    rgb->color.blue = blue;
    _RGB_Refresh(rgb);
}
void RGB_SetColor(RGB_t* rgb, RGB_Color_t color)
{
    rgb->color = color;
    _RGB_Refresh(rgb);
}

void RGB_SetColor24B(RGB_t* rgb, uint32_t color24b)
{
    rgb->color.red = (color24b >> 16) & 0xFF;
    rgb->color.green = (color24b >> 8) & 0xFF;
    rgb->color.blue = color24b & 0xFF;
    _RGB_Refresh(rgb);
}
void RGB_ShuDown(RGB_t* rgb)
{
    rgb->is_shutDown = 1;
    _RGB_Refresh(rgb);
}
void RGB_Enlight(RGB_t* rgb)
{
    rgb->is_shutDown = 0;
    _RGB_Refresh(rgb);
}
void RGB_Toggle(RGB_t* rgb)
{
    rgb->is_shutDown = !rgb->is_shutDown;
    _RGB_Refresh(rgb);
}

