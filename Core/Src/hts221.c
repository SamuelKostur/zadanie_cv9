/*
 * hts221.c
 *
 *  Created on: Dec 4, 2020
 *      Author: Frixys
 */

#include "hts221.h"

uint8_t hts221_addres = HTS221_DEVICE_ADDRESS_0;

uint8_t hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, hts221_addres, 0));
}


void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, hts221_addres, 0);
}


void hts221_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, hts221_addres, 1);
}


float hts221_get_temp()
{
	uint8_t temp[2];
	hts221_readArray(temp, HTS221_ADDRESS_TEMP_L, 2);

	return (float)((temp[1] << 8) | temp[0]) / 8.0f;
}

float hts221_get_humidity()
{
	uint8_t hum[2];
	hts221_readArray(hum, HTS221_ADDRESS_TEMP_L, 2);

	return (float)((hum[1] << 8) | hum[0]) / 2.0f;
}



uint8_t hts221_init(void)
{
	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I_ADDRES);

	if(val == HTS221_WHO_AM_I_VALUE){
		status = 1;
	}
	else{
		status = 0;
		return status;
	}

	//hts221 device init
	uint8_t ctrl1 = 0b10000011;
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}

