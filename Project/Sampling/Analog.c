#include "Analog.h"
#include "adc.h"
#include "Config.h"

#if(ANALOG_ADC1_ACTIVED_CHANNELS > 0)

uint16_t ADC1ConvertedValues[ANALOG_ADC1_ACTIVED_CHANNELS * ANALOG_ADC1_MEANFILTER_RATIO];
#endif
#if(ANALOG_ADC2_ACTIVED_CHANNELS > 0)
uint16_t ADC2ConvertedValues[ANALOG_ADC2_ACTIVED_CHANNELS * ANALOG_ADC2_MEANFILTER_RATIO];
#endif
#if(ANALOG_ADC3_ACTIVED_CHANNELS > 0)
uint16_t ADC3ConvertedValues[ANALOG_ADC3_ACTIVED_CHANNELS * ANALOG_ADC3_MEANFILTER_RATIO];
#endif
#if(ANALOG_ADC4_ACTIVED_CHANNELS > 0)
uint16_t ADC4ConvertedValues[ANALOG_ADC4_ACTIVED_CHANNELS * ANALOG_ADC4_MEANFILTER_RATIO];
#endif
#if(ANALOG_ADC5_ACTIVED_CHANNELS > 0)
uint16_t ADC5ConvertedValues[ANALOG_ADC5_ACTIVED_CHANNELS * ANALOG_ADC5_MEANFILTER_RATIO];
#endif

void Analog_Init(void)
{
#if(ANALOG_ADC1_ACTIVED_CHANNELS > 0)
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_Delay(1);
    HAL_ADC_Start_DMA(&hadc1, (void*)ADC1ConvertedValues, ANALOG_ADC1_ACTIVED_CHANNELS * ANALOG_ADC1_MEANFILTER_RATIO);
    HAL_Delay(1);
#endif
#if(ANALOG_ADC2_ACTIVED_CHANNELS > 0)
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_Delay(1);
    HAL_ADC_Start_DMA(&hadc2, (void*)ADC2ConvertedValues, ANALOG_ADC2_ACTIVED_CHANNELS * ANALOG_ADC2_MEANFILTER_RATIO);
    HAL_Delay(1);
#endif
#if(ANALOG_ADC3_ACTIVED_CHANNELS > 0)
    HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_Delay(1);
    HAL_ADC_Start_DMA(&hadc3, (void*)ADC3ConvertedValues, ANALOG_ADC3_ACTIVED_CHANNELS * ANALOG_ADC3_MEANFILTER_RATIO);
    HAL_Delay(1);
#endif
#if(ANALOG_ADC4_ACTIVED_CHANNELS > 0)
    HAL_ADCEx_Calibration_Start(&hadc4, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_Delay(1);
    HAL_ADC_Start_DMA(&hadc4, (void*)ADC4ConvertedValues, ANALOG_ADC4_ACTIVED_CHANNELS * ANALOG_ADC4_MEANFILTER_RATIO);
    HAL_Delay(1);
#endif
#if(ANALOG_ADC5_ACTIVED_CHANNELS > 0)
    HAL_ADCEx_Calibration_Start(&hadc5, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_Delay(1);
    HAL_ADC_Start_DMA(&hadc5, (void*)ADC5ConvertedValues, ANALOG_ADC5_ACTIVED_CHANNELS * ANALOG_ADC5_MEANFILTER_RATIO);
    HAL_Delay(1);
#endif
}
inline uint32_t Analog_GetValue(Analog_t *analog, uint8_t index)
{
	ADC_TypeDef* hadc_inst =  analog->hadc->Instance;
#if(ANALOG_ADC1_ACTIVED_CHANNELS > 0)
	if(hadc_inst == ADC1) return ADC1ConvertedValues[analog->noc - 1 + index * ANALOG_ADC1_ACTIVED_CHANNELS];
#endif
#if(ANALOG_ADC2_ACTIVED_CHANNELS > 0)
    if(hadc_inst == ADC2) return ADC2ConvertedValues[analog->noc - 1 + index * ANALOG_ADC2_ACTIVED_CHANNELS];
#endif
#if(ANALOG_ADC3_ACTIVED_CHANNELS > 0)
	if(hadc_inst == ADC3) return ADC3ConvertedValues[analog->noc - 1 + index * ANALOG_ADC3_ACTIVED_CHANNELS];
#endif
#if(ANALOG_ADC4_ACTIVED_CHANNELS > 0)
    if(hadc_inst == ADC4) return ADC4ConvertedValues[analog->noc - 1 + index * ANALOG_ADC4_ACTIVED_CHANNELS];
#endif
#if(ANALOG_ADC5_ACTIVED_CHANNELS > 0)
	if(hadc_inst == ADC5) return ADC5ConvertedValues[analog->noc - 1 + index * ANALOG_ADC5_ACTIVED_CHANNELS];
#endif
	Error_Handler();
	return 0;
}
inline uint32_t Analog_GetAverage(Analog_t *analog)
{
    ADC_TypeDef* hadc_inst =  analog->hadc->Instance;
    uint16_t* ADCConvertedValues = NULL;
    uint32_t step_num = 0;
    uint32_t step_len = 0;
    uint32_t sum = 0;

#if(ANALOG_ADC1_ACTIVED_CHANNELS > 0)
	if(hadc_inst == ADC1) 
    {
        ADCConvertedValues = ADC1ConvertedValues;
        step_num = ANALOG_ADC1_MEANFILTER_RATIO;
        step_len = ANALOG_ADC1_ACTIVED_CHANNELS;
        goto Analog_ADC_Defineded;
    }
#endif
#if(ANALOG_ADC2_ACTIVED_CHANNELS > 0)
    if(hadc_inst == ADC2) 
    {
        ADCConvertedValues = ADC2ConvertedValues;
        step_num = ANALOG_ADC2_MEANFILTER_RATIO;
        step_len = ANALOG_ADC2_ACTIVED_CHANNELS;
        goto Analog_ADC_Defineded;
    }
#endif
#if(ANALOG_ADC3_ACTIVED_CHANNELS > 0)
	if(hadc_inst == ADC3) 
    {
        ADCConvertedValues = ADC3ConvertedValues;
        step_num = ANALOG_ADC3_MEANFILTER_RATIO;
        step_len = ANALOG_ADC3_ACTIVED_CHANNELS;
        goto Analog_ADC_Defineded;
    }
#endif
#if(ANALOG_ADC4_ACTIVED_CHANNELS > 0)
    if(hadc_inst == ADC4) 
    {
        ADCConvertedValues = ADC4ConvertedValues;
        step_num = ANALOG_ADC4_MEANFILTER_RATIO;
        step_len = ANALOG_ADC4_ACTIVED_CHANNELS;
        goto Analog_ADC_Defineded;
    }
#endif
#if(ANALOG_ADC5_ACTIVED_CHANNELS > 0)
	if(hadc_inst == ADC5) 
    {
        ADCConvertedValues = ADC5ConvertedValues;
        step_num = ANALOG_ADC5_MEANFILTER_RATIO;
        step_len = ANALOG_ADC5_ACTIVED_CHANNELS;
        goto Analog_ADC_Defineded;
    }
#endif
    Error_Handler();
Analog_ADC_Defineded:
    if(ADCConvertedValues == NULL) Error_Handler();
    for(uint32_t i = 0; i < step_num; i++)
    {
        sum += ADCConvertedValues[i * step_len + analog->noc - 1];
    }
    return sum / step_num;
}
