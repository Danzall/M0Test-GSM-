#ifndef _BOOTLOADER_H
#define _BOOTLOADER_H

typedef enum{
	BOOT_OPT_VER=0,
	BOOT_OPT_PGM
}boot_option_enum;

unsigned short BOOT_Init(boot_option_enum p_opt);
void BOOT_Erase(void);
void BOOT_Save(unsigned long *p_buffer,unsigned short p_size);
char BOOT_Crc(unsigned short p_crc,unsigned long p_size);
#endif
