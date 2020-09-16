/*
 * task1.c
 *
 *  Created on: 25 Aug 2020
 *      Author: linus
 */

#include "IO.h"
#include "adc.h"


void fire_HAWKs(void){
	HAL_GPIO_WritePin(HAWK1_GPIO_Port, HAWK1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(HAWK2_GPIO_Port, HAWK2_Pin, GPIO_PIN_SET);
}

void turn_off_HAWKs(void){
	HAL_GPIO_WritePin(HAWK1_GPIO_Port, HAWK1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HAWK2_GPIO_Port, HAWK2_Pin, GPIO_PIN_RESET);
}

void fire_TDs(void){
	HAL_GPIO_WritePin(TD1_GPIO_Port, TD1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TD2_GPIO_Port, TD2_Pin, GPIO_PIN_SET);
}

void turn_off_TDs(void){
	HAL_GPIO_WritePin(TD1_GPIO_Port, TD1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TD2_GPIO_Port, TD2_Pin, GPIO_PIN_RESET);
}

void init_ADC(void){
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Start_DMA(&hadc1, adc_buf, 8);
}

void read_ADC(float * out){

	float vsense = 3.3 / 4096.;
	float scale = 16 / 3.3;

	out[0] = (float)adc_buf[0] * vsense * scale;
	out[1] = (float)adc_buf[1] * vsense * scale;
	out[2] = (float)adc_buf[2] * vsense * scale;

	out[3] = -( 1000 * (( 3.3 / 2.0 - (float)adc_buf[3] * vsense) ) / 0.066 );
	out[4] = -( 1000 * (( 3.3 / 2.0 - (float)adc_buf[4] * vsense) ) / 0.066 );

	out[5] = (float)adc_buf[5] * vsense * scale;
	out[6] = (float)adc_buf[6] * vsense * scale;

	out[7] = ((((float)adc_buf[7] * vsense) - 0.76) / 0.0025) + 25.0;

}
