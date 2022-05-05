/*
 * GSM.h
 *
 *  Created on: Apr 24, 2019
 *      Author: Jeefo
 */

#ifndef GSM_H_
#define GSM_H_

void GSM_Service();
void GSM_Send(char* data);
void Debug_Send(char* data);
void GSM_Init();
void sendSMS(char* num, char* msg);
void GSM_Receive(char in);
void recData();
void procData();
void OK();
void Context();
void checkGPRS();
void procSMS();
void buildInfo();
void buildTest();
void getSignal();
void caps(char* str);
void Error();
void pec_Update(char* pec, char index);
void GPRS_SendData(char* gprs);
void Socket1();
void DataStatus();
void DataDecrypt();
int Getvalue(char in);
void getIMEI();
void getCCID();
void Register();
void VoltageAlarm();
void Call();
void Balance();
void Network();
//void alarmFunc(Alarm input);
void Deny();
void GSM_Init();
char CheckUser();
void RegisterConfirm();
void SendChar(char data);
void BuildPower();
void recData();
void FTP_OpenF();
void FTP_StatusF();
void FS_Open_F();
void FileSize();
void Read();
void Seek();
void getFtp();
void GSM_Send_Bin(char* data, int size);
void MQTT_Process();
void MQTT_MQTT_ConnectF();
void MQTT_SuscribeF();
void MQTT_Publish_F();
typedef enum{
	GSM_Off = 1,
	GSM_On,
	GSM_Enable,
	Search,
	GPRS_Search,
	SMSconfig,
	Operator,
	Imei,
	Ccid,
	GprsContext,
	USSD_Config,
	USSD,
	CallerID,
	SMS_Check,
	SMS_Send,
	SMS_Text,
	GPRS_Attach,
	GPRS_On,
	GPRS_IP,
	GPRS_Off,
	SocketOpen,
	SocketClose,
	SMS_Del,
	Signal,
	SocketListen,
	SocketCheck,
	GPRS_Write,
	GPRS_Send,
	GPRS_SendMode,
	GPRS_SendModeTRans,
	AutoBaud,
	DataMode,
	GPRS_Text,
	FTP_User,
	FTP_Password,
	FTP_Open,
	FTP_Status,
	FTP_Close,
	FTP_Download,
	FTP_Size,
	FTP_List,
	FTP_Config,
	FTP_Path,
	FS_Storage,
	FS_SList,
	FS_Download,
	FS_Seek,
	FS_Read,
	FS_Open,
	FS_Close,
	FlashWrite,
	MQTT_Connect,
	MQTT_Subscribe,
	AT
}State;

typedef enum{
	Connect = 1,
	Subscribe,
	Publish,
	Unsubscribe,
}MQTT_State;

typedef struct{
	int signal;
	int config;
	int send;
	int prompt;
	int content;
	int context;
	int gprsPending;
	int gprsActive;
	int del;
	int reply;
	int restartCount;
	int socket;
}SMSflags;

typedef struct{
	char index[4];
	char recMSISDN[25];
	int prompt;
	char content1[180];
	char IP[30];
	int count;
	char build[160];
}SMSinfo;

typedef struct{
	char url[30];
	char urlport[6];
	char imei[40];
	char ccid[40];
	int socket1;
	//int socket2;
	//int socket3;
	//int socket4;
	int socket;
	int registered;
	int gprsDataPending;
	int userFlag;
	int msisdnFlag;
	int passwordFlag;
	int alarm;
	char user[15];
	char user1[15];
	//char user2[15];
	char msisdn[20];
	char password[15];
	char network[25];
	char balance[15];
	int low;
	char vth[10];
	int vth_l;
	int vthFlag;
	int relayFlag;
	int ccidFlag;
	int activeFlag;
	int updateFlag;
	int modeFlag;
	int vThreshold;
	int ccidStatus;
	int balanceFlag;
	int balancePending;
	int GPRStimer;
	int GPRSinterval;
	char GPRSint[10];
	int imeiFlag;
	int dataFlag;
	int GPRS_Rec;
	int Prompt;
	int Timeout;
	int FTP_Open;
	int FS_SeekFlag;
	int FS_Connect;
	int FTP_Data;
	int FTP_Save_Addr;
	int FTP_Size;
	int MQTT;
	int MQTT_Data;
	int MQTT_Size;
	int MQTT_Subscribe;
}GSMinfo;

typedef void (*fPointer) (void);
#endif /* GSM_H_ */
