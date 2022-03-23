/*
 * ntc.c
 *
 *  Created on: 22 mar 2022
 *      Author: Tommaso Canova
 */

#include "ntc.h"

char ntc_buff [BUFF_SIZE];
double ntc_volt, ntc_adc_value;

double get_ntc_volt(ADC_HandleTypeDef* adc, uint32_t timeout){

	ntc_adc_value = 0.0;

	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, timeout);

	for (uint8_t i = 0; i < SAMPLES; i++){
		ntc_adc_value += HAL_ADC_GetValue(adc);
	}

	ntc_volt = (ntc_adc_value / SAMPLES) * GPIO_MAX_VOLTAGE / ADC_BIT_RESOLUTION;

	sprintf(ntc_buff, "NTC %f [V] \r\n",ntc_volt);
	HAL_UART_Transmit(&huart2, (uint8_t *)ntc_buff, strlen(ntc_buff), 100);

	return ntc_volt;
}

temperature_level get_ntc_temp_zone(double ntc_volt){

	temperature_level temp_zone = TEMP_ERROR;

	if(ntc_volt >= NTC_20_DEG_VOLTAGE_VALUE && ntc_volt <= GPIO_MAX_VOLTAGE){
		temp_zone = COLD;
		sprintf(ntc_buff, "NTC TEMP ZONE: COLD \r\n\r\n");
	}else if(ntc_volt >= NTC_40_DEG_VOLTAGE_VALUE && ntc_volt < NTC_20_DEG_VOLTAGE_VALUE){
		temp_zone = NORMAL;
		sprintf(ntc_buff, "NTC TEMP ZONE: NORMAL \r\n\r\n");
	}else if(ntc_volt >= NTC_60_DEG_VOLTAGE_VALUE && ntc_volt < NTC_40_DEG_VOLTAGE_VALUE){
		temp_zone = HOT;
		sprintf(ntc_buff, "NTC TEMP ZONE: HOT \r\n\r\n");
	}else if(ntc_volt >= NTC_125_DEG_VOLTAGE_VALUE && ntc_volt < NTC_60_DEG_VOLTAGE_VALUE){
		temp_zone = DANGER;
		sprintf(ntc_buff, "NTC TEMP ZONE: DANGER \r\n\r\n");
	}else{
		sprintf(ntc_buff, "NTC TEMP ZONE: NTC ERROR \r\n\r\n");
	}

	HAL_UART_Transmit(&huart2, (uint8_t *)ntc_buff, strlen(ntc_buff), 100);

	return temp_zone;
}


