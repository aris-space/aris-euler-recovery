/*
 * SHT31.c
 *
 *  Created on: 17 Jun 2020
 *      Author: linus
 */


#include "main.h"
#include "devices/SHT31.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>



uint8_t sht31_is_busy(struct sht31_dev * dev){
	uint32_t tick;
	tick = HAL_GetTick();
	if (tick >= dev->last_call + SHT_SAMPLING){
		dev->last_call = tick;
		return 1;
	}
	return 0;
}


int sht31_init(struct sht31_dev * dev)
{
	if (HAL_I2C_GetState(dev->i2c_bus) != HAL_I2C_STATE_READY)
	{
		printf("i2c1 not ready!\n");
	} else {
		printf("i2c1 is ready!\n");
	}

	HAL_StatusTypeDef _ret;

	_ret = HAL_I2C_IsDeviceReady(dev->i2c_bus, dev->addr, 10, dev->delay);
	if ( _ret != HAL_OK )
	{
		printf("SHT31 setup fail\n");
		printf("Errorcode: %d\n", _ret);
		return 0;
	}

	printf("SHT31 setup success\n");
	//request first measurement
	uint8_t buf[2];
	uint16_t REG = 0x2416;
	buf[0] = (uint8_t)(REG >> 8);
	buf[1] = (uint8_t)REG;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, buf, 2, dev->delay);
	return 1;
}

void sht31_read(struct sht31_dev * dev, float * dat, uint16_t * dat_raw)
{

	float temp;
	float humid;
	uint8_t buf[6];

	//T MSB, T LSB, T CRC, H MSB, H LSB, H CRC
	HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, buf, 6, dev->delay);
	/*
	uint16_t rawTemp = (uint16_t)((((uint16_t)_buf[0])<<8) | (uint16_t)_buf[1]);
	uint16_t rawHum = (uint16_t)((((uint16_t)_buf[3])<<8) | (uint16_t)_buf[4]);
	*/
	uint16_t rawTemp = ((uint16_t)buf[0] << 8) | buf[1];
	uint16_t rawHum = ((uint16_t)buf[3] << 8) | buf[4];
	humid = 100.0 * (float)rawHum / 65535.0;
	temp = -45.0 + 175 * (float)rawTemp / 65535.0;
	//printf("temperature is %4.2f deg\n",temp);
	//printf("humidity is %4.2f perc\n",humid);
	dat[0] = temp;
	dat[1] = humid;
	dat_raw[0] = rawTemp;
	dat_raw[1] = rawHum;

	uint16_t REG = 0x2416;
	buf[0] = (uint8_t)(REG >> 8);
	buf[1] = (uint8_t)REG;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, buf, 2, dev->delay);

};
void write(struct sht31_dev * dev, uint8_t REG, uint8_t val)
{
	uint8_t buf[2];
	buf[0] = REG;
	buf[1] = val;

	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, buf, 2, dev->delay);

};

