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

extern uint8_t t_sanity_check(float * t);
extern uint8_t p_sanity_check(float * p);
extern uint8_t a_sanity_check(float * a);
extern uint8_t state_est_sanity_check(float * h, float * a, float * v);

extern uint8_t config_baro(struct sht31_dev * t_dev, struct ms5607_dev * p1_dev, struct ms5607_dev * p2_dev, float * t, float * p);
extern uint8_t config_imu(struct icm20601_dev * a1_dev, struct icm20601_dev * a2_dev);
extern uint8_t selftest(float TD1, float TD2, float BAT1, float BAT2, float LDR);

#endif /* INC_SELFTEST_H_ */
