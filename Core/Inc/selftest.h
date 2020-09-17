/*
 * selftest.h
 *
 *  Created on: Sep 10, 2020
 *      Author: linus
 */

#ifndef INC_SELFTEST_H_
#define INC_SELFTEST_H_

#include "main.h"
#include "devices/MS5607.h"
#include "devices/ICM20601.h"
#include "devices/SHT31.h"
#include "devices/H3L.h"

#define MAX_SETUP_SAMPLE 100
#define MAX_SETUP_SAMPLE_INTERVAL 10

extern uint8_t config_baro(struct sht31_dev * t_dev, struct ms5607_dev * p1_dev, struct ms5607_dev * p2_dev, float * t, float * p);
extern uint8_t config_imu(struct icm20601_dev * a1_dev, struct icm20601_dev * a2_dev, int8_t * imu_sign, int8_t * acc_sign);
extern uint8_t selftest(void);


#endif /* INC_SELFTEST_H_ */
