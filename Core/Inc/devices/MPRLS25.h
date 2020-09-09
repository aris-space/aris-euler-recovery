/*
 * MPRLS25.h
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

#ifndef MPRLS25_H_
#define MPRLS25_H_

#define PITOT1_INIT() \
  { \
    .cs_port = SPI2_CS_PITOT_GPIO_Port, \
    .cs_pin = SPI2_CS_PITOT_Pin, \
    .spi_bus = &hspi2, \
	.delay = 100, \
  }

#define MPRLS_P_MAX 2.5
#define MPRLS_P_MIN 0
#define MPRLS_OUT_MAX 15099494
#define MPRLS_OUT_MIN 1677722

#define MPRLS25_SPI_TIMEOUT 3000


// *** structs *** //

typedef struct mprls25_dev {

	// Hardware Configuration
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	SPI_HandleTypeDef* spi_bus;
	uint8_t delay;
	uint8_t buf[3];
	uint32_t last_call;
} MPRLS25;

extern uint8_t mprls25_is_active(struct mprls25_dev * dev, uint32_t interval);
extern uint8_t mprls25_init(struct mprls25_dev * dev);
extern void mprls25_read_pressure(struct mprls25_dev * dev, uint8_t * dat);
extern void mprls25_convert(struct mprls25_dev * dev, float * p);


#endif /* MPRLS25_H_ */
