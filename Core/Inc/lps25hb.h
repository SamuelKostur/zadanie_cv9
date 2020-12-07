/*
 * lps25hb.h
 *
 *  Created on: Dec 4, 2020
 *      Author: Frixys
 */

//#ifndef INC_LPS25HB_H_
//#define INC_LPS25HB_H_
//#endif /* INC_LPS25HB_H_ */

#include "main.h"
#include "i2c.h"

#define 	LPS25HB_DEVICE_ADDRESS_0				0xB8
#define 	LPS25HB_DEVICE_ADDRESS_1				0xBA

#define 	LPS25HB_WHO_AM_I_VALUE					0xBD
#define 	LPS25HB_WHO_AM_I_ADDRES					0x0F

#define 	LPS25HB_ADDRESS_CTRL1					0x20

#define 	LPS25HB_ADDRESS_ACCX					0x28
#define 	LPS25HB_ADDRESS_ACCY					0x2A
#define 	LPS25HB_ADDRESS_ACCZ					0x2C

#define 	LPS25HB_ADDRESS_BAR				0x28

uint8_t lps25hb_init(void);
void lps25hb_get_acc(float* x, float * y, float * z);
uint8_t lps25hb_read_byte(uint8_t reg_addr);
void lps25hb_write_byte(uint8_t reg_addr, uint8_t value);
float lps25hb_get_bar();
