/*
 * BARO1.h
 *
 *  Created on: 17 Jun 2020
 *      Author: linus
 */

#include "i2c.h"
#include "gpio.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef MS5607_H_
#define MS5607_H_

#define BARO1_INIT() \
  { \
    .addr = 0x76 << 1, \
	.i2c_bus = &hi2c1, \
	.delay = 100, \
	.last_stage = 0, \
	.last_call = 0, \
  }

#define BARO2_INIT() \
  { \
    .addr = 0x76 << 1, \
	.i2c_bus = &hi2c2, \
	.delay = 100, \
	.last_stage = 0, \
	.last_call = 0, \
  }

// *** structs *** //

typedef struct ms5607_dev {

	// Hardware Configuration
	uint8_t addr;
	I2C_HandleTypeDef* i2c_bus;
	uint8_t delay;
	uint16_t cal[6];
	uint32_t D1;
	uint32_t D2;
	uint32_t last_call;
	uint8_t last_stage;
} MS5607;

extern uint8_t ms5607_is_busy(struct ms5607_dev * dev, uint32_t interval, uint8_t stage);
extern uint8_t ms5607_init(struct ms5607_dev * dev);
extern void ms5607_prep_temp(struct ms5607_dev * dev);
extern void ms5607_prep_pressure(struct ms5607_dev * dev, uint8_t * dat);
extern void ms5607_read_pressure(struct ms5607_dev * dev, uint8_t * dat);
extern void ms5607_convert(struct ms5607_dev * dev, float * p, float * t);


#endif /* MS5607_H_ */
