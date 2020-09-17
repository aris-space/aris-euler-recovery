/*
 * selftest.c
 *
 *  Created on: Sep 10, 2020
 *      Author: linus
 */
#include "selftest.h"


int8_t IMU_X_SIGN = 1;
int8_t IMU_Y_SIGN = 1;
int8_t IMU_Z_SIGN = 1;
int8_t ACC_X_SIGN = 1;
int8_t ACC_Y_SIGN = 1;
int8_t ACC_Z_SIGN = 1;


void config_baro(void){

	// sample pressure/temperture 100 times
	// set as environement parameters3

}

void config_imu(void){

	// assume rocket is in upright position, config axes accordingly

	IMU_X_SIGN = 1;
	IMU_Y_SIGN = 1;
	IMU_Z_SIGN = 1;
	ACC_X_SIGN = 1;
	ACC_Y_SIGN = 1;
	ACC_Z_SIGN = 1;
}

uint8_t selftest(void){

	return 1;
}
