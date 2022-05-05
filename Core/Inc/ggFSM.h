/*
 * ggFSM.h
 *
 *  Created on: 22 Feb 2021
 *      Author: munyag
 */

#ifndef GGFSM_H_
#define GGFSM_H_


#define Flash_Version "1.01"

// PARAMETER DEFINES
#define PARAM_START_ADD		100		// START ADDRESS FOR PARAMETERS
#define PARAM_H_Size		5		// HEADER
#define PARAM_F_Size		5		// FOOTER SIZE
#define PARAM_MAX_COUNT 	200		// MAX NUMBER OF ITEM IN PARAMETER TABLE
#define PARAM_SIZE 			53		// PARAMETER ITEM SIZE
#define PARAM_M_SIZE		2		// MARKER SIZE
#define PARAM_N_SIZE		10		// NAME SIZE0
#define PARAM_D_SIZE		40		// DATA SIZE

// SMS DEFINES
#define SMS_Start_ADD	 	PARAM_START_ADD + PARAM_H_Size+ PARAM_F_Size+(PARAM_SIZE*PARAM_MAX_COUNT)
#define SMS_H_Size			5		// HEADER
#define SMS_F_Size			5		// FOOTER SIZE
#define SMS_M_Size			1
#define SMS_Index			3		// INDEX OF THE SMS
#define SMS_Msg_Size		202		// SMS SIZE = DATA(160) + DATE(20) + MARKER(2)
#define SMS_INQ_Size		10
#define SMS_OUTQ_Size		10
#define SMS_S_Inq			SMS_Start_ADD + SMS_H_Size
#define SMS_S_Outq			SMS_S_Inq + ((SMS_Msg_Size) * SMS_INQ_Size)

// DATA DEFINES FOR ALARMS & COMMS to SERVER
#define DATA_Start_ADD 		SMS_Start_ADD + SMS_H_Size + SMS_F_Size + (SMS_Msg_Size*SMS_INQ_Size) + (SMS_Msg_Size*SMS_OUTQ_Size)
#define DATA_H_Size			5		// HEADER
#define DATA_F_Size			5		// FOOTER SIZE
#define DATA_M_Size			1
#define DATA_R_Size			1		// Retry Counter
#define DATA_Msisdn			20		// FALL BACK MSISDN SIZE
#define DATA_Date			20		// DATE INSERTED INTO QUE
#define DATA_SMS_Size		160		// DATA TO SEND
#define DATA_Msg_Size		202
#define DATA_Index			3		// INDEX OF THE SMS
#define DATA_INQ_Size		30
#define DATA_OUTQ_Size		5
#define DATA_S_Inq			DATA_Start_ADD + DATA_H_Size
#define DATA_S_Outq			DATA_S_Inq + (DATA_Msg_Size * DATA_INQ_Size)

// USER DEFINES
#define USR_Start_ADD 		DATA_Start_ADD + DATA_H_Size + DATA_F_Size + (DATA_Msg_Size*DATA_INQ_Size) + (DATA_Msg_Size*DATA_OUTQ_Size)
#define USR_H_Size			5		// HEADER
#define USR_F_Size			5		// FOOTER SIZE
#define USR_Name_Size		20		// USER NAME SIZE MAX 20 BYTES
#define USR_No_Size			20		// USER MSISDN
#define USR_Lvl_Size		2		// USER LEVEL
#define USR_St_Size			1		// STATUS SIZE
#define USR_Index			1
#define USR_RepTime			5		// REPORT TIME
#define USR_RepE			1		// REPORTING ENABLED
#define USR_xyz				2		// CHARACTERS TO CONTROL USER USAGE
#define USR_Size			60
#define USR_Max_Count		20

// HEALTH MESSAGES


void ggFS_Init();


#endif /* GGFSM_H_ */
