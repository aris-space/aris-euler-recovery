/*
 * SHT31.h
 *
 *  Created on: 17 Jun 2020
 *      Author: linus
 */

#ifndef SHT31_H_
#define SHT31_H_


#define SHT_SAMPLING 1000

#define SHT_INIT() \
  { \
    .addr = 0x45 << 1, \
	.i2c_bus = &hi2c1, \
	.delay = 100, \
	.last_call = 0, \
  }

typedef struct sht31_dev {

	// Hardware Configuration
	uint8_t addr;
	I2C_HandleTypeDef* i2c_bus;
	uint8_t delay;
	uint32_t last_call;
} SHT31;

uint8_t sht31_is_busy(struct sht31_dev * dev);

int sht31_init(struct sht31_dev * dev);
void sht31_read(struct sht31_dev * dev, float * dat, uint16_t * dat_raw);

void write(struct sht31_dev * dev, uint8_t REG, uint8_t val);

#endif /* SHT31_H_ */
