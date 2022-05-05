/*
 * ggFSM.c
 *
 *  Created on: 22 Feb 2021
 *      Author: munyag
 */

//#include "ggParam.h"
#include "string.h"
#include "main.h"

#include "Flash_Driver.h"
#include "ggFSM.h"

void ggFS_Init(){

	//CHECK HEADER OF FILESYSTEM AND FORMAT IF THEY ARE DIFFERENT
	char tmp_string[30];

	memset(tmp_string,0,30);
	Flash_Read(0,tmp_string,4);
	//tmp_string[4] = 0;
	//pc_uart_message(test_string);
	Debug_Send("FS_Ver1 ");
	Debug_Send(tmp_string);
	Debug_Send("\n");

	if(strcmp(tmp_string,Flash_Version)){
		//Flash_Chip_Erase();
		Debug_Send("MIS-MATCH FILE SYSTEM ON FLASH-1\n\r");


		Flash_Write(0,Flash_Version,strlen(Flash_Version)+1);			// blank the top part of param
		Flash_Read(0,tmp_string,5);
		//tmp_string[4] = 0;
		Debug_Send("FS_Ver Confirm ");
		Debug_Send(tmp_string);
		Debug_Send(" vs ");
		Debug_Send(Flash_Version);
		Debug_Send("\n");
	}



	memset(tmp_string,0,30);

	//ggParam_Init();

	//Flash_Write_Bin(FLASH1,Upgrade_Control,'D');
	/*Flash_Write_Bin(FLASH1,Upgrade_Control+1,0);
	Flash_Write_Bin(FLASH1,Upgrade_Control+2,0);
	Flash_Write_Bin(FLASH1,Upgrade_Control+3,0);
	Flash_Write_Bin(FLASH1,Upgrade_Control+4,0);
	Flash_Write_Bin(FLASH1,Upgrade_Control+5,0);
	Flash_Write_Bin(FLASH1,Upgrade_Control+6,0);
	Flash_Write_Bin(FLASH1,Upgrade_Control+7,0);
	*/
	/*Flash_Write(FLASH1,Upgrade_Control+1,tmp_string,7);
	//Flash_Write(FLASH1,Upgrade_Control+1,"00000",5);
	Flash_Write(FLASH1,87,tmp_string,4);
	Param_Init();
	IMMOB_Update();
	RPM_Update();
	User_Init();
	Tag_Init();
	Way_Init();
	Schedule_Init();*/
	//FSM_Init();

}
