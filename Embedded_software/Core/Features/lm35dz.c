#include "lm35dz.h"
#include "adc.h"

float Lm35dz_Read(void)
{
	uint32_t adc_val;
	float temerature;

	HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1);

    adc_val = HAL_ADC_GetValue(&hadc1);
    temerature = (adc_val * 0.0805664);
	return temerature;
}
