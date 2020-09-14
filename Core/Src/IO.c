/*
 * task1.c
 *
 *  Created on: 25 Aug 2020
 *      Author: linus
 */

#include "IO.h"

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
