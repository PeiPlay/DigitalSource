#pragma once
#include "main.h"
#include "adc.h"
#include "hrtim.h"




//*******************************Sampling*******************************//
#include "Analog.h"
#include "Sampling.h"

#define ANALOG_ADC1_ACTIVED_CHANNELS 4 //ADC1被激活的通道数
#define ANALOG_ADC1_MEANFILTER_RATIO 8 //ADC1均值滤波器的比例

#define ANALOG_ADC2_ACTIVED_CHANNELS 4 //ADC2被激活的通道数
#define ANALOG_ADC2_MEANFILTER_RATIO 8 //ADC2均值滤波器的比例

//*******************************Control*******************************//
#include "Control.h"
#define CONTROL_CHANNEL_NUM 1 //数控电源输出通道数
#define CONTROL_CHANNEL1_INIT (Channel_t) {                         \
    .transform = {                                                  \
        .buck = {                                                   \
            .hhrtim = &hhrtim,                                      \
            .isInversed = 0,                                        \
            .dutyLimit = 0.99f,                                     \
            .timerIdentifier = HRTIM_TIMERID_TIMER_A,               \
            .defaultDuty = 0.5f,                                    \
        },                                                          \
        .boost = {                                                  \
            .hhrtim = &hhrtim,                                      \
            .isInversed = 0,                                        \
            .dutyLimit = 0.99f,                                     \
            .timerIdentifier = HRTIM_TIMERID_TIMER_B,               \
            .defaultDuty = 0.5f,                                    \
        }                                                           \
    },                                                              \
    .sample = {                                                     \
        .output_voltage = {                                         \
            .analog1 = {.hadc = &hadc1, .noc = 1},                  \
            .analog2 = {.hadc = &hadc2, .noc = 1},                  \
            .map_k = 0.0008f,                                          \
            .map_b = 0.0f                                           \
        },                                                          \
        .source_voltage = {                                         \
            .analog1 = {.hadc = &hadc1, .noc = 2},                  \
            .analog2 = {.hadc = &hadc2, .noc = 2},                  \
            .map_k = 1.0f,                                          \
            .map_b = 0.0f                                           \
        }                                                           \
    },                                                              \
    .pid = {                                                        \
        .kp = 0,            .ki = 0,            .kd = 0,            \
        .integral_startzone = 20.0f,    .integral_deadband  = 0,    \
        .integral_max     	= 1.0f,     .integral_min       = 0,    \
        .output_max         = 1.0f,     .output_min         = 0     \
    }                                                               \
}

#define CONTROL_STREAM_INIT (TimedInterrupt_t) {                    \
    .htim = &htim6,                                                 \
    .intPeriod = 1000,                                              \
    .mode = TimedInterrupt_Mode_Independent,                        \
}
