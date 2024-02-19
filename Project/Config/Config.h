#pragma once 
#include "main.h"


//*--------------------引入需要的板级支持包----------------------*//
#include "adc_bsp.h"
// #include "fdcan_bsp.h"
#include "flash_bsp.h"
#include "tim_bsp.h"
#include "uart_bsp.h"
// #include "usb_bsp.h"
//*--------------------引入需要的模组工具----------------------*//
#include "DwtClock.h"
#include "Eeprom.h"
#include "HRpwm.h"
#include "OledUI.h"
#include "Pid.h"
#include "RGB.h"
#include "Sampler.h"
#include "TimedInterrupt.h"

//*++++++++++++++++++++++++++电压变换Transformation++++++++++++++++++++++++++*//
#include "Control.h"
#include "Channel.h"

extern AdcBsp_t bsp_adc1;
extern AdcBsp_t bsp_adc2;

#define BSP_ADC1_INIT (AdcBsp_t){                                   \
    .hadc = &hadc1,                                                 \
    .ch_cnt = 2,                                                    \
    .filt_rate = 4                                                  \
}
#define BSP_ADC2_INIT (AdcBsp_t){                                   \
    .hadc = &hadc2,                                                 \
    .ch_cnt = 2,                                                    \
    .filt_rate = 4                                                  \
}

#define CONTROL_CHANNELS_NUM 1
extern Channel_t channels[];

#define CONTROL_CHANNEL0_INIT (Channel_t) {                         \
   .transform = {                                                   \
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
            .adc_arr = {&bsp_adc1, &bsp_adc2},                      \
            .adc_cnt = 2,                                           \
            .noc = 1,                                               \
            .map_k = 0.0008f,                                       \
            .map_b = 0.0f                                           \
        },                                                          \
        .source_voltage = {                                         \
            .adc_arr = {&bsp_adc1, &bsp_adc2},                      \
            .adc_cnt = 2,                                           \
            .noc = 3,                                               \
            .map_k = 0.0008f,                                       \
            .map_b = 0.0f                                           \
        },                                                          \
    },                                                              \
    .pid = {                                                        \
        .kp = 0,            .ki = 0,            .kd = 0,            \
        .integral_startzone = 20.0f,    .integral_deadband  = 0,    \
        .integral_max     	= 1.0f,     .integral_min       = 0,    \
        .output_max         = 1.0f,     .output_min         = 0     \
    }                                                               \
}








