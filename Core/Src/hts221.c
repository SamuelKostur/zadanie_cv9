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
	uint8_t MSB_Tx = hts221_read_byte(HTS221_ADDRESS_MSB_T0_T1_degC_x8);
	int16_t MSB_T0 = ((int16_t) (MSB_Tx & 0b00000011)) << 8;
	int16_t MSB_T1 = ((int16_t) (MSB_Tx & 0b00001100)) << 6;

	float T0_degC = (float) ((int16_t)hts221_read_byte(HTS221_ADDRESS_T0_degC_x8) | MSB_T0) / 8.0f;
	float T1_degC = (float) ((int16_t)hts221_read_byte(HTS221_ADDRESS_T1_degC_x8) | MSB_T1) / 8.0f;

	uint8_t temporary[2];
	hts221_readArray(temporary, HTS221_ADDRESS_T0_OUT_L, 2);
	int16_t T0_OUT =  (int16_t)(temporary[1] << 8) | temporary[0];

	hts221_readArray(temporary, HTS221_ADDRESS_T1_OUT_L, 2);
	int16_t T1_OUT = (int16_t)(temporary[1] << 8) | temporary[0];

	hts221_readArray(temporary, HTS221_ADDRESS_TEMP_L, 2);
	int16_t T_OUT =  (int16_t)(temporary[1] << 8) | temporary[0];

	return (T1_degC - T0_degC) * (T_OUT - T0_OUT) / (T1_OUT - T0_OUT) + T0_degC;
}

float hts221_get_humidity()
{
	const float H0_rh = hts221_read_byte(HTS221_ADDRESS_H0_rH_x2)/ 2.0f;
	const float H1_rh = hts221_read_byte(HTS221_ADDRESS_H1_rH_x2)/ 2.0f;

	uint8_t temporary[2];
	hts221_readArray(temporary, HTS221_ADDRESS_H0_OUT_L, 2);
	int16_t H0_OUT =  (int16_t)(temporary[1] << 8) | temporary[0];

	hts221_readArray(temporary, HTS221_ADDRESS_H1_OUT_L, 2);
	int16_t H1_OUT =  (int16_t)(temporary[1] << 8) | temporary[0];

	hts221_readArray(temporary, HTS221_ADDRESS_HUM_L, 2);
	int16_t H_OUT =  (int16_t)(temporary[1] << 8) | temporary[0];

	return (H1_rh - H0_rh) * (H_OUT - H0_OUT) / (H1_OUT - H0_OUT) + H0_rh;
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

