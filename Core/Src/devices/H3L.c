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



HAL_StatusTypeDef _ret;
uint8_t _DELAY_HL = 100;
uint8_t _ADDR_HL = 0x18 << 1;

//almost copy of SHT31 drivers
int H3L_INIT()
{
	if (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
	{
		printf("i2c1 not ready!\n");
	} else {
		printf("i2c1 is ready!\n");
	}
	_ret = HAL_I2C_IsDeviceReady(&hi2c1, _ADDR_HL, 10, _DELAY_HL);
	if ( _ret != HAL_OK )
	{
		printf("H3L setup fail\n");
		printf("Errorcode: %d\n", _ret);
		return 0;
	}

	//power up
	uint8_t PWR_CONF = 0b00111111;
	PWR_CONF = 0x27;

	H3L_write(0x20,PWR_CONF);


	//printf("H3L setup success\n");
	//printf("H3L address: %d\n",_ADDR_HL);

	unsigned char buf[6];

	buf[0] = (uint8_t)0x0F;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, buf, 1, _DELAY_HL);
	//HAL_Delay(5);
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;

	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, buf, 1, _DELAY_HL);
	//printf("WHOAMI: %d",buf[0]);
	return 1;
}


void H3L_read_raw(uint8_t* buf)
{

	unsigned char _buf[2];

	_buf[0] = (uint8_t)0x28;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[0] = _buf[0];

	_buf[0] = (uint8_t)0x29;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[1] = _buf[0];

	_buf[0] = (uint8_t)0x2a;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[2] = _buf[0];

	_buf[0] = (uint8_t)0x2b;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[3] = _buf[0];

	_buf[0] = (uint8_t)0x2c;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[4] = _buf[0];

	_buf[0] = (uint8_t)0x2d;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[5] = _buf[0];

	if ( _ret != HAL_OK )
	{
		printf("H3L read fail, error: %d \n",_ret);
	}

};

void H3L_convert(uint8_t* buf, double* out)
{

	int16_t x = (buf[0])| (int16_t)(buf[1] << 8);
	int16_t y = (buf[2])| (int16_t)(buf[3] << 8);
	int16_t z = (buf[4])| (int16_t)(buf[5] << 8);
	x = x >> 4;
	y = y >> 4;
	z = z >> 4;

	out[0] = (double)(x) * 49. / 1000. * 9.81;
	out[1] = (double)(y) * 49. / 1000. * 9.81;
	out[2] = (double)(z) * 49. / 1000. * 9.81;

	//printf("ax: %4.2f, ay: %4.2f, az: %4.2f\n",buffer[0],buffer[1],buffer[2]);

}



void H3L_read(double* buffer){


	unsigned char buf[6];
	unsigned char _buf[2];

	_buf[0] = (uint8_t)0x28;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[0] = _buf[0];

	_buf[0] = (uint8_t)0x29;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[1] = _buf[0];

	_buf[0] = (uint8_t)0x2a;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[2] = _buf[0];

	_buf[0] = (uint8_t)0x2b;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[3] = _buf[0];

	_buf[0] = (uint8_t)0x2c;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[4] = _buf[0];

	_buf[0] = (uint8_t)0x2d;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	_buf[0] = 0;
	_ret = HAL_I2C_Master_Receive(&hi2c1, _ADDR_HL, _buf, 1, _DELAY_HL);
	buf[5] = _buf[0];

	if ( _ret != HAL_OK )
	{
		printf("H3L read fail, error: %d \n",_ret);
	}


	int16_t x = (buf[0])| (int16_t)(buf[1] << 8);
	int16_t y = (buf[2])| (int16_t)(buf[3] << 8);
	int16_t z = (buf[4])| (int16_t)(buf[5] << 8);
	x = x >> 4;
	y = y >> 4;
	z = z >> 4;

	buffer[0] = (double)(x) * 49. / 1000. * 9.81;
	buffer[1] = (double)(y) * 49. / 1000. * 9.81;
	buffer[2] = (double)(z) * 49. / 1000. * 9.81;

	//printf("ax: %4.2f, ay: %4.2f, az: %4.2f\n",buffer[0],buffer[1],buffer[2]);

}

void H3L_write(uint8_t REG, uint8_t val)
{
	unsigned char _buf[2];
	//printf("writing to h3l: %d\n",val);
	_buf[0] = REG;
	_buf[1] = val;
	_ret = HAL_I2C_Master_Transmit(&hi2c1, _ADDR_HL, _buf, 2, _DELAY_HL);
	if ( _ret != HAL_OK )
	{
		printf("H3L write fail, error: %d \n",_ret);
	}
};

