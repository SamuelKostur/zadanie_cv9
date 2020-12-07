/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "lsm6ds0.h"
#include "hts221.h"
#include "lps25hb.h"
#include "lis3mdltr.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float data[NUM_DATA]; // {azimuth, temperature, humidity, pressure, altitude}
float minDataVal[] = {-99.9, -99.9, 0, 0, -9999.9};
float maxDataVal[] = {99.9, 99.9, 99, 9999.99, 9999.9};
uint8_t dataID = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void writeDisplay();
float calcAzimuth(float x, float y);
float calcAltitude(float pressure, float temperature);
float adjustData(float value, float minValue, float maxValue);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  //lsm6ds0_init();
  hts221_init();
  lps25hb_init();
  lis3mdl_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  MAIN_updateData();
	  LL_mDelay(50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */
	void writeDisplay(){
		static uint8_t compStr[13];
		switch (dataID){
			case 0:
				DISPLAY_setCompStr(compStr ,sprintf((char*) compStr,"MAG_%04.1f",data[dataID]));
				break;
			case 1:
				DISPLAY_setCompStr(compStr ,sprintf((char*) compStr,"TEMP_%04.1f",data[dataID]));
				break;
			case 2:
				DISPLAY_setCompStr(compStr ,sprintf((char*) compStr,"HUM_%02.0f",data[dataID]));
				break;
			case 3:
				DISPLAY_setCompStr(compStr ,sprintf((char*) compStr,"BAR_%07.2f",data[dataID]));
				break;
			case 4:
				DISPLAY_setCompStr(compStr ,sprintf((char*) compStr,"ALT_%06.1f",data[dataID]));
				break;
		}
	}

	void MAIN_buttonShiftDataID(){
		dataID++;
		dataID %= 5;
		DISPLAY_resetCurPos();
	}

	void MAIN_updateData(){
		//static float acc[3];
		//lsm6ds0_get_acc(acc, (acc+1), (acc+2));
		static float mag[3];
		lis3mdl_get_mag(mag,(mag+1), (mag+2));
		data[0] = calcAzimuth(mag[0], mag[1]);

		data[1] = hts221_get_temp();
		data[2] = hts221_get_humidity();
		data[3] = lps25hb_get_press();
		data[4] = calcAltitude(data[1], data[3]);

		for (int i = 0; i < NUM_DATA; i++){
			data[i] = adjustData(data[i],minDataVal[i],maxDataVal[i]);
		}
	}

	float adjustData(float value, float minValue, float maxValue){
		if(value < minValue){
			value = minValue;
		}

		if(value > maxValue){
			value = maxValue;
		}
		return value;
	}

	float calcAzimuth(float x, float y){
		return (float) (atan2((double)y,(double)x)) * 180.0f/3.1415f;
	}

	float calcAltitude(float pressure, float temperature){
		const float P0 = 1013.25;
		return (float) (pow((double)(P0/pressure), (double)(1.0f/5.257f)) -1.0f) * (temperature + 273.15f)/0.0065f;
	}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
