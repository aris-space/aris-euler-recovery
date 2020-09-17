/*
 * buzzer.c
 *
 *  Created on: 17 Sep 2020
 *      Author: linus
 */


#include "buzzer.h"
#include "main.h"
#include "dwt_stm32_delay.h"


float C = 523.25;
float Cis = 554.37;
float D = 587.33;
float Dis = 622.25;
float E = 659.25;
float F = 698.46;
float Fis = 739.99;
float G = 783.99;
float Gis = 830.61;
float A = 880.0;
float Ais = 932.33;
float B = 987.77;


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

void take_on_me(void)
{
	play(2 * Fis,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * Fis,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * D,eighth);
	play(B,eighth);
	HAL_Delay(eighth);
	play(B,eighth);
	HAL_Delay(eighth);
	play(2 * E,eighth);
	HAL_Delay(eighth);
	play(2 * E,eighth);
	HAL_Delay(eighth);
	play(2 * E,eighth);
	play(2 * Gis,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * Gis,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * A,eighth);
	play(4 * Cis,eighth);
	play(2 * A,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * A,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * A,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * E,eighth);
	HAL_Delay(eighth);
	play(2 * D,eighth);
	HAL_Delay(eighth);
	play(2 * Fis,eighth);
	HAL_Delay(eighth);
	play(2 * Fis,eighth);
	HAL_Delay(eighth);
	play(2 * Fis,eighth);
	HAL_Delay(eighth);
	play(2 * Fis,eighth);
	play(2 * E,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * E,sixteenth);
	HAL_Delay(sixteenth);
	play(2 * Fis,eighth);
	play(2 * E,eighth);

}

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
	play(n*2*Ais,eighth);
	play(n*2*A,eighth);
	play(n*2*Ais,eighth);
	play(n*2*A,eighth);
	play(n*2*Ais,eighth);
	play(n*G,eighth);
	HAL_Delay(eighth);
	play(n*G,eighth);
	play(n*2*Ais,eighth);
	play(n*2*A,eighth);
	play(n*2*Ais,eighth);
	play(n*2*A,eighth);
	play(n*2*Ais,eighth);
}

