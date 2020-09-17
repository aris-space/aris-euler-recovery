/*
 * buzzer.c
 *
 *  Created on: 17 Sep 2020
 *      Author: linus
 */


#include "buzzer.h"
#include "main.h"
#include "dwt_stm32_delay.h"


float B = 1864.66;
float As = 1661.22;
float Es = 1244.51;
float G = 392;
float Ais = 466.16;
float A = 440;
float C = 523;
float D = 587.0;
float Ais2 = 932.3;
float A2 = 880;

float CIS = 1109.0;
float H = 988.0;
float F = 740.0;

float sixteenth = 128.0;
float eighth = 256.0;
float fourth = 512.0;


void play(float freq, float time)
{
	float value = 0.0;
	while(value < time){
		HAL_GPIO_TogglePin(BUZ_GPIO_Port, BUZ_Pin);
		DWT_Delay_us (1000*1000/freq);
		value += 1000/freq;
	}
	HAL_GPIO_WritePin(BUZ_GPIO_Port, BUZ_Pin, GPIO_PIN_RESET);
};

void coffin_dance(int n)
{
	play(n*G,eighth);
	HAL_Delay(eighth);
	play(n*G,eighth);
	play(n*D,eighth);
	play(n*C,eighth);
	HAL_Delay(eighth);
	play(n*Ais,eighth);
	HAL_Delay(eighth);
	play(n*A,eighth);
	HAL_Delay(eighth);
	play(n*A,eighth);
	play(n*A,eighth);
	play(n*C,eighth);
	HAL_Delay(eighth);
	play(n*Ais,eighth);
	play(n*A,eighth);
	play(n*G,eighth);
	HAL_Delay(eighth);
	play(n*G,eighth);
	play(n*Ais2,eighth);
	play(n*A2,eighth);
	play(n*Ais2,eighth);
	play(n*A2,eighth);
	play(n*Ais2,eighth);
	play(n*G,eighth);
	HAL_Delay(eighth);
	play(n*G,eighth);
	play(n*Ais2,eighth);
	play(n*A2,eighth);
	play(n*Ais2,eighth);
	play(n*A2,eighth);
	play(n*Ais2,eighth);
}

