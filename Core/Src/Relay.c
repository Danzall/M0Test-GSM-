/*
 * Relay.c
 *
 *  Created on: May 17, 2022
 *      Author: Danzall
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "GSM.h"
//#include "motor.h"
/* USER CODE END Includes */

void RelayOn(){
	HAL_GPIO_WritePin(RelayOpen_GPIO_Port, RelayClose_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(RelayOpen_GPIO_Port, RelayClose_Pin, GPIO_PIN_RESET);
}

void RelayOff(){
	HAL_GPIO_WritePin(RelayOpen_GPIO_Port, RelayOpen_Pin, GPIO_PIN_SET);
	HAL_Delay(50);
	HAL_GPIO_WritePin(RelayOpen_GPIO_Port, RelayOpen_Pin, GPIO_PIN_RESET);
}
