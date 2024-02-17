/*
* @file    adc_bsp
* @brief   ADC板级支持包，提供ADC-DMA的功能封装
*          可在CubeMX中配置ADC-DMA，转换速度溢出可以通过过采样牺牲速度进行滤波
*/
#pragma once

#include "main.h"
#include "adc.h"

typedef struct 
{
    ADC_HandleTypeDef* hadc; //ADC句柄
    uint32_t ch_cnt;            //通道数量
    uint32_t filt_rate;         //滤波倍数
    uint16_t* channels;         //通道转换结果数组x
} AdcBsp_t;

void AdcBsp_Init(AdcBsp_t* adc);
uint16_t AdcBsp_GetValue(AdcBsp_t* adc, uint32_t noc);
void AdcBsp_Deinit(AdcBsp_t* adc);
