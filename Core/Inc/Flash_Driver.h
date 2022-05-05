/*
 * FLash_Driver.h
 *
 *  Created on: 02 Jan 2021
 *      Author: munya
 */

#ifndef FLASH_DRIVER_H_
#define FLASH_DRIVER_H_

#define M95_WRITE_EN 	0x06
#define M95_WRITE_DIS 	0x04
#define M95_STATUS 		0x05
#define M95_READ 		0x03
//#define M95_READ 		0x0B
#define M95_WRITE 		0x02
//#define FLASH_ID		0x9F
//#define FLASH_RS1		0x05
//#define FLASH_RS2		0x35
#define FLASH_ID		0x90
#define FLASH_CE		0x60		// CHIP ERASE
#define FLASH_SE		0x20		// 04kB SECTOR ERASE
#define FLASH_BE		0xD8		// 64kB SECTOR ERASE

typedef enum{
	FLASH1 = 0,
	FLASH2
}Flash_No;

typedef struct{
	unsigned char isFormat;
	unsigned char isBusy;
	unsigned char useFlash;
	//Flash_No activeFlash;
}FLASH_struct;

void Flash_Init();
void Flash_Read(unsigned long p_addr,char *p_buffer,unsigned short p_size);

void Flash_Write(unsigned long p_addr,const char *p_buffer,unsigned short p_size);
unsigned char Flash_isBusy(void);
void Flash_GetId();
void Flash_ReadStatus();
void Flash_Chip_Erase();
void Flash_ReadFT(unsigned long start_address, unsigned long end_address );
void Flash_Erase_Sector(unsigned int p_address);

void Flash_Write_Bin(unsigned long p_addr,char p_bin);
char Flash_Read_Bin(unsigned long p_addr);

//void Flash_Set(Flash_No flasher);
//Flash_No Flash_Get();
void M95_Busy();
void Flash_Block_Erase(unsigned long p_addr,char p_size);
void Flash_CS_Low();
void Flash_CS_Hi();

void Flash_Init();

#endif /* FLASH_DRIVER_H_ */
