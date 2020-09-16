/*
 * H3L.h
 *
 *  Created on: 17 Jun 2020
 *      Author: linus
 */

#ifndef H3L_H_
#define H3L_H_


#define ACCEL_INIT() \
  { \
    .addr = 0x18 << 1, \
	.i2c_bus = &hi2c1, \
	.delay = 100, \
  }

// *** structs *** //

typedef struct h3l_dev {

	// Hardware Configuration
	uint8_t addr;
	I2C_HandleTypeDef* i2c_bus;
	uint8_t delay;
	int16_t dat[3];
} H3L;

uint8_t h3l_init(struct h3l_dev * dev);
// void h3l_read(h3l_dev * dev, float* buffer);
void h3l_read_raw(struct h3l_dev * dev, int16_t * dat);
void h3l_convert(struct h3l_dev * dev, float * out);
void h3l_write(struct h3l_dev * dev, uint8_t reg, uint8_t val);



#endif /* H3L_H_ */
