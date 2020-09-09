/*
 * ADS1235.c
 *
 *  Created on: 29 Aug 2020
 *      Author: linus
 */


#include <devices/ADS1235.h>



HAL_StatusTypeDef _ret;

// Read bytes from MEMS
void _ads_read_bytes(struct ads1235_dev * dev, uint8_t reg, uint8_t* pData, uint16_t size){
	reg = reg | 0x80;
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->spi_bus, &reg, 1, ADS1235_SPI_TIMEOUT);
	HAL_SPI_Receive(dev->spi_bus, pData, size, ADS1235_SPI_TIMEOUT);
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

// Write bytes to MEMS
void _ads_write_bytes(struct ads1235_dev * dev, uint8_t reg, uint8_t *pData, uint16_t size){

	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(dev->spi_bus, &reg, 1, ADS1235_SPI_TIMEOUT);
	HAL_SPI_Transmit(dev->spi_bus, pData, size, ADS1235_SPI_TIMEOUT);
	HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

uint8_t ads1235_is_active(struct ads1235_dev * dev, uint32_t interval){
	uint32_t tick;
	tick = HAL_GetTick();
	if (tick > dev->last_call + interval){
		dev->last_call = tick;
		return 1;
	}
	return 0;
}

uint8_t ads1235_init(struct ads1235_dev * dev)
{
	uint8_t pData;
	uint8_t reg = 0x00;
	uint8_t devID;
	_ads_read_bytes(dev, reg, &pData, 1);

	devID = pData & 0b11110000;
	printf("device ID: %d\n", pData & 0b11110000);
	printf("device ID: %d\n", devID);

	if ( devID != 1100 )
	{
		printf("STRAIN setup fail\n");
		printf("Errorcode: %d\n", _ret);
		return 0;
	}

	pData = 0b01000100;
	//0
	//1000
	//100
	//400 SPS, FIR filter

	_ads_write_bytes(dev, 0x02, &pData, 1);



	printf("STRAIN setup success\n");
	return 1;
}

void ads1235_read_raw(struct ads1235_dev * dev, uint8_t * dat)
{
	uint8_t pData;
	uint8_t buf[3];

	// READ MUX 0

	// REFP0, REFN0
	pData = 0b00001010;

	_ads_write_bytes(dev, 0x06, &pData, 1);

	// AIN4, AIN5
	pData = 0b01111000;

	_ads_write_bytes(dev, 0x11, &pData, 1);


	_ads_read_bytes(dev, 0x12, buf, 3);

	dat[0] = buf[0];
	dat[1] = buf[1];
	dat[2] = buf[2];


	// READ MUX 1

	// REFP1, REFN1
	pData = 0b00001111;

	_ads_write_bytes(dev, 0x06, &pData, 1);

	// AIN2, AIN3
	pData = 0b01010110;

	_ads_write_bytes(dev, 0x11, &pData, 1);

	_ads_read_bytes(dev, 0x12, buf, 3);

	dat[3] = buf[0];
	dat[4] = buf[1];
	dat[5] = buf[2];

}

void ads1235_convert(struct ads1235_dev * dev, float * t)
{


}


