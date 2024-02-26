#include "Config.h"

AdcBsp_t bsp_adc1;
AdcBsp_t bsp_adc2;

Channel_t channels[CONTROL_CHANNELS_NUM];

TimedInterrupt_t controlStream;

UpperComputer_t upperComputer;

