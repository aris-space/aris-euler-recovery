/*
 * task1.h
 *
 *  Created on: 25 Aug 2020
 *      Author: linus
 */

#ifndef INC_IO_H_
#define INC_IO_H_

#include "main.h"

uint32_t adc_buf[8];

extern void fire_HAWKs(uint8_t * armed);
extern void turn_off_HAWKs(void);
extern void fire_TDs(uint8_t * armed);
extern void turn_off_TDs(void);
extern void init_ADC(void);
extern void read_ADC(float * out);


#endif /* INC_IO_H_ */
