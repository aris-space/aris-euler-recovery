/*
 * H3L.c
 *
 *  Created on: 17 Jun 2020
 *      Author: linus
 */



#include "main.h"
#include "devices/H3L.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>
#include <math.h>



uint8_t _DELAY_HL = 100;
uint8_t _ADDR_HL = 0x18 << 1;

//almost copy of SHT31 drivers
uint8_t h3l_init(struct h3l_dev * dev)
{
	HAL_StatusTypeDef _ret;
	if (HAL_I2C_GetState(dev->i2c_bus) != HAL_I2C_STATE_READY)
	{
		printf("i2c1 not ready!\n");
	} else {
		printf("i2c1 is ready!\n");
	}
	_ret = HAL_I2C_IsDeviceReady(dev->i2c_bus, dev->addr, 10, dev->delay);
	if ( _ret != HAL_OK )
	{
		printf("H3L setup fail\n");
		printf("Errorcode: %d\n", _ret);
		return 0;
	}

	//power up
	uint8_t PWR_CONF = 0b00111111;
	//PWR_CONF = 0x27;

	h3l_write(dev, 0x20, PWR_CONF);


	PWR_CONF = 0b10000000;
	h3l_write(dev, 0x23, PWR_CONF);


	//printf("H3L setup success\n");
	//printf("H3L address: %d\n",_ADDR_HL);

	/*
	uint8_t buf, reg;

	reg = (uint8_t)0x0F;
	_ret = HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, &reg, 1, dev->delay);
	//HAL_Delay(5);
	_ret = HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, &buf, 1, dev->delay);
	printf("WHOAMI: %d \n", buf);
	*/

	return 1;
}


void h3l_read_raw(struct h3l_dev * dev, int16_t * dat)
{

	uint8_t reg;
	uint8_t buf[6];

	reg = 0x28;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, &reg, 1, dev->delay);
	HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, &buf[0], 1, dev->delay);
	reg = 0x29;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, &reg, 1, dev->delay);
	HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, &buf[1], 1, dev->delay);
	reg = 0x2A;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, &reg, 1, dev->delay);
	HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, &buf[2], 1, dev->delay);
	reg = 0x2B;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, &reg, 1, dev->delay);
	HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, &buf[3], 1, dev->delay);
	reg = 0x2C;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, &reg, 1, dev->delay);
	HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, &buf[4], 1, dev->delay);
	reg = 0x2D;
	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, &reg, 1, dev->delay);
	HAL_I2C_Master_Receive(dev->i2c_bus, dev->addr, &buf[5], 1, dev->delay);

	dev->dat[0] = (buf[0]) | (int16_t)(buf[1] << 8);
	dev->dat[1] = (buf[2]) | (int16_t)(buf[3] << 8);
	dev->dat[2] = (buf[4]) | (int16_t)(buf[5] << 8);
	dev->dat[0] = dev->dat[0] >> 4;
	dev->dat[1] = dev->dat[1] >> 4;
	dev->dat[2] = dev->dat[2] >> 4;
	dat[0] = dev->dat[0];
	dat[1] = dev->dat[1];
	dat[2] = dev->dat[2];

};

void h3l_convert(struct h3l_dev * dev, float* out)
{

	out[0] = (float)(dev->dat[0]) * 49. / 1000. * 9.81;
	out[1] = (float)(dev->dat[1]) * 49. / 1000. * 9.81;
	out[2] = (float)(dev->dat[2]) * 49. / 1000. * 9.81;

	//printf("ax: %4.2f, ay: %4.2f, az: %4.2f\n",buffer[0],buffer[1],buffer[2]);

}


void h3l_write(struct h3l_dev * dev, uint8_t reg, uint8_t val)
{
	uint8_t _buf[2];
	//printf("writing to h3l: %d\n",val);
	_buf[0] = reg;
	_buf[1] = val;

	HAL_I2C_Master_Transmit(dev->i2c_bus, dev->addr, _buf, 2, dev->delay);

};

