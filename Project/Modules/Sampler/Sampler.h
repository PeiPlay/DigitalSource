#pragma once 
#include "main.h"
#include "adc_bsp.h"

typedef struct 
{
    AdcBsp_t* adc_arr[5];
    uint32_t  adc_cnt;
    uint32_t  noc;
    float map_k;
    float map_b;
} Sampler_t;

float Sampler_GetValue(Sampler_t* sampler);
float Sampler_GetValueMapped(Sampler_t* sampler);
