/*
 * task1.h
 *
 *  Created on: 25 Aug 2020
 *      Author: linus
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"


#define STAT_INIT() \
  { \
    .LED_port = STAT_GPIO_Port, \
    .LED_pin = STAT_Pin, \
	.interval = 333, \
  }
#define RDY_INIT() \
  { \
    .LED_port = RDY_GPIO_Port, \
    .LED_pin = RDY_Pin, \
	.interval = 100, \
  }
#define SAVE_INIT() \
  { \
    .LED_port = SAVE_GPIO_Port, \
    .LED_pin = SAVE_Pin, \
	.interval = 500, \
  }
#define PRGM_INIT() \
  { \
    .LED_port = PRGM_GPIO_Port, \
    .LED_pin = PRGM_Pin, \
	.interval = 1000, \
  }



// *** structs *** //

typedef struct LED_dev {

	// Hardware Configuration
	GPIO_TypeDef *LED_port;
	uint16_t LED_pin;
	uint32_t last_call;
	uint32_t interval;
} LED;

extern uint8_t LED_is_busy(struct LED_dev * dev);
extern void toggle(struct LED_dev * ctrl);
extern void turn_on(struct LED_dev * ctrl);
extern void turn_off(struct LED_dev * ctrl);


#endif /* INC_LED_H_ */
