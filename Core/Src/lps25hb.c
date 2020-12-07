/*
 * lps25hb.c
 *
 *  Created on: Dec 4, 2020
 *      Author: Frixys
 */

#include "lps25hb.h"

uint8_t lps25hb_addres = LPS25HB_DEVICE_ADDRESS_0;

uint8_t lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, lps25hb_addres, 0));
}


void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, lps25hb_addres, 0);
}


void lps25hb_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, lps25hb_addres, 1);
}


float lps25hb_get_bar()
{
	uint8_t temp[3];
	lps25hb_readArray(temp, LPS25HB_ADDRESS_BAR, 3);

	return ((uint32_t)((temp[2] << 16) | (uint16_t)((temp[1] << 8) | temp[0]))) / 4096.0f;
}




uint8_t lps25hb_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(val == LPS25HB_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else			//if the device is not found on one address, try another one
		{
			lps25hb_addres = LPS25HB_DEVICE_ADDRESS_1;
			val = lps25hb_read_byte(LPS25HB_WHO_AM_I_ADDRES);
			if(val == LPS25HB_WHO_AM_I_VALUE)
			{
				status = 1;
			}
			else
			{
				status = 0;
				//return status;
			}
		}


	//bar device init

	uint8_t ctrl1 = 0b11000000;

	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;
}


