#include "Sampler.h"

float Sampler_GetValue(Sampler_t* sampler)
{
   uint32_t sum = 0;
   for(uint32_t i = 0; i < sampler->analog_cnt; i++)
   {
       sum += AdcBsp_GetValue(sampler->analog_arr[i].hadc, sampler->analog_arr[i].noc);
   }
   return sum / sampler->analog_cnt;
}
float Sampler_GetValueMapped(Sampler_t* sampler)
{
    return Sampler_GetValue(sampler) * sampler->map_k + sampler->map_b;
}

