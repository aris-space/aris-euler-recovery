/*
 * manager.c
 *
 *  Created on: 14 Sep 2020
 *      Author: linus
 */

#include "manager.h"
#include "main.h"
#include "devices/LED.h"
#include "devices/MS5607.h"
#include "devices/ICM20601.h"
#include "devices/SHT31.h"
#include "devices/H3L.h"


#include "IO.h"
#include "SD.h"
#include "selftest.h"
#include "buzzer.h"
#include "dwt_stm32_delay.h"
#include "fs_timer.h"
#include "Sim_Con/state_est.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

task_t BARO_TASK = BARO_TASK_INIT();
task_t SHT_TASK = SHT_TASK_INIT();
task_t IMU_TASK = IMU_TASK_INIT();
task_t ACCEL_TASK = ACCEL_TASK_INIT();
task_t ADC_TASK = ADC_TASK_INIT();
task_t LOG_TASK = LOG_TASK_INIT();
task_t STATE_EST_TASK = STATE_EST_TASK_INIT();

task_t RDY_TASK = RDY_TASK_INIT();
task_t STAT_TASK = STAT_TASK_INIT();
task_t SAVE_TASK = SAVE_TASK_INIT();
task_t PRGM_TASK = PRGM_TASK_INIT();

TIMER mach_timer = MACH_INIT();
TIMER fail_safe_timer = FAIL_SAFE_INIT();
TIMER fail_safe_timer_main = FAIL_SAFE_MAIN_INIT();

LED STAT = STAT_INIT();
LED SAVE = SAVE_INIT();
LED PRGM = PRGM_INIT();
LED RDY = RDY_INIT();


MS5607 BARO1 = BARO1_INIT();
MS5607 BARO2 = BARO2_INIT();

ICM20601 IMU1 = IMU1_INIT();
ICM20601 IMU2 = IMU2_INIT();

SHT31 TEMP = SHT_INIT();

H3L ACCEL = ACCEL_INIT();


uint32_t tick;
uint32_t fake_tick;
uint32_t counter = 0;

uint8_t raw_data1[3];
uint8_t raw_data2[3];

float p1 = 0;
float p2 = 0;
float t_p1 = 0;
float t_p2 = 0;

int16_t accel1_raw_buf[6];
int16_t accel2_raw_buf[6];

int16_t accel_raw[3];

float accel[3];

float accel1_val[7];
float accel2_val[7];

uint16_t t_buf[2];
float t_val[2];

float adc_dat[8];


char buffer[BUFLEN]; // to store data

uint16_t num_dat_file = 0;
uint16_t num_log_file = 0;
uint8_t SD_state = 0;
char FILE_NAME[11];
char LOG_NAME[10];


uint8_t armed = 0;
uint8_t event = 0;
uint8_t flight_phase = 0;
float alt = 0;
float velocity = 0;
float t_cpu;
float I_BAT1;
float I_BAT2;
float V_BAT1;
float V_BAT2;
float V_LDR;
float V_TD1;
float V_TD2;

state_est_state_t state_est_state = { 0 };

uint8_t FAKE_DATA = 0;

uint8_t CHECK_FLAG = 0;

uint32_t TD_fired = 0;

float TIME[FAKE_FILE_LEN];
float P1[FAKE_FILE_LEN];
float P2[FAKE_FILE_LEN];
float Ax1[FAKE_FILE_LEN];
float Ay1[FAKE_FILE_LEN];
float Az1[FAKE_FILE_LEN];
float Ax2[FAKE_FILE_LEN];
float Ay2[FAKE_FILE_LEN];
float Az2[FAKE_FILE_LEN];

float launch_detect_buffer[5];

uint8_t launch_detect(float * a1, float * a2){
	for (int i = 1; i < 5; i++){
		launch_detect_buffer[i-1] = launch_detect_buffer[i];
	}
	launch_detect_buffer[4] = (sqrt(a1[1]*a1[1] + a1[2]*a1[2] + a1[3]*a1[3]) + sqrt(a2[1]*a2[1] + a2[2]*a2[2] + a2[3]*a2[3])) / 2;
	float sum_a = 0;
	for (int i = 0; i < 5; i++){
		sum_a += launch_detect_buffer[i];
	}
	sum_a /= 5;

	// if average of acceleration over 5 measurements is higher than 4G, launch has been detected.
	if (sum_a >= 40) return 1;
	return 0;
}

void schedulerinit () {


	//initialize all devices
	ms5607_init(&BARO1);
	ms5607_init(&BARO2);
	sht31_init(&TEMP);
	icm20601_init(&IMU1);
	icm20601_init(&IMU2);
	h3l_init(&ACCEL);
	init_ADC();

	DWT_Delay_Init();


	// cycle through LEDs

	turn_on(&STAT);
	HAL_Delay(300);
	turn_on(&SAVE);
	HAL_Delay(300);
	turn_on(&PRGM);
	HAL_Delay(300);
	turn_on(&RDY);
	HAL_Delay(300);

	turn_off(&STAT);
	turn_off(&SAVE);
	turn_off(&PRGM);
	turn_off(&RDY);

	// initialize SD card
	turn_on(&SAVE);
	SD_state = init_sd(&num_dat_file, &num_log_file);
	if (SD_state == 0){
		turn_off(&SAVE);
	} else {
		turn_on(&RDY);
		HAL_Delay(100);
		turn_off(&RDY);
		HAL_Delay(100);
	}
	if (DEBUG_PRINT == 1) printf("num_dat_file: %hu \n",num_dat_file);
	if (DEBUG_PRINT == 1) printf("num_log_file: %hu \n",num_log_file);

	num_log_file ++;
	num_dat_file ++;

	sprintf(FILE_NAME,"FL%04u.CSV", num_dat_file);
	if (DEBUG_PRINT == 1) printf("saving %s ...",FILE_NAME);

	sprintf(LOG_NAME,"LOG%02u.CSV", num_log_file);
	if (DEBUG_PRINT == 1) printf("saving %s ...",LOG_NAME);

	SD_state = init_file(FILE_NAME, LOG_NAME);

	if (SD_state == 0){
		turn_off(&SAVE);
	} else {
		turn_on(&RDY);
		HAL_Delay(100);
		turn_off(&RDY);
		HAL_Delay(100);
	}
	turn_on(&RDY);
	HAL_Delay(1000);

	bufclear(buffer);
	sprintf(buffer, "%ld, SCHEDULER INIT OK, - \n", HAL_GetTick());
	log_to_SD(LOG_NAME, buffer);

	//coffin_dance(1);
	take_on_me();
	take_on_me();

	if (FAKE_DATA == 1)
	{
		// read in fake data
		read_from_SD("FAKE.CSV", TIME, P1, P2, Ax1, Ay1, Az1, Ax2, Ay2, Az2);
	}

	// initialize state estimation with environment values

	float ground_pressure = 0;
	float ground_temperature = 0;

	config_baro(&TEMP, &BARO1, &BARO2, &ground_temperature, &ground_pressure);
	config_imu(&IMU1, &IMU2);

	reset_state_est_state(ground_pressure, ground_temperature, &state_est_state);
}

void scheduler (){

	tick = HAL_GetTick();

	// TASK LED
	if(tick >= getNextExecution(&RDY_TASK)){
		RDY_TASK.last_call = tick;
		toggle(&RDY);
	}
	if(tick >= getNextExecution(&SAVE_TASK)){
		SAVE_TASK.last_call = tick;
		toggle(&SAVE);
	}
	if(tick >= getNextExecution(&STAT_TASK)){
		STAT_TASK.last_call = tick;
		toggle(&STAT);
	}
	if(tick >= getNextExecution(&PRGM_TASK)){
		PRGM_TASK.last_call = tick;
		toggle(&PRGM);
	}


	// TASK SHT
	if(tick >= getNextExecution(&SHT_TASK)){
		SHT_TASK.last_call = tick;
		sht31_read(&TEMP, t_val, t_buf);
	}

	// TASK BARO
	if(tick >= getNextExecution(&BARO_TASK)){

		switch(BARO_TASK.stage){
			case MS_TEMPERATURE_REQ:
				ms5607_prep_pressure(&BARO1, raw_data1);
				ms5607_prep_pressure(&BARO2, raw_data2);
				BARO_TASK.last_call = tick;
				BARO_TASK.stage = MS_PRESSURE_REQ;
				break;
			case MS_PRESSURE_REQ:
				ms5607_read_pressure(&BARO1, raw_data1);
				ms5607_read_pressure(&BARO2, raw_data2);
				BARO_TASK.last_call = tick;
				ms5607_convert(&BARO1, &p1, &t_p1);
				ms5607_convert(&BARO2, &p2, &t_p2);
				BARO_TASK.stage = MS_TEMPERATURE_REQ;
				break;
		}
	}

	// TASK IMU

	if(tick >= getNextExecution(&IMU_TASK)){
		IMU_TASK.last_call = tick;
		//icm20601_read_data_raw(&IMU1, accel1_raw_buf);
		//icm20601_convert_data(&IMU1, accel1_val, accel1_raw_buf);
		icm20601_read_data(&IMU1, accel1_val);

		//icm20601_read_data_raw(&IMU2, accel2_raw_buf);
		//icm20601_convert_data(&IMU2, accel2_val, accel2_raw_buf);
		icm20601_read_data(&IMU2, accel2_val);
	}

	// TASK SHOCK ACCEL

	if(tick >= getNextExecution(&ACCEL_TASK)){
		ACCEL_TASK.last_call = tick;
		h3l_read_raw(&ACCEL, accel_raw);
		h3l_convert(&ACCEL, accel);
	}

	// TASK ADC
	if(tick >= getNextExecution(&ADC_TASK)){
		ADC_TASK.last_call = tick;
		read_ADC(adc_dat);
		if (DEBUG_PRINT == 1) printf("1 %4.2f V \n", adc_dat[0]);
		if (DEBUG_PRINT == 1) printf("2 %4.2f V \n", adc_dat[1]);
		if (DEBUG_PRINT == 1) printf("3 %4.2f V \n", adc_dat[2]);
		if (DEBUG_PRINT == 1) printf("4 %4.2f V \n", adc_dat[3]);
		if (DEBUG_PRINT == 1) printf("5 %4.2f V \n", adc_dat[4]);
		if (DEBUG_PRINT == 1) printf("6 %4.2f V \n", adc_dat[5]);
		if (DEBUG_PRINT == 1) printf("7 %4.2f V \n", adc_dat[6]);
		if (DEBUG_PRINT == 1) printf("8 %4.2f C \n", adc_dat[7]);
		V_TD1 = adc_dat[0];
		V_TD2 = adc_dat[1];
		V_LDR = adc_dat[2];
		I_BAT1 = adc_dat[3];
		I_BAT2 = adc_dat[4];
		V_BAT1 = adc_dat[5];
		V_BAT2 = adc_dat[6];
		t_cpu = adc_dat[7];
	}

	// TASK STATE ESTIMATION

	if(tick >= getNextExecution(&STATE_EST_TASK)){
		STATE_EST_TASK.last_call = tick;

		if (FAKE_DATA == 1){

			// if file does not exist, continue with nominal operation
			if (TIME[counter] == 0){
				FAKE_DATA = 0;
			} else {
				// use fake/old data from SD card to overwrite current sensor data
				fake_tick = TIME[counter];
				p1 = P1[counter];
				p2 = P2[counter];
				accel1_val[1] = Ax1[counter];
				accel1_val[2] = Ay1[counter];
				accel1_val[3] = Az1[counter];
				accel2_val[1] = Ax2[counter];
				accel2_val[2] = Ay2[counter];
				accel2_val[3] = Az2[counter];
				counter ++;
			}

			// if fake file ends, continue with nominal operation
			if (counter >= FAKE_FILE_LEN){
				FAKE_DATA = 0;
			}
		}

		// call state estimation

		// feed in sensor values
		state_est_state.state_est_meas.baro_data[0].pressure = p1;
		state_est_state.state_est_meas.baro_data[0].temperature = t_p1;
		state_est_state.state_est_meas.baro_data[0].ts = tick;

		state_est_state.state_est_meas.imu_data[0].acc_x = -accel1_val[1];
		state_est_state.state_est_meas.imu_data[0].ts = tick;

		state_est_state.state_est_meas.baro_data[1].pressure = p2;
		state_est_state.state_est_meas.baro_data[1].temperature = t_p2;
		state_est_state.state_est_meas.baro_data[1].ts = tick;

		state_est_state.state_est_meas.imu_data[1].acc_x = -accel2_val[1];
		state_est_state.state_est_meas.imu_data[1].ts = tick;

		state_est_step(tick, &state_est_state, true);


		// timer start
		if ((state_est_state.flight_phase_detection.flight_phase == THRUSTING) || (launch_detect(accel1_val, accel2_val) == 1) ){
			start_timer(&mach_timer, &tick);
			start_timer(&fail_safe_timer, &tick);
			start_timer(&fail_safe_timer_main, &tick);
		}

		if ((tick > 30000) && (CHECK_FLAG == 0)){

			// Perform sanity check of state estimation 30 seconds after bootup!
			// this is in steady state on the launchpad

			float check_a = -accel1_val[1];
			float check_h = state_est_state.state_est_data.position_world[2];
			float check_v = state_est_state.state_est_data.velocity_rocket[0];
			if (state_est_sanity_check(&check_a, &check_h, &check_v) == 0){
				if (DEBUG_PRINT == 1) printf("sanity check for state estimation failed! \n");
			}

			check_a = -accel2_val[1];
			if (state_est_sanity_check(&check_a, &check_h, &check_v) == 0){
				if (DEBUG_PRINT == 1) printf("sanity check for state estimation failed! \n");
			}
			CHECK_FLAG = 1;
		}

	}


	// if mach timer has passed, software arm the system
	if (check_timer(&mach_timer, &tick) == 1) armed = 1;

	// if fail_safe timer has passed, skip to descent flight phase
	if (check_timer(&fail_safe_timer, &tick) == 1) state_est_state.flight_phase_detection.flight_phase = DROGUE_DESCENT;

	// if fail_safe timer has passed, skip to descent flight phase
	if (check_timer(&fail_safe_timer_main, &tick) == 1) {
		if (state_est_state.flight_phase_detection.flight_phase < DROGUE_DESCENT){
			// if the main fail_safe_timer for some reason ends before we're in DESCENT mode
			state_est_state.flight_phase_detection.flight_phase = DROGUE_DESCENT;
		} else if (state_est_state.flight_phase_detection.flight_phase == DROGUE_DESCENT) {
			// after main fail safe timer ends, we jump into RECOVERY mode an initiate main deploy
			// this happens for example if the barometer values are invalid during descent
			state_est_state.flight_phase_detection.flight_phase = DROGUE_DESCENT;
		}
	}


	// act according to flight phase
	switch(state_est_state.flight_phase_detection.flight_phase){

		case IDLE:
			break;
		case AIRBRAKE_TEST:
			break;
		case THRUSTING:
			break;
		case COASTING:
			break;
		case DROGUE_DESCENT:
			// apogee
			fire_HAWKs(&armed);
			event = HAWKS;
			break;
		case BALLISTIC_DESCENT:
			// oh-oh...
			fire_HAWKs(&armed);
			event = HAWKS;
			state_est_state.flight_phase_detection.flight_phase = DROGUE_DESCENT;
			break;
		case MAIN_DESCENT:
			// second event
			if (TD_fired == 0){
				fire_TDs(&armed);
				TD_fired = tick;
				event = TENDER;
			}

			// allow 100ms of high-current through igniters
			// if after 100ms the current is still peaking over 1 Amp, the igniters have fused
			// this might damage the electronics and drain the battery
			if(tick >= TD_fired + 100){
				if ((I_BAT1 >= 1000) || (I_BAT2 >= 1000)){
					// turn off the pyro channels to save power and protect the circuit board
					if (DEBUG_PRINT == 1) printf("fused igniters detected!! \n");
					turn_off_TDs();
					event = TENDER_DISABLE;
				}
			}
			break;
		case TOUCHDOWN:
			play(440,100);
			play(659.25,100);
			play(880,200);
			HAL_Delay(600);
			break;
	}


	// TASK LOGGING
	if(tick >= getNextExecution(&LOG_TASK)){
		LOG_TASK.last_call = tick;
		sprintf(buffer,"%ld, %d ,%d, %d, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f, %4.2f\n",
				tick, armed, event, flight_phase, alt, velocity, t_val[1],t_val[0],t_cpu,t_p1,t_p2,accel1_val[0],accel2_val[0],p1,p2,accel1_val[1],accel1_val[2],accel1_val[3],accel1_val[4],accel1_val[5],accel1_val[6],accel2_val[1],accel2_val[2],accel2_val[3],accel2_val[4],accel2_val[5],accel2_val[6],accel[0],accel[1],accel[2],I_BAT1,I_BAT2,V_BAT1,V_BAT2,V_LDR,V_TD1,V_TD2);

		write_to_SD(FILE_NAME, buffer);
	}

	if (DEBUG_PRINT == 1) printf("tick: %ld \n",tick);
	if (DEBUG_PRINT == 1) printf("p1 = %4.2f bar and t1 = %4.2f C \n",p1,t_p1);
	if (DEBUG_PRINT == 1) printf("p2 = %4.2f bar and t2 = %4.2f C \n",p2,t_p2);
	if (DEBUG_PRINT == 1) printf("T = %4.2f C and H = %4.2f perc \n",t_val[0],t_val[1]);
	if (DEBUG_PRINT == 1) printf("IMU1 T: %4.2f C \n", accel1_val[0]);
	if (DEBUG_PRINT == 1) printf("IMU1 ax: %4.2f m/s2 \n", accel1_val[1]);
	if (DEBUG_PRINT == 1) printf("IMU1 ay: %4.2f m/s2 \n", accel1_val[2]);
	if (DEBUG_PRINT == 1) printf("IMU1 az: %4.2f m/s2 \n", accel1_val[3]);
	if (DEBUG_PRINT == 1) printf("IMU2 T: %4.2f C \n", accel2_val[0]);
	if (DEBUG_PRINT == 1) printf("IMU2 ax: %4.2f m/s2 \n", accel2_val[1]);
	if (DEBUG_PRINT == 1) printf("IMU2 ay: %4.2f m/s2 \n", accel2_val[2]);
	if (DEBUG_PRINT == 1) printf("IMU2 az: %4.2f m/s2 \n", accel2_val[3]);
	if (DEBUG_PRINT == 1) printf("ACC ax: %4.2f m/s2 \n", accel[0]);
	if (DEBUG_PRINT == 1) printf("ACC ay: %4.2f m/s2 \n", accel[1]);
	if (DEBUG_PRINT == 1) printf("ACC az: %4.2f m/s2 \n", accel[2]);

}

uint32_t getNextExecution(task_t * task){
	return task->last_call + task->interval;
}
