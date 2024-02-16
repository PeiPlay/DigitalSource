#include "Sampling.h"
#include "Config.h"


//得到双路均值滤波值
inline uint32_t Sampling_GetValue(Sampling_t* sampling)
{
    return (Analog_GetAverage(&sampling->analog1) + Analog_GetAverage(&sampling->analog2)) / 2;
}
//得到映射值
inline float Sampling_GetValueMapped(Sampling_t* sampling)
{
    return (float)(Sampling_GetValue(sampling)) * sampling->map_k + sampling->map_b;
}
