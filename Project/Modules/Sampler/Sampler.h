#pragma once 
#include "main.h"
#include "adc_bsp.h"

typedef struct 
{
    struct{
        AdcBsp_t* hadc;
        uint32_t  noc;
    } analog_arr[5];
    uint32_t analog_cnt;
    float map_k;
    float map_b;
} Sampler_t;

float Sampler_GetValue(Sampler_t* sampler);
float Sampler_GetValueMapped(Sampler_t* sampler);
