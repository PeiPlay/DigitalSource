#include "RGB.h"
#include "tim_bsp.h"

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
    uint32_t clk_freq = TimBsp_GetCounterFreq(rgb->pwm.htim);
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

