/*
 * Flash_Driver.c
 *
 *  Created on: 02 Jan 2021
 *      Author: munya
 */

//#include "LED_Driver.h"
#include "main.h"
//#include "gpio.h"
//#include "usart.h"
//#include "spi.h"

#include "main.h"
#include "stm32f0xx_hal.h"
//#include "cmsis_os.h"
#include "string.h"
//#include "GSM.h"
//#include "ADE7953.h"
//#include "myString.h"

#include "Flash_Driver.h"


volatile FLASH_struct flash_struct;

extern SPI_HandleTypeDef hspi2;

void Flash_Init(){

	//HAL_UART_Transmit(&huart1,"FLASH Init\n",11,5);
	Debug_Send("FLASH Init\n");

	//HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin,GPIO_PIN_SET);
}



void Flash_Read(unsigned long p_addr,char *p_buffer,unsigned short p_size){
	unsigned int m95_index;
	uint8_t m95_addr[4];
	flash_struct.isBusy = 1;
	M95_Busy();
	Flash_CS_Low();

	m95_addr[0] = M95_READ;
	m95_addr[1] = (p_addr>>16)&0xff;
	m95_addr[2] = (p_addr>>8)&0xff;
	m95_addr[3] = p_addr&0xff;

	HAL_SPI_Transmit(&hspi2,m95_addr,4,100);

	HAL_SPI_Receive(&hspi2,(uint8_t *)p_buffer,p_size,100);

	Flash_CS_Hi();

	flash_struct.isBusy = 0;
}

void M95_Put(unsigned long p_addr,char *p_buffer,unsigned short p_size){
	//unsigned int m95_index;
	unsigned char m95_addr[4];
	uint8_t tesr[100];

	M95_Busy();

	Flash_CS_Low();

	m95_addr[0] = M95_WRITE_EN;
	HAL_SPI_Transmit(&hspi2,m95_addr,1,100);
	Flash_CS_Hi();
	Debug_Send("Unlocked\r\n");
	HAL_Delay(10);

	Flash_CS_Low();

	m95_addr[0] = M95_WRITE;
	m95_addr[1] = (p_addr>>16)&0xff;
	m95_addr[2] = (p_addr>>8)&0xff;
	m95_addr[3] = p_addr&0xff;

	HAL_SPI_Transmit(&hspi2,m95_addr,4,100);


	HAL_SPI_Transmit(&hspi2,(uint8_t)p_buffer,p_size,100);

	Flash_CS_Hi();

	Flash_CS_Low();

	m95_addr[0] = M95_WRITE_DIS;

	Flash_CS_Hi();
}

void Flash_Write( unsigned long p_addr,const char *p_buffer,unsigned short p_size){
	//unsigned short m95_tmp;
	unsigned short m95_page_size;
	unsigned short m95_size;
	unsigned short m95_write_size;
	//unsigned char test[10];

	M95_Busy();

	Flash_CS_Low();

	//myTRACE_Print(p_buffer);
	//myTRACE_Print(" p_buffer\n");
	flash_struct.isBusy = 1;
	m95_size=p_size;
	//m95_tmp=p_addr/256;
	//m95_page_size=256-(p_addr-(m95_tmp*256));

	m95_page_size =256 - ( p_addr % 256);
	while(m95_size){
		if(m95_size>m95_page_size)
			m95_write_size=m95_page_size;
		else
			m95_write_size=m95_size;
		M95_Put(p_addr,p_buffer,m95_write_size);
		m95_size-=m95_write_size;
		p_buffer+=m95_write_size;
		p_addr+=m95_write_size;
		m95_page_size=256;
	}
	flash_struct.isBusy = 0;
	Flash_CS_Hi();

}

void Flash_Write_Bin(unsigned long p_addr,char p_bin){
	M95_Busy();
	Debug_Send("Busy done\r\n");
	flash_struct.isBusy = 1;
	M95_Put(p_addr,&p_bin,1);
	flash_struct.isBusy = 0;
}

char Flash_Read_Bin(unsigned long p_addr){
	uint8_t m95_addr[4];

	flash_struct.isBusy = 1;
	M95_Busy();
	Flash_CS_Low();

	m95_addr[0] = M95_READ;
	m95_addr[1] = (p_addr>>16)&0xff;
	m95_addr[2] = (p_addr>>8)&0xff;
	m95_addr[3] = p_addr&0xff;
	HAL_SPI_Transmit(&hspi2,m95_addr,4,100);

	HAL_SPI_Receive(&hspi2,m95_addr,1,100);

	Flash_CS_Hi();
	flash_struct.isBusy = 0;
	return m95_addr[0];
}



//ENSURE THAT FLASH IS NOT BUSY WITH WRITE IN PROGRESS BEFORE ISSUING ANOTHER COMMAND
void M95_Busy(){
	uint8_t m95_status[1];
	Flash_CS_Low();
	m95_status[0] = M95_STATUS;

	HAL_SPI_Transmit(&hspi2,m95_status,1,100);
	//SPI_Write(M95_STATUS);
	HAL_SPI_Receive(&hspi2,m95_status,1,100);
	//m95_status=SPI_Write(0);
	while(m95_status[0]&0x01){
		//SET_ROLL();
		//ROLL_UP();
		HAL_SPI_Receive(&hspi2,m95_status,1,100);
		//DWT_Delay_us(20);
		//m95_status=SPI_Write(0);
		//if(Boot_Bit)
		//	HAL_IWDG_Refresh(&hiwdg);						// RELOAD WDT WHILE FLASH IS BUSY
	}
	Flash_CS_Hi();
	//CLR_ROLL();
}


void Flash_CS_Low(){
		//HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
}

void Flash_CS_Hi(){
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin,GPIO_PIN_SET);
}


unsigned char Flash_isBusy(void){
/*	if(flash_struct.isBusy)
		return 1;
	else
		return 0;
*/
}
