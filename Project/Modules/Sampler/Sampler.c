#include "Sampler.h"

float Sampler_GetValue(Sampler_t* sampler)
{
    uint32_t sum = 0;
    for(uint32_t i = 0; i < sampler->adc_cnt; i++)
    {
        sum += AdcBsp_GetValue(sampler->adc_arr[i], sampler->noc);
    }
    return sum / sampler->adc_cnt;
}
float Sampler_GetValueMapped(Sampler_t* sampler)
{
    return Sampler_GetValue(sampler) * sampler->map_k + sampler->map_b;
}

