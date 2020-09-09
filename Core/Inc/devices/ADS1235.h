/*
 * ADS1235.h
 *
 *  Created on: 17 Jun 2020
 *      Author: linus
 */

#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef ADS1235_H_
#define ADS1235_H_

#define STRAIN_INIT() \
  { \
    .cs_port = SPI2_CS_STRAIN1_GPIO_Port, \
    .cs_pin = SPI2_CS_STRAIN1_Pin, \
    .spi_bus = &hspi2, \
	.delay = 100, \
  }


#define ADS1235_SPI_TIMEOUT 3000


// *** structs *** //

typedef struct ads1235_dev {

	// Hardware Configuration
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	SPI_HandleTypeDef* spi_bus;
	uint8_t delay;
	uint32_t last_call;
} ADS1235;

extern uint8_t ads1235_is_active(struct ads1235_dev * dev, uint32_t interval);
extern uint8_t ads1235_init(struct ads1235_dev * dev);
extern void ads1235_read_raw(struct ads1235_dev * dev, uint8_t * dat);
extern void ads1235_convert(struct ads1235_dev * dev, float * t);

extern SPI_HandleTypeDef hspi2;


#endif /* ADS1235_H_ */
