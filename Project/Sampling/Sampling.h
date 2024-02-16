#pragma once
#include "main.h"
#include "Analog.h"


//采用双路同时检测的均值滤波
typedef struct
{
    Analog_t analog1;
    Analog_t analog2;
    float map_k;
    float map_b;
} Sampling_t;

//得到双路均值滤波值
uint32_t Sampling_GetValue(Sampling_t* sampling);
//得到映射值
float Sampling_GetValueMapped(Sampling_t* sampling);

