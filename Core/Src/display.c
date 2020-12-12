/*
 * display.c
 *
 *  Created on: Nov 29, 2020
 *      Author: Frixys
 */
#include "display.h"

//global variables
uint8_t curStr[NUM_DIG];
uint8_t complStr[MAX_STR_LENGTH];
uint8_t complStrLen = 19; //length without null char
uint8_t curPos = 0;
uint8_t shiftDir = LEFT_TO_RIGHT;
int8_t decSepPos = -5; //default value meaning there is no decimal point in string

const char segVal_ASCII_start = '-';
// 0b dABC DEFG
const uint8_t segVal_ASCII[]= {

/*  -     .     /    */
	0x01, 0x80, 0x00,
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7C, 0x4E, 0x3D, 0x4F, 0x47, 0x5E,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x37, 0x06, 0x3C, 0x57, 0x0E, 0x6A, 0x15, 0x7E, 0x67, 0x73, 0x05, 0x5B,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
	0x0F, 0x3E, 0x2A, 0x7F, 0x49, 0x3B, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x08,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x7D, 0x1F, 0x0D, 0x3D, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x00, 0x00, 0x15, 0x1D, 0x00, 0x73, 0x05, 0x00, 0x0F, 0x1C, 0x00, 0x00,
/*  x     y     z     */
    0x00, 0x3B, 0x00
};

const GPIO_TypeDef* seg_Ports[] = {seg_G_Port, seg_F_Port, seg_E_Port, seg_D_Port, seg_C_Port, seg_B_Port, seg_A_Port, seg_DP_Port};
const uint32_t seg_Pins[] = {seg_G_Pin, seg_F_Pin, seg_E_Pin, seg_D_Pin, seg_C_Pin, seg_B_Pin, seg_A_Pin, seg_DP_Pin};

const GPIO_TypeDef* dig_Ports[] = {dig_1_Port,dig_2_Port,dig_3_Port,dig_4_Port};
const uint32_t dig_Pins[] = {dig_1_Pin,dig_2_Pin,dig_3_Pin,dig_4_Pin};

//functions to handle multiplexing of the currently displaying alphanum char
void updateAlphanumChar(uint8_t seg_Val);
void updateDecimalPoint(uint8_t curActDig);
void setDigit(uint8_t pos);
void resetAllSegments();
void resetAllDigits();

//functions to handle shifting of the currently displaying string
void updateCurPos(uint8_t *prevPos, uint8_t shiftDir);
void updateShiftDir(uint8_t curPos, uint8_t *shiftDir);
void setCurStr(uint8_t curPos);

//functions to handle multiplexing of the currently displaying alphanum char
void DISPLAY_displayCurStr(){
	static uint8_t curActDig = 0;
	resetAllDigits();

	updateAlphanumChar( segVal_ASCII[ toupper(curStr[curActDig]) - segVal_ASCII_start] );
	updateDecimalPoint(curActDig);
	setDigit(curActDig);
	curActDig++;
	curActDig %= 4;
}

void updateAlphanumChar(uint8_t segmentValues){
	resetAllSegments();
	for(int i = 0; i<(NUM_SEG-1); i++){
		if(segmentValues & (0x01<<i)) LL_GPIO_ResetOutputPin((GPIO_TypeDef*)seg_Ports[i], (uint32_t)seg_Pins[i]);
	}
}

void updateDecimalPoint(uint8_t curActDig){
	if(decSepPos == (curPos + curActDig)){
		LL_GPIO_ResetOutputPin((GPIO_TypeDef*)seg_DP_Port, (uint32_t)seg_DP_Pin);
	}
}
//Turns required digit ON
void setDigit(uint8_t pos){
	LL_GPIO_SetOutputPin((GPIO_TypeDef*)dig_Ports[pos], (uint32_t)dig_Pins[pos]);
}

/*Reset (turn-off) all the segments of display*/
void resetAllSegments(){
	for(int i = 0; i<NUM_SEG; i++){
		LL_GPIO_SetOutputPin((GPIO_TypeDef*)seg_Ports[i], (uint32_t)seg_Pins[i]);
	}
}

/* Reset (turn-off) all digits*/
void resetAllDigits(void)
{
	for(int i = 0; i<NUM_DIG; i++){
		LL_GPIO_ResetOutputPin((GPIO_TypeDef*)dig_Ports[i], (uint32_t)dig_Pins[i]);
	}
}

//Functions to handle shifting of the currently displaying string
void DISPLAY_shiftCurStr(){
	//static uint8_t shiftDir = LEFT_TO_RIGHT;

	updateCurPos(&curPos, shiftDir);
	updateShiftDir(curPos, &shiftDir);
	setCurStr(curPos);
}

void updateCurPos(uint8_t *prevPos, uint8_t shiftDir){
	shiftDir == LEFT_TO_RIGHT ? (*prevPos)++ : (*prevPos)--;
}

void updateShiftDir(uint8_t curPos, uint8_t *shiftDir){
	if(curPos >= (complStrLen - NUM_DIG)){
		*shiftDir =  RIGHT_TO_LEFT;
	}

	if(curPos <= 0){
		*shiftDir = LEFT_TO_RIGHT;
	}
}

void setCurStr(uint8_t curPos){
	for(uint8_t i = 0; i<NUM_DIG; i++){
		curStr[i] = complStr[curPos + i];
	}
}

//Function to change the complete string which is continuously being displayed on the display
void DISPLAY_setCompStr(uint8_t *newComplStr,uint8_t size){
	//size - size of string without '\0'
	//determine position of the decimal point and parse string
	decSepPos = -5;

	for(int i = 0; i<size; i++){
		if(newComplStr[i] == '.'){
			decSepPos = i - 1; //-1 because i want it to be displayed on digit(its DP_segment) together with the previous char
		}

		if(decSepPos == -5){
			complStr[i] = newComplStr[i];
		}
		else{

			complStr[i] = newComplStr[i+1];
		}
	}


	complStrLen = (decSepPos == -5) ? size : (size - 1);  //(size-1) because '.' was extracted
	complStr[complStrLen] = '\0';
}

void DISPLAY_resetCurPos(){
	curPos = 1; //this way it starts from the real beginning not from the second position
	shiftDir = RIGHT_TO_LEFT;
}

