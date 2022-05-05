#include <string.h>

//#include <stm32f10x_flash.h>

#include <mystring.h>
//#include <mytrace.h>

#include "bootloader.h"

#define BOOT_VERSION 0x0100

/*#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

#define BANK0_START_ADDR  ((uint32_t)0x08000000)
#define BANK0_END_ADDR    ((uint32_t)0x08017ff0)
#define BANK0_NUMBER_PAGES ((BANK0_END_ADDR - BANK0_START_ADDR) / 0x800)
#define BANK1_START_ADDR  ((uint32_t)0x08020000)
#define BANK1_END_ADDR    ((uint32_t)0x0803ffff)
#define NUMBER_PAGES ((BANK1_END_ADDR - BANK1_START_ADDR) / 0x800)*/

const unsigned short CRC16STable[] =\
{\
	0x0000,0xC0C1,0xC181,0x0140,\
	0xC301,0x03C0,0x0280,0xC241,\
	0xC601,0x06C0,0x0780,0xC741,\
	0x0500,0xC5C1,0xC481,0x0440,\
	0xCC01,0x0CC0,0x0D80,0xCD41,\
	0x0F00,0xCFC1,0xCE81,0x0E40,\
	0x0A00,0xCAC1,0xCB81,0x0B40,\
	0xC901,0x09C0,0x0880,0xC841,\
	0xD801,0x18C0,0x1980,0xD941,\
	0x1B00,0xDBC1,0xDA81,0x1A40,\
	0x1E00,0xDEC1,0xDF81,0x1F40,\
	0xDD01,0x1DC0,0x1C80,0xDC41,\
	0x1400,0xD4C1,0xD581,0x1540,\
	0xD701,0x17C0,0x1680,0xD641,\
	0xD201,0x12C0,0x1380,0xD341,\
	0x1100,0xD1C1,0xD081,0x1040,\
	0xF001,0x30C0,0x3180,0xF141,\
	0x3300,0xF3C1,0xF281,0x3240,\
	0x3600,0xF6C1,0xF781,0x3740,\
	0xF501,0x35C0,0x3480,0xF441,\
	0x3C00,0xFCC1,0xFD81,0x3D40,\
	0xFF01,0x3FC0,0x3E80,0xFE41,\
	0xFA01,0x3AC0,0x3B80,0xFB41,\
	0x3900,0xF9C1,0xF881,0x3840,\
	0x2800,0xE8C1,0xE981,0x2940,\
	0xEB01,0x2BC0,0x2A80,0xEA41,\
	0xEE01,0x2EC0,0x2F80,0xEF41,\
	0x2D00,0xEDC1,0xEC81,0x2C40,\
	0xE401,0x24C0,0x2580,0xE541,\
	0x2700,0xE7C1,0xE681,0x2640,\
	0x2200,0xE2C1,0xE381,0x2340,\
	0xE101,0x21C0,0x2080,0xE041,\
	0xA001,0x60C0,0x6180,0xA141,\
	0x6300,0xA3C1,0xA281,0x6240,\
	0x6600,0xA6C1,0xA781,0x6740,\
	0xA501,0x65C0,0x6480,0xA441,\
	0x6C00,0xACC1,0xAD81,0x6D40,\
	0xAF01,0x6FC0,0x6E80,0xAE41,\
	0xAA01,0x6AC0,0x6B80,0xAB41,\
	0x6900,0xA9C1,0xA881,0x6840,\
	0x7800,0xB8C1,0xB981,0x7940,\
	0xBB01,0x7BC0,0x7A80,0xBA41,\
	0xBE01,0x7EC0,0x7F80,0xBF41,\
	0x7D00,0xBDC1,0xBC81,0x7C40,\
	0xB401,0x74C0,0x7580,0xB541,\
	0x7700,0xB7C1,0xB681,0x7640,\
	0x7200,0xB2C1,0xB381,0x7340,\
	0xB101,0x71C0,0x7080,0xB041,\
	0x5000,0x90C1,0x9181,0x5140,\
	0x9301,0x53C0,0x5280,0x9241,\
	0x9601,0x56C0,0x5780,0x9741,\
	0x5500,0x95C1,0x9481,0x5440,\
	0x9C01,0x5CC0,0x5D80,0x9D41,\
	0x5F00,0x9FC1,0x9E81,0x5E40,\
	0x5A00,0x9AC1,0x9B81,0x5B40,\
	0x9901,0x59C0,0x5880,0x9841,\
	0x8801,0x48C0,0x4980,0x8941,\
	0x4B00,0x8BC1,0x8A81,0x4A40,\
	0x4E00,0x8EC1,0x8F81,0x4F40,\
	0x8D01,0x4DC0,0x4C80,0x8C41,\
	0x4400,0x84C1,0x8581,0x4540,\
	0x8701,0x47C0,0x4680,0x8641,\
	0x8201,0x42C0,0x4380,0x8341,\
	0x4100,0x81C1,0x8081,0x4040\
};

unsigned short (*boot_func)(boot_option_enum p_opt);
unsigned short CRC16_Byte(unsigned char p_index);


unsigned short BOOT_Program(boot_option_enum p_opt) __attribute__((section("BOOT_SECTION")));

//volatile FLASH_Status FLASHStatus;
volatile unsigned long BOOT_Save_Address;

unsigned short BOOT_Init(boot_option_enum p_opt){

	unsigned short tmp_boot_value;
	char boot_tmp_str[10];

	boot_func=(void*)0x08018001;	 		//ORIGINAL ONE

	//boot_func=(void*)0x08018000;
	//FLASHStatus = FLASH_COMPLETE;
	//BOOT_Save_Address=BANK1_START_ADDR;
	if(p_opt==BOOT_OPT_VER){


		//tmp_boot_value=boot_func(BOOT_OPT_VER);
		//myLongStr(tmp_boot_value,boot_tmp_str,10,16);
		//myTRACE_Print("Bootloader Ver=0x");
		//myTRACE_Print(boot_tmp_str);
		//myTRACE_Print("\r\n");
		return tmp_boot_value;
	}
	else if(p_opt==BOOT_OPT_PGM){
		__disable_irq();
		//boot_func(BOOT_OPT_PGM);
		BOOT_Program(BOOT_OPT_PGM);

	}
	return 0;
}

void BOOT_Erase(void){
	unsigned short EraseCounter;
	FLASH_Unlock();
	//FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	/*BOOT_Save_Address=BANK1_START_ADDR;
	//FLASHStatus = FLASH_COMPLETE;
	for(EraseCounter = 0; (EraseCounter < NUMBER_PAGES) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++){
	        FLASHStatus = FLASH_ErasePage(BANK1_START_ADDR + (0x800 * EraseCounter));
	    }*/
	//FLASH_Lock();
}

void BOOT_Save(unsigned long *p_buffer,unsigned short p_size){
	unsigned long boot_tmp_address;
	boot_tmp_address = BOOT_Save_Address;

	//FLASHStatus = FLASH_COMPLETE;
    /*while((boot_tmp_address < (BOOT_Save_Address+p_size)) && (FLASHStatus == FLASH_COMPLETE))
    {

        FLASHStatus = FLASH_ProgramWord(boot_tmp_address, *p_buffer);
        boot_tmp_address = boot_tmp_address + 4;
        p_buffer++;
    }*/

    BOOT_Save_Address+=p_size;
}


char BOOT_Crc(unsigned short p_crc,unsigned long p_size){

	unsigned short tmp_crc16;
	unsigned short tmp_crc16_byte;
	char tmp_crc_result;
	char boot_tmp_str[20];
	unsigned char *boot_tmp_pnt;

	tmp_crc16=0x0;
	tmp_crc_result=0;
	//boot_tmp_pnt=(unsigned char*)BANK1_START_ADDR;

	while(p_size){

		tmp_crc16_byte=CRC16_Byte((tmp_crc16 ^ *boot_tmp_pnt) & 0xFF);
		tmp_crc16 = (tmp_crc16 >> 8) ^ tmp_crc16_byte;
		boot_tmp_pnt++;
		p_size--;
	}

	//myTRACE_Print("CRC=");
	//myLongStr(tmp_crc16,boot_tmp_str,20,16);
	//strcat(boot_tmp_str,"-");
	//myTRACE_Print(boot_tmp_str);
	//myLongStr(p_crc,boot_tmp_str,20,16);
	//strcat(boot_tmp_str,"\n\r");
	//myTRACE_Print(boot_tmp_str);

	//tmp_crc16 = (tmp_crc16 >> 8) ^ m_CRC16STable[(tmp_crc16 ^ Byte) & 0xFF];
	if(p_crc==tmp_crc16)tmp_crc_result=1;

	return tmp_crc_result;
}

unsigned short CRC16_Byte(unsigned char p_index){
	char tmp_crc_str[3];
	unsigned short tmp_crc16_tbl;

	memcpy(tmp_crc_str,(char*)&CRC16STable[p_index],2);

	tmp_crc16_tbl=tmp_crc_str[1]<<8;
	tmp_crc16_tbl+=tmp_crc_str[0];
	return 	tmp_crc16_tbl;
}

unsigned short BOOT_Program(boot_option_enum p_opt){
	unsigned long tmp_count;
	tmp_count =0;
	unsigned long tmp_read_addr;
	unsigned long tmp_write_addr;

	if(p_opt==BOOT_OPT_VER){
		//myTRACE_Print("Boot Ver\r");
		//return BOOT_VERSION;
	}
	else if(p_opt==BOOT_OPT_PGM){
		//Unlock
		/*FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;

		//Erase
		for(tmp_count=0;tmp_count<BANK0_NUMBER_PAGES;tmp_count++){

			while((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY);
			FLASH->CR|= CR_PER_Set;
			FLASH->AR = BANK0_START_ADDR+(0x800*tmp_count);
			FLASH->CR|= CR_STRT_Set;
			while((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY);
			FLASH->CR &= CR_PER_Reset;
		}*/

		//Program
		//tmp_read_addr=BANK1_START_ADDR;
		//tmp_write_addr=BANK0_START_ADDR;
/*
		for(tmp_count=0;tmp_count<(BANK0_NUMBER_PAGES*0x400);tmp_count++){
			while((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY);
			FLASH->CR |= CR_PG_Set;
			*(__IO uint16_t*)tmp_write_addr=*(__IO uint16_t*)tmp_read_addr;
			while((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY);
			tmp_write_addr+=2;
			tmp_read_addr+=2;
			FLASH->CR &= CR_PG_Reset;
		}*/
	}

	/*IWDG->KR = ((uint16_t)0x5555);
	IWDG->PR = ((uint8_t)0x03);
	IWDG->RLR = 349;
	IWDG->KR = ((uint16_t)0xAAAA);
	IWDG->KR = ((uint16_t)0xCCCC);
	  while(1);*/
	return 0;
}

