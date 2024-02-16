#pragma once
#include "main.h" 

/*
 * File: Analog.h
 * Description: 
 *      Analog组件用于实现ADC+DMA的模拟量采集
 *      DMA将采集到的数据存入对应ADC的数组中
 *      数组的长度可自定义成开启通道数的n倍用于均值滤波，牺牲速度换取精度
 */


typedef struct 
{
    ADC_HandleTypeDef *hadc;    // ADC句柄
    uint8_t noc;                // 采集通道数(从1开始)
} Analog_t;

// 初始化所有ADC通道
void Analog_Init(void);
// 获取ADC通道的值，其中index从0开始，表示得到缓冲区
uint32_t Analog_GetValue(Analog_t *analog, uint8_t index);
uint32_t Analog_GetAverage(Analog_t *analog);

