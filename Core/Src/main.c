/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"
#include "usb_device.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define M95_WRITE_EN 	0x06
#define M95_WRITE_DIS 	0x04
#define M95_STATUS 		0x05
#define M95_READ 		0x03
//#define M95_READ 		0x0B
#define M95_WRITE 		0x02
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char USB_Send[] = "Hello USB";
extern int mqttSend;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	remap();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin,GPIO_PIN_SET);
  //MQTT_Init();
  GSM_Init();
  //Wifi_Init();
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_SET);		//wifi enable
  //HAL_Delay(2000);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */
	//if (huart->Instance == USART2) GSM_Receive((uint8_t)(uhdata & (uint8_t)uhMask));
	//if (huart->Instance == USART1) Debug_Receive((uint8_t)(uhdata & (uint8_t)uhMask));

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED2_Pin|LED1_Pin|GSM_Enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GSM_On_Pin|LED5_Pin|RelayOpen_Pin|RelayClose_Pin
                          |LED4_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED2_Pin LED1_Pin GSM_Enable_Pin */
  GPIO_InitStruct.Pin = LED2_Pin|LED1_Pin|GSM_Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : GSM_On_Pin LED5_Pin RelayOpen_Pin RelayClose_Pin
                           LED4_Pin LED3_Pin */
  GPIO_InitStruct.Pin = GSM_On_Pin|LED5_Pin|RelayOpen_Pin|RelayClose_Pin
                          |LED4_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_Pin */
  GPIO_InitStruct.Pin = CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*void Debug_Send(char* data){
	int size;
	int timeout = 5;
	size = strlen(data);
	HAL_UART_Transmit(&huart1, (uint8_t*)data, size, timeout);
	//HAL_UART_Transmit_IT(&huart1, data, size);
}*/

/*void flashRead(){
	 char flash[150];
	 int address = 0;
	 int size = 10;
	 while (address < 100){
		 memset(flash,0,150);
		 Debug_Send("Test Read flash addres: ");
		 myLongStr(address,flash,10,10);
		 Debug_Send(flash);
		 Debug_Send("\r\n");
		 Flash_Read(address,flash,size);
		 address += size;
		 flash[size] = 0;
		 Debug_Send(flash);
		 Debug_Send("\r\n");
	 }
	 //Flash_Write(80,"2.08",7);
	 //Flash_Write(0,Flash_Version,strlen(Flash_Version)+1);
	 Flash_Write_Bin(30,'D');
	 //M95_Put(30,'D',1);
	 //M95_Busy();
	 //Flash_Write(1000,"test",5);
	 //Flash_Read(30,flash,6);
	 //flash[6] = 0;
	 //address += 10;
	 Debug_Send("Read flash: ");
	 //Debug_Send(flash);
	 Debug_Send("\r\n");

 }*/

void remap(){
	 // Copy interrupt vector table to the RAM.
	 volatile uint32_t *VectorTable = (volatile uint32_t *)0x20000000;
	 uint32_t ui32_VectorIndex = 0;

	 for(ui32_VectorIndex = 0; ui32_VectorIndex < 48; ui32_VectorIndex++)
	 {
		 VectorTable[ui32_VectorIndex] = *(__IO uint32_t*)((uint32_t) 0x8009000 + (ui32_VectorIndex << 2));
	 }

	 __HAL_RCC_AHB_FORCE_RESET();

	 //  Enable SYSCFG peripheral clock
	 __HAL_RCC_SYSCFG_CLK_ENABLE();

	 __HAL_RCC_AHB_RELEASE_RESET();

	 // Remap RAM into 0x0000 0000
	 __HAL_SYSCFG_REMAPMEMORY_SRAM();

	 __enable_irq();
	 Debug_Send("test Vector table remapped\r\n");
}

void flashPut(uint32_t p_addr,char* data, uint32_t size){
	 unsigned char temp[30];
	 Debug_Send("Flash write: ");
	 /*Debug_Send(data);
	 if(data[0] == 0x0a) Debug_Send("a");
	 if(*data == 0x0a) Debug_Send("A");
	 if(data[0] == 0x0d) Debug_Send("d");
	 if(*data == 0x0d) Debug_Send("D");*/
	 Debug_Send(" at: ");
	 myLongStr(p_addr,temp,10,10);
	 Debug_Send(temp);
	 Debug_Send(" , ");
	 myLongStr(size,temp,10,10);
	 Debug_Send(temp);
	 Debug_Send("\r\n");
	 flashBusy();
	 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);	//set write enable flag
	 temp[0] = M95_WRITE_EN;
	 HAL_SPI_Transmit(&hspi2,temp,1,100);
	 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	 HAL_Delay(1);

	 //uint32_t p_addr = 30;

	 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);	//load address
	 temp[0] = M95_WRITE;
	 temp[1] = (p_addr>>16)&0xff;
	 temp[2] = (p_addr>>8)&0xff;
	 temp[3] = p_addr&0xff;

	 HAL_SPI_Transmit(&hspi2,temp,4,100);						//transmit address

	 temp[0] = 68;
	 HAL_SPI_Transmit(&hspi2,data,size,100);						//transmit data
	 HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);

	 flashBusy();

}

void flashWrite( unsigned long p_addr,const char *p_buffer,unsigned short p_size){
	//unsigned short m95_tmp;
	unsigned short m95_page_size;
	unsigned short m95_size;
	unsigned short m95_write_size;
	//unsigned char test[10];

	//M95_Busy();

	//Flash_CS_Low();

	//myTRACE_Print(p_buffer);
	//myTRACE_Print(" p_buffer\n");
	//flash_struct.isBusy = 1;
	m95_size=p_size;
	//m95_tmp=p_addr/256;
	//m95_page_size=256-(p_addr-(m95_tmp*256));

	m95_page_size =256 - ( p_addr % 256);
	while(m95_size){
		if(m95_size>m95_page_size)
			m95_write_size=m95_page_size;
		else
			m95_write_size=m95_size;
		flashPut(p_addr,p_buffer,m95_write_size);
		m95_size-=m95_write_size;
		p_buffer+=m95_write_size;
		p_addr+=m95_write_size;
		m95_page_size=256;
	}
	//flash_struct.isBusy = 0;
	//Flash_CS_Hi();

}

void flashRead(uint32_t p_addr, char* data, uint32_t size){
	 char temp[20];
	 /*Debug_Send("Flash read at: ");
	 myLongStr(p_addr,temp,10,10);
	 Debug_Send(temp);
	 Debug_Send("\r\n");*/

	flashBusy();
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	//uint32_t p_addr = 30;
	 temp[0] = M95_READ;
	 temp[1] = (p_addr>>16)&0xff;
	 temp[2] = (p_addr>>8)&0xff;
	 temp[3] = p_addr&0xff;

	HAL_SPI_Transmit(&hspi2,temp,4,100);

	HAL_SPI_Receive(&hspi2,data,size,100);

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	//temp[3] = 0;
	//Debug_Send(temp);
	//Debug_Send("\r\n");

	 /*Debug_Send("Flash read: ");
	 Debug_Send(data);
	 Debug_Send(" at: ");
	 myLongStr(p_addr,temp,10,10);
	 Debug_Send(temp);
	 Debug_Send(" , ");
	 myLongStr(size,temp,10,10);
	 Debug_Send(temp);
	 Debug_Send("\r\n");*/
}

void flashBusy(){
	 char temp[20];
	 temp[0] = M95_STATUS;
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi2,temp,1,100);
	//SPI_Write(M95_STATUS);
	HAL_SPI_Receive(&hspi2,temp,1,100);
	//m95_status=SPI_Write(0);
	while(temp[0]&0x01){
		//SET_ROLL();
		//ROLL_UP();
		HAL_SPI_Receive(&hspi2,temp,1,100);
		//DWT_Delay_us(20);
		//m95_status=SPI_Write(0);
		//if(Boot_Bit)
		//	HAL_IWDG_Refresh(&hiwdg);						// RELOAD WDT WHILE FLASH IS BUSY
	}
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
    //HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    //Debug_Send("M0 test running\r\n");
	if(mqttSend == 1){
		mqttSend = 0;
		Debug_Send("Publish func returnd\r\n");
	}
    GSM_Service();

    //CDC_Transmit_FS((uint8_t*) USB_Send, strlen(USB_Send));
    //Debug_Send("M0 test running\r\n");
    //GSM_Send("AT\r\n");
    //Wifi_Service();
    //flashRead();
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(50);
    recData();
    //WifirecData();
  }
  /* USER CODE END StartTask02 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
