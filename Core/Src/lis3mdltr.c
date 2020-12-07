/*
 * lis3mdltr.c
 *
 *  Created on: 17. 7. 2019
 *      Author: Stancoj
 */


#include "lis3mdltr.h"

#define LIS2_DH_CTRL2_FILTER_SETTINGS		(LIS3MDL_CTRL2_HPM_NORMAL_REF | LIS3MDL_CTRL2_HPCF0)


static uint8_t sLIS3MDLI2Caddress = LIS3MDL_DEVICE_ADDRESS;

uint64_t lastMovementTick = 0;

uint8_t lis3mdl_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, sLIS3MDLI2Caddress, 0));
}

void lis3mdl_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, sLIS3MDLI2Caddress, 0);
}

void lis3mdl_readArray(uint8_t * data, uint8_t reg, uint8_t length)
{
	i2c_master_read(data, length, reg, sLIS3MDLI2Caddress, 1);
}

int8_t lis3mdl_get_temp()
{
	uint8_t temp[2];
	lis3mdl_readArray(temp, LIS3MDL_ADDRESS_TEMP_L, 2);

	return (((int16_t)((temp[1] << 8) | temp[0])) >> 3)  + 25;
}

void lis3mdl_get_mag(float* x, float* y, float* z)
{
	uint8_t data[6];
	int16_t xx, yy, zz;

	uint8_t temp;

	//get current scale and use it for final calculation
    temp = lis3mdl_read_byte(LIS3MDL_ADDRESS_CTRL3);

	temp = temp >> 5;
    temp &= 0x03;			//full scale bits exctracted

	lis3mdl_readArray(data, LIS3MDL_ADDRESS_XL, 6);

	xx = ((uint16_t)data[1]) << 8 | data[0];
	yy = ((uint16_t)data[3]) << 8 | data[2];
	zz = ((uint16_t)data[5]) << 8 | data[4];

    const float range_scale = 6842.0f; //range +-4gaus

	*x = (float)(xx/range_scale);
	*y = (float)(yy/range_scale);
	*z = (float)(zz/range_scale);
}


uint8_t lis3mdl_init(void)
{

	uint8_t status = 1;

	//LIS3MDL_ACC_ON;

	LL_mDelay(100);

	uint8_t val = lis3mdl_read_byte(LIS3MDL_WHO_AM_I_ADDRES);

	if(val == LIS3MDL_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		sLIS3MDLI2Caddress = LIS3MDL_DEVICE_ADDRESS_ALTERNATIVE;
		val = lis3mdl_read_byte(LIS3MDL_WHO_AM_I_ADDRES);
		if(val == LIS3MDL_WHO_AM_I_VALUE)
		{
			status = 1;
		}
		else
		{
			status = 0;
			return status;
		}
	}

	//acc device init

	uint8_t ctrl1 = LIS3MDL_CTRL1_OM_UHP | LIS3MDL_CTRL1_DO_80HZ | (1 << 7);
	lis3mdl_write_byte(LIS3MDL_ADDRESS_CTRL1, ctrl1);

	uint8_t ctrl2 = LIS3MDL_CTRL2_FS_4GAUS;
	lis3mdl_write_byte(LIS3MDL_ADDRESS_CTRL2, ctrl2);

	uint8_t ctrl3 = LIS3MDL_CTRL3_MD_CONTINUES;
	lis3mdl_write_byte(LIS3MDL_ADDRESS_CTRL3, ctrl3);

	uint8_t ctrl4 = LIS3MDL_CTRL4_OMZ_UHP;
	lis3mdl_write_byte(LIS3MDL_ADDRESS_CTRL4, ctrl4);

	return status;
}
