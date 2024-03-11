#include "adc_bsp.h"
#include <stdlib.h>

void AdcBsp_Init(AdcBsp_t* adc)
{
	if(adc->filt_rate * adc->ch_cnt > ADCBSP_BUFFER_SIZE) {Error_Handler();}
    HAL_ADCEx_Calibration_Start(adc->hadc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
	HAL_Delay(10);
    HAL_ADC_Start_DMA(&hadc1, (void*)adc->channels, adc->ch_cnt * adc->filt_rate);
	HAL_Delay(10);
}
uint16_t AdcBsp_GetValue(AdcBsp_t* adc, uint32_t noc)
{
   if (adc == NULL || noc > adc->ch_cnt || noc == 0) return 0;
   uint32_t sum = 0;
   for(uint32_t i = 0; i < adc->filt_rate; i++)
   {
       sum += adc->channels[(noc - 1) + i * adc->ch_cnt];
   }
   return sum / adc->filt_rate;
}


