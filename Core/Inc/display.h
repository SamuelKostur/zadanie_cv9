/*
 * display.h
 *
 *  Created on: Nov 29, 2020
 *      Author: Frixys
 */
/*
#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_
#endif*/ /* INC_DISPLAY_H_ */
#include <stdio.h>
#include "stm32f3xx_ll_gpio.h"
#include <ctype.h>

#define NUM_DIG 4 //number of display digs representing numbers
#define NUM_SEG 8 //number of segments per digit
#define LEFT_TO_RIGHT 1
#define RIGHT_TO_LEFT 0
#define MAX_STR_LENGTH 14
#define seg_A_Pin LL_GPIO_PIN_1
#define seg_B_Pin LL_GPIO_PIN_0
#define seg_C_Pin LL_GPIO_PIN_8
#define seg_D_Pin LL_GPIO_PIN_5
#define seg_E_Pin LL_GPIO_PIN_11
#define seg_F_Pin LL_GPIO_PIN_3
#define seg_G_Pin LL_GPIO_PIN_4
#define seg_DP_Pin LL_GPIO_PIN_1

#define seg_A_Port GPIOA
#define seg_B_Port GPIOA
#define seg_C_Port GPIOA
#define seg_D_Port GPIOB
#define seg_E_Port GPIOA
#define seg_F_Port GPIOA
#define seg_G_Port GPIOB
#define seg_DP_Port GPIOB

#define dig_1_Pin LL_GPIO_PIN_5
#define dig_2_Pin LL_GPIO_PIN_4
#define dig_3_Pin LL_GPIO_PIN_6
#define dig_4_Pin LL_GPIO_PIN_2
#define dig_time_Pin LL_GPIO_PIN_7

#define dig_1_Port GPIOA
#define dig_2_Port GPIOA
#define dig_3_Port GPIOA
#define dig_4_Port GPIOA
#define dig_time_Port GPIOA

//functions
void DISPLAY_displayCurStr();
void DISPLAY_shiftCurStr();
void DISPLAY_setCompStr(uint8_t *newComplStr, uint8_t size);
void DISPLAY_resetCurPos();
