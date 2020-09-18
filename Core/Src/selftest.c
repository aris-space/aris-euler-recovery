/*
 * selftest.c
 *
 *  Created on: Sep 10, 2020
 *      Author: linus
 */
#include "selftest.h"
#include "buzzer.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

uint8_t p_sanity_check(float * p){
	if ((*p < 110000) | (*p > 80000)) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t p_descent_sanity_check(float * p){
	if ((*p < 110000) | (*p > 20000)) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t t_sanity_check(float * t){
	if ((*t < 80) | (*t > 1)) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t a_sanity_check(float * a){
	if ((*a < 10) | (*a > 8)) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t state_est_sanity_check(float * h, float * a, float * v){
	if (a_sanity_check(a) == 0){
		if (DEBUG_PRINT == 1) printf("state est accel out of bounds. a = %4.2f \n",*a);
		return 0;
	}

	if ((*h > 50) | (*h < -50)) {
		if (DEBUG_PRINT == 1) printf("state est altitude out of bounds. h = %4.2f \n",*h);
		return 0;
	}

	if ((*v > 10) | (*v < -10)) {
		if (DEBUG_PRINT == 1) printf("state est velocity out of bounds. v = %4.2f \n",*v);
		return 0;
	}

	return 1;
}

uint8_t config_baro(struct sht31_dev * t_dev, struct ms5607_dev * p1_dev, struct ms5607_dev * p2_dev, float * t, float * p){

	float t1_sum = 0;
	float p1_sum = 0;
	float t2_sum = 0;
	float p2_sum = 0;
	uint16_t buf[2];
	uint8_t pbuf[3];
	float sht_val[2];
	float p1;
	float p2;
	float t1;
	float t2;
	// sample pressure/temperture 100 times
	for (int i = 0; i < MAX_SETUP_SAMPLE + 10; i++)
	{
		ms5607_prep_pressure(p1_dev, pbuf);
		ms5607_prep_pressure(p2_dev, pbuf);
		HAL_Delay(5);
		ms5607_read_pressure(p1_dev, pbuf);
		ms5607_read_pressure(p2_dev, pbuf);
		ms5607_convert(p1_dev, &p1, &t1);
		ms5607_convert(p2_dev, &p2, &t2);
		if (i > 10) {
			// ignore the first 10 values to let the barometer "warm" up
			t1_sum += t1;
			p1_sum += p1;
			t2_sum += t2;
			p2_sum += p2;
		}
		HAL_Delay(MAX_SETUP_SAMPLE_INTERVAL + 5);
	}
	p1_sum /= MAX_SETUP_SAMPLE;
	t1_sum /= MAX_SETUP_SAMPLE;
	p2_sum /= MAX_SETUP_SAMPLE;
	t1_sum /= MAX_SETUP_SAMPLE;

	if (!t_sanity_check(&t1_sum)){
		if (DEBUG_PRINT == 1) printf("Temperature of BARO 1 out of bounds. abort. \n");
		return 0;
	}
	if (!t_sanity_check(&t2_sum)){
		if (DEBUG_PRINT == 1) printf("Temperature of BARO 2 out of bounds. abort. \n");
		return 0;
	}
	if (!p_sanity_check(&p1_sum)){
		if (DEBUG_PRINT == 1) printf("Pressure of BARO 1 out of bounds. abort. \n");
		return 0;
	}
	if (!p_sanity_check(&p2_sum)){
		if (DEBUG_PRINT == 1) printf("Pressure of BARO 2 out of bounds. abort. \n");
		return 0;
	}

	// set as environement parameters
	*t = (t1_sum + t2_sum) / 2;
	*p = (p1_sum + p2_sum) / 2;


	// sample SHT temperature 100 times

	for (int i = 0; i < MAX_SETUP_SAMPLE + 10; i++)
	{
		sht31_read(t_dev, sht_val, buf);
		// ignore the first 10 measurements to let the SHT "warm" up
		if (i > 10) t1_sum += sht_val[1];
		HAL_Delay(MAX_SETUP_SAMPLE_INTERVAL);
	}
	t1_sum /= MAX_SETUP_SAMPLE;

	if (!t_sanity_check(&t1_sum)){
		if (DEBUG_PRINT == 1) printf("Temperature of SHT out of bounds. continuing anyway. \n");
		// if SHT unavailable, take BARO temperature
	} else {
		// if SHT is available, use SHT temperature value for environement
		*t = t1_sum;
	}

	return 1;
}

uint8_t config_imu(struct icm20601_dev * a1_dev, struct icm20601_dev * a2_dev){

	float a1_temp[7];
	float a2_temp[7];

	float a1_sum[3];
	float a2_sum[3];

	for (int i = 0; i < MAX_SETUP_SAMPLE + 10; i++)
	{
		icm20601_read_data(a1_dev, a1_temp);
		icm20601_read_data(a2_dev, a2_temp);
		if (i > 10) {
			// ignore the first 10 measurements to let the accelerometer "warm" up
			for (int j = 1; j < 4; j++){
				a1_sum[j] += abs(a1_temp[j]);
				a2_sum[j] += abs(a2_temp[j]);
			}
		}
		HAL_Delay(MAX_SETUP_SAMPLE_INTERVAL);
	}

	// assume rocket is in upright position, config axes accordingly

	uint8_t imu1_state = 0;
	uint8_t imu2_state = 0;
	for (int j = 0; j < 3; j++){
		a1_sum[j] /= MAX_SETUP_SAMPLE;
		a2_sum[j] /= MAX_SETUP_SAMPLE;
		if (a_sanity_check(&a1_sum[j]) == 1){
			imu1_state ++;
		}
		if (a_sanity_check(&a2_sum[j]) == 1){
			imu2_state ++;
		}
	}

	if (imu1_state + imu2_state != 2){
		if (DEBUG_PRINT == 1) printf("IMU axes out of bounds. abort. \n");
		if (DEBUG_PRINT == 1) printf("IMU1: %d \n", imu1_state);
		if (DEBUG_PRINT == 1) printf("IMU2: %d \n", imu2_state);
		return 0;
	}
	return 1;
}

uint8_t selftest(void){

	//check battery power,
	//check TD voltage
	//check LDR sensor

	return 1;
}
