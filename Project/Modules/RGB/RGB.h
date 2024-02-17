#pragma once
#include "main.h"

typedef struct
{
    TIM_HandleTypeDef* htim;
    uint32_t red_channel;
    uint32_t green_channel;
    uint32_t blue_channel;
} RGB_PWM_t;
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGB_Color_t;

typedef struct
{
    RGB_PWM_t pwm; 
    RGB_Color_t color;
    uint8_t is_shutDown:1;
} RGB_t;


void RGB_Init(RGB_t* rgb);
void RGB_SetColor(RGB_t* rgb, RGB_Color_t color);
void RGB_SetColorRGB(RGB_t* rgb, uint8_t red, uint8_t green, uint8_t blue);
void RGB_SetColor24B(RGB_t* rgb, uint32_t color24b);
void RGB_ShuDown(RGB_t* rgb);
void RGB_Enlight(RGB_t* rgb);
void RGB_Toggle(RGB_t* rgb);
