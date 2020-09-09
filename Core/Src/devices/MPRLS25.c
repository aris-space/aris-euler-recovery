/*
 * MPRLS25.c
 *
 *  Created on: 29 Aug 2020
 *      Author: linus
 */


/*
 *
 * Usage:
 *
 *
 */


#include <devices/MPRLS25.h>



HAL_StatusTypeDef _ret;


// Read bytes from MEMS
void _mprls25_read_bytes(struct mprls25_dev * dev, uint8_t reg, uint8_t* pData, uint16_t size){
	reg = reg | 0x80;
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->spi_bus, &reg, 1, MPRLS25_SPI_TIMEOUT);
	HAL_SPI_Receive(dev->spi_bus, pData, size, MPRLS25_SPI_TIMEOUT);
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

// Write bytes to MEMS
void _mprls25_write_bytes(struct mprls25_dev * dev, uint8_t reg, uint8_t *pData, uint16_t size){

	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->spi_bus, &reg, 1, MPRLS25_SPI_TIMEOUT);
	HAL_SPI_Transmit(dev->spi_bus, pData, size, MPRLS25_SPI_TIMEOUT);
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

uint8_t mprls25_is_active(struct mprls25_dev * dev, uint32_t interval){
	uint32_t tick;
	tick = HAL_GetTick();
	if (tick > dev->last_call + interval){
		dev->last_call = tick;
		return 1;
	}
	return 0;
}

uint8_t mprls25_init(struct mprls25_dev * dev)
{


	uint8_t send[2];
	uint8_t reg = 0xAA;
	send[0] = 0x00;
	send[1] = 0x00;

	_mprls25_write_bytes(dev, reg, send, 2);
	//printf("PITOT setup success\n");
	return 1;
}


void mprls25_read_pressure(struct mprls25_dev * dev, uint8_t * dat)
{
	uint8_t send[2];
	uint8_t rec[4];
	uint8_t reg = 0xAA;
	send[0] = 0x00;
	send[1] = 0x00;
	_mprls25_write_bytes(dev, reg, send, 2);

	reg = 0xF0;
	_mprls25_read_bytes(dev, reg, rec, 4);

	while (rec[0] & 0x20){
		printf("NO read mprls status: %d \n", rec[0]);
		_mprls25_read_bytes(dev, reg, rec, 4);
	}

	printf("OK read mprls status: %d \n", rec[0]);
	dat[0] = rec[1];
	dat[1] = rec[2];
	dat[2] = rec[3];
	dev->buf[0] = rec[1];
	dev->buf[1] = rec[2];
	dev->buf[2] = rec[3];
}

void mprls25_convert(struct mprls25_dev * dev, float * p)
{
	uint32_t out;
	out = (uint32_t)(dev->buf[0] << 16) | (uint32_t)(dev->buf[1] << 8) | (uint32_t)dev->buf[2];
	printf("out: %ld \n",out);
	*p = (out - MPRLS_OUT_MIN) * (MPRLS_P_MAX - MPRLS_P_MIN) / (MPRLS_OUT_MAX - MPRLS_OUT_MIN) + MPRLS_P_MIN;
}


