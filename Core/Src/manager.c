/*
 * manager.c
 *
 *  Created on: 14 Sep 2020
 *      Author: linus
 */

#include "manager.h"


task_t BARO_TASK = BARO_TASK_INIT();
task_t SHT_TASK = SHT_TASK_INIT();
task_t IMU_TASK = IMU_TASK_INIT();

task_t RDY_TASK = RDY_TASK_INIT();
task_t STAT_TASK = STAT_TASK_INIT();
task_t SAVE_TASK = SAVE_TASK_INIT();
task_t PRGM_TASK = PRGM_TASK_INIT();

LED STAT = STAT_INIT();
LED SAVE = SAVE_INIT();
LED PRGM = PRGM_INIT();
LED RDY = RDY_INIT();

MS5607 BARO1 = BARO1_INIT();
MS5607 BARO2 = BARO2_INIT();

ICM20601 IMU1 = IMU1_INIT();
ICM20601 IMU2 = IMU2_INIT();

SHT31 TEMP = SHT_INIT();

uint32_t tick;

uint8_t raw_data1[3];
uint8_t raw_data2[3];

float p1 = 0;
float p2 = 0;
float t_p1 = 0;
float t_p2 = 0;

int16_t accel1_raw_buf[6];
int16_t accel2_raw_buf[6];

float accel1_val[6];
float accel2_val[6];

uint16_t t_buf[2];
float t_val[2];


void schedulerinit () {
	ms5607_init(&BARO1);
	ms5607_init(&BARO2);
	sht31_init(&TEMP);
	icm20601_init(&IMU1);
	icm20601_init(&IMU2);

	turn_on(&STAT);
	HAL_Delay(300);
	turn_on(&SAVE);
	HAL_Delay(300);
	turn_on(&PRGM);
	HAL_Delay(300);

	turn_off(&STAT);
	turn_off(&SAVE);
	turn_off(&PRGM);
	HAL_Delay(1000);
}

void scheduler (){

	tick = HAL_GetTick();

	// TASK LED
	if(tick >= getNextExecution(&RDY_TASK)){
		RDY_TASK.last_call = HAL_GetTick();
		toggle(&RDY);
	}
	if(tick >= getNextExecution(&SAVE_TASK)){
		SAVE_TASK.last_call = HAL_GetTick();
		toggle(&SAVE);
	}
	if(tick >= getNextExecution(&STAT_TASK)){
		STAT_TASK.last_call = HAL_GetTick();
		toggle(&STAT);
	}
	if(tick >= getNextExecution(&PRGM_TASK)){
		PRGM_TASK.last_call = HAL_GetTick();
		toggle(&PRGM);
	}


	// TASK SHT
	if(tick >= getNextExecution(&SHT_TASK)){
		SHT_TASK.last_call = HAL_GetTick();
		sht31_read(&TEMP, t_val, t_buf);
	}

	// TASK BARO
	if(tick >= getNextExecution(&BARO_TASK)){

		switch(BARO_TASK.stage){
			case MS_TEMPERATURE_REQ:
				ms5607_prep_pressure(&BARO1, raw_data1);
				ms5607_prep_pressure(&BARO2, raw_data2);
				BARO_TASK.last_call = HAL_GetTick();
				BARO_TASK.stage = MS_PRESSURE_REQ;
				break;
			case MS_PRESSURE_REQ:
				ms5607_read_pressure(&BARO1, raw_data1);
				ms5607_read_pressure(&BARO2, raw_data2);
				BARO_TASK.last_call = HAL_GetTick();
				ms5607_convert(&BARO1, &p1, &t_p1);
				ms5607_convert(&BARO2, &p2, &t_p2);
				BARO_TASK.stage = MS_TEMPERATURE_REQ;
				break;
		}
	}

	// TASK IMU

	if(tick >= getNextExecution(&IMU_TASK)){
		IMU_TASK.last_call = HAL_GetTick();
		//icm20601_read_data_raw(&IMU1, accel1_raw_buf);
		//icm20601_convert_data(&IMU1, accel1_val, accel1_raw_buf);
		icm20601_read_data(&IMU1, accel1_val);

		//icm20601_read_data_raw(&IMU2, accel2_raw_buf);
		//icm20601_convert_data(&IMU2, accel2_val, accel2_raw_buf);
		icm20601_read_data(&IMU2, accel2_val);
	}

	// TASK SHOCK ACCEL
	// .........

	// TASK ADC
	// .........


	// TASK LOGGING
	// .........



	printf("tick: %ld \n",tick);
	printf("p1 = %4.2f bar and t1 = %4.2f C \n",p1,t_p1);
	printf("p2 = %4.2f bar and t2 = %4.2f C \n",p2,t_p2);
	printf("T = %4.2f C and H = %4.2f perc \n",t_val[0],t_val[1]);
	printf("IMU1 T: %4.2f C \n", accel1_val[0]);
	printf("IMU1 ax: %4.2f m/s2 \n", accel1_val[1]);
	printf("IMU1 ay: %4.2f m/s2 \n", accel1_val[2]);
	printf("IMU1 az: %4.2f m/s2 \n", accel1_val[3]);
	printf("IMU2 T: %4.2f C \n", accel2_val[0]);
	printf("IMU2 ax: %4.2f m/s2 \n", accel2_val[1]);
	printf("IMU2 ay: %4.2f m/s2 \n", accel2_val[2]);
	printf("IMU2 az: %4.2f m/s2 \n", accel2_val[3]);

}

uint32_t getNextExecution(task_t * task){
	return task->last_call + task->interval;
}
