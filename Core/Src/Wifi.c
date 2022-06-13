/*
 * Wifi.c
 *
 *  Created on: Sep 4, 2020
 *      Author: Jeefo
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "GSM.h"
//#include "UART.h"
//#include "LCD.h"
#include "Wifi.h"
#include "MQTT.h"

//uint16_t onTimer = 0;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

#define WrecBuffSize 1300
#define WprocBuffSize 200
char WrecBuff[WrecBuffSize];
char WprocBuff[WprocBuffSize];
char WprocBuff1[WprocBuffSize];
int WrecBuffPointerWrite = 0;
int WrecBuffPointerRead = 0;
int WprocBuffpointer = 0;
Wifi_State WifiState = Wifi_AT;
WifiFlag wifiFlag;
MQTTFlag mqttFlag;
MQTTInfo mqttInfo;
uint8_t dummy[50];
uint8_t ssid[30] = "flying low";
//uint8_t ssid[30] = "TP-Link_5EDC";
uint8_t passwrd[30] = "underthedome";
//uint8_t passwrd[30] = "Danzall123";
uint8_t temp[130];
uint8_t IP1[100];
uint8_t HTML[] = "<!DOCTYPE html><html><head><style>.button {border: none;color: white;padding: 15px 32px;text-align: center;"
		"text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}.button1 {background-color: #4CAF50;} /* Green */"
		".button2 {background-color: #008CBA;} /* Blue */</style></head><body><button class=\"button button1\">Green</button><button class=\"button button2\">Blue</button>"
		"</body></html>";
uint8_t HTML2[] = "<!DOCTYPE html><html><head></head><body><button class=\"button button1\">Green</button><button class=\"button button2\">Blue</button>"
		"</body></html>\r\n";
uint8_t HTML1[] = "<!DOCTYPE html><html><body><button>Dropdown</button></body></html>\r\n";
uint8_t HTML3[] = "HTTP/1.1 200 OK\nContent-type:text/html\r\n\r\nClick\r\n";
uint8_t HTML4[] = "HTTP/1.1 200 OK \nContent-type:text/html\r\n\r\nClick4\r\n";
//uint8_t HTML5[] = "HTTP/1.1 200 OK \nContent-type:text/html\r\n\r\nClick2 <a href=\"/H\">here</a>\r\n";
uint8_t HTML5[] = "HTTP/1.1 200 OK \nContent-type:text/html\r\n\r\nClick <a href=\"/H\">here</a> to change\r\n";
uint8_t HTML6[] = "HTTP/1.1 200 OK \nContent-type:text/html\r\n\r\ns:0,s1:0,s2:0,s3:0,s4:0\r\n";
int len;
uint8_t temp1[300];
uint8_t ConnID[4];
uint8_t mqttReceived[30];
int connCounter;


void Wifi_Init(){
	HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
	Debug_Send("Wifi init\r\n");
	HAL_UART_Receive_IT(&huart2, (uint8_t *) dummy, 40);
	//HAL_UART_Receive_IT(&huart1, (uint8_t *) dummy, 40);
	//__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

	//HAL_UART_Receive(&huart3, dummy, 50, 5);
	//strcpy(quality,"N/A");
	HAL_Delay(2000);
	wifiFlag.connect = 0;
	wifiFlag.list = 0;
	wifiFlag.ip = 0;
	wifiFlag.print = 1;
	wifiFlag.server = 0;
	//WifiState = GetIP;
	Debug_Send("Done\r\n");
	HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_SET);
	//HAL_Delay(4000);
	//WifiState = CPMUX;
	mqttFlag.active = 1;
	mqttInfo.timer = 0;

}

void Wifi_Service(){
	//Wifi_Send("AT\r\n");
	//sprintf(temp, "Wifi service state-%i\r\n", WifiState);
	//GSM_Send(temp);
	//GSM_Send("Wifi service\r\n");
	switch(WifiState){
	case Wifi_ver:
		Wifi_Send("AT+GMR\r\n");
		WifiState = Wifi_AT;
		WifiState = 0;
		break;
	case Wifi_AT:
		Wifi_Send("AT\r\n");
		WifiState = ListAP;
		WifiState = CheckConnect;
		WifiState = Mode;
		WifiState = 0;
		break;
	case Mode:
		Wifi_Send("AT+CWMODE=1\r\n");
		WifiState = CheckConnect;
		break;
	case ListAP:
		Wifi_Send("AT+CWLAP\r\n");
		//WifiState = GetIP;
		wifiFlag.list = 1;

		break;
	case GetIP:
		Wifi_Send("AT+CIFSR\r\n");
		//WifiState = AT;
		//WifiState = ListAP;
		wifiFlag.print = 0;
		//WifiState = CPMUX;
		WifiState = 0;
		break;
	case CheckConnect:
		Wifi_Send("AT+CWJAP?\r\n");
		WifiState = 0;
		//WifiState = ListAP;
		break;

	case WifiConnect:
		strcpy(temp,"AT+CWJAP=");
		strcat(temp,"\"");
		strcat(temp,ssid);
		strcat(temp,"\",\"");
		strcat(temp,passwrd);
		strcat(temp,"\"");
		strcat(temp,"\r\n");
		Wifi_Send(temp);
		WifiState = 0;

		break;
	case WifiAT:
		Wifi_Send("AT\r\n");
		WifiState = 0;
		break;
	case IP_Stat:
		Wifi_Send("AT+CIPSTATUS\r\n");

		break;
	case CPMUX:
		Wifi_Send("AT+CIPMUX=1\r\n");
		WifiState = ServerOn;
		WifiState = Dial;
		if (wifiFlag.server == 0) WifiState = Dial;
		else WifiState = ServerOn;
		break;

	case ServerOn:
		Wifi_Send("AT+CIPSERVER=1,5008\r\n");
		HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);
		WifiState = 0;
		break;
	case ServerOff:
		Wifi_Send("AT+CIPSERVER=0\r\n");
		WifiState = 0;
		break;
	case Dial:
		//Wifi_Send("AT+CIPSTART=\"TCP\",\"iot.espressif.cn\",8000\r\n");
		Wifi_Send("AT+CIPSTART=0,\"TCP\",\"196.40.108.169\",1883\r\n");
		//Wifi_Send("AT+CIPSTART=0,\"TCP\",\"196.40.108.169\",5010\r\n");
		//Wifi_Send("AT+CIPSTART=0,\"TCP\",\"192.168.1.35\",5010\r\n");
		WifiState = 0;
		break;
	case IP_Send:
		//strcpy(temp1,HTML2);
		len = strlen(HTML5);
		if(mqttFlag.active == 1) len = mqttInfo.length;
		myLongStr(len , temp,10, 10);
		//len = 70;
		//Debug_Send("Packet length: ");
		//Debug_Send(temp);
		//Debug_Send("\r\n");
		//Wifi_Send("AT+CIPSEND=<");
		strcpy((char*)temp,"AT+CIPSENDEX=");
		strcat((char*)temp,ConnID);
		strcat((char*)temp,",");
		myLongStr(len,dummy,10,10);
		strcat((char*)temp,dummy);
		strcat((char*)temp,"\r\n");
		Wifi_Send((char*)temp);
		//Wifi_Send(">\r\n");
		//Wifi_Send("AT+CIPSEND=<length>\r\n");
		WifiState = 0;
		break;
	case IP_Data:
		//Debug_Send("Send data\r\n");

		if(mqttFlag.active == 1) GSM_Send_Bin(temp1, mqttInfo.length);
		else{
			strcpy((char*)temp1,HTML5);
			strcat((char*)temp1,"\r\n");
			Debug_Send((char*)temp1);
			Wifi_Send((char*)temp1);
		}

		//Debug_Send("Send data done\r\n");
		WifiState = 0;
		break;
	case IP_Buffer:
		strcpy((char*)temp,"AT+CIPSENDBUF=");
		strcat((char*)temp,ConnID);
		strcat((char*)temp,",");
		myLongStr(len,temp1,10,10);
		strcat((char*)temp,temp1);
		strcat((char*)temp,"\r\n");
		Wifi_Send((char*)temp);

		break;
	case Close_Socket:
		//Wifi_Send("AT+CLOSE\r\n");
		strcpy((char*)temp,"AT+CIPCLOSE=");
		strcat((char*)temp,ConnID);
		strcat((char*)temp,"\r\n");
		Wifi_Send((char*)temp);
		WifiState = 0;
		break;
	}
	if (mqttFlag.subscribe == 2) mqttInfo.timer++;
	if (mqttInfo.timer == 90){
		char tempCount[5];
		//MQTT_Publish_F1();
		//MQTT_Publish_F2("func2","house");
		strcpy(temp,"Count:");
		myLongStr(connCounter,tempCount,10,10);
		strcat(temp,tempCount);
		//strcat(temp,"\r\n");
		MQTT_Publish_F2(temp,"house");
		connCounter++;
		//MQTT_Ping_F1();
		mqttInfo.timer = 0;
	}
}

void Wifi_Send(char* data){
	int size;
	int timeout = 25;
	size = strlen(data);
	HAL_UART_Transmit(&huart2, data, size, timeout);
	HAL_UART_Receive_IT(&huart2, (uint8_t *) dummy, 140);
	//HAL_UART_Transmit_IT(&huart3, data, size);
	//HAL_UART_Receive_IT(&huart1, data, size);
}


void WifirecData(){
	//HAL_GPIO_TogglePin(LED2B0_GPIO_Port, LED2B0_Pin);
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
	//while (WrecBuffPointerWrite != WrecBuffPointerRead){
		if (WrecBuffPointerWrite != WrecBuffPointerRead){

			WprocBuff[WprocBuffpointer] = WrecBuff[WrecBuffPointerRead];
			WrecBuffPointerRead++;
			if (WrecBuffPointerRead > WrecBuffSize) WrecBuffPointerRead = 0;

			WprocBuffpointer++;
			if (WprocBuffpointer > WprocBuffSize) WprocBuffpointer = 0;

			mqttReceived[mqttInfo.Buffpointer] = WprocBuff[WprocBuffpointer - 1];
			if((mqttFlag.connect == 1) && (mqttReceived[mqttInfo.Buffpointer] == 2)){		//check connect response
				Debug_Send("Got connect\r\n");
				mqttFlag.connect = 2;
				MQTT_SuscribeF1();
			}
			if((mqttFlag.subscribe == 1) && (mqttReceived[mqttInfo.Buffpointer] == 0x90)){		//check subscribe response
				Debug_Send("Got subscribe\r\n");
				mqttFlag.subscribe = 2;
				//MQTT_Publish_F1();
				MQTT_Publish_F2("func2","house");
				mqttFlag.data = 1;
				//MQTT_Ping_F1();
			}
			if((mqttFlag.subscribe == 2) && (mqttReceived[mqttInfo.Buffpointer] == 0x30)){
				//Debug_Send("Got publish\r\n");
				//WifiState =  Wifi_AT;
				mqttFlag.data = 1;
			}

			if((mqttFlag.subscribe == 2) && (mqttReceived[mqttInfo.Buffpointer] == 0x31)){
				//Debug_Send("Got publish r\r\n");
				//WifiState =  Wifi_AT;
				mqttFlag.data = 1;
			}

			if (mqttReceived[mqttInfo.Buffpointer] < 10){
				//if ((WprocBuff[WprocBuffpointer - 1] != 0x10)||(WprocBuff[WprocBuffpointer - 1] != 0x13)){
				if (WprocBuff[WprocBuffpointer - 1] != 0x10){
					myLongStr(mqttReceived[mqttInfo.Buffpointer],temp,10,16);
					//Debug_Send("byten: ");
					//Debug_Send(temp);
					//Debug_Send("\r\n");
				}
			}

			if (mqttFlag.data == 1){
				mqttReceived[mqttInfo.Buffpointer] = WprocBuff[WprocBuffpointer - 1];
				/*myLongStr(mqttInfo.Buffpointer,temp,10,10);
				Debug_Send("pos: ");
				Debug_Send(temp);
				Debug_Send(" - ");
				myLongStr(mqttReceived[mqttInfo.Buffpointer],temp,10,10);
				//Debug_Send("byte: ");
				Debug_Send(temp);
				Debug_Send("\r\n");*/
				if (mqttReceived[mqttInfo.Buffpointer] == 0x02){
					Debug_Send("Connected\r\n");
					//mqttState = Subscribe;
					//gsmState = GPRS_Send;
				}
				mqttInfo.Buffpointer++;
				if (mqttInfo.Buffpointer == 2){	//check byte [1]
					mqttInfo.Size = mqttReceived[mqttInfo.Buffpointer - 1];
					//mqttInfo.Size = 8;	//test
					/*myLongStr(mqttInfo.Size,temp,10,10);
					Debug_Send("Size: ");
					Debug_Send(temp);
					Debug_Send("\r\n");
					if (mqttInfo.Size > 30){
						Debug_Send("packet invalid\r\n");
						mqttFlag.Data = 0;
						mqttInfo.Buffpointer = 0;
					}*/
					//if (mqttInfo.Size > 10){
				}
				if (mqttInfo.Buffpointer == 4){	//check when byte 3 arrives
					if((mqttReceived[1] > 30)||(mqttReceived[3] > 30)){
						//Debug_Send("packet invalid,topic or packet size 2 large\r\n");
						mqttFlag.data = 0;
						mqttInfo.Buffpointer = 0;
					}
				}

				if(mqttInfo.Buffpointer>mqttInfo.Size+1){
					mqttFlag.data = 0;
					mqttInfo.Buffpointer = 0;
					//Debug_Send("mqtt stop\r\n");
					MQTT_ProcessF(mqttReceived);
				}
				//if ((MQTTBuffpointer > gsmInfo.MQTT_Size + 2)||(gsmInfo.MQTT_Size == 0)) MQTT_Process();
			}

			if (WprocBuff[WprocBuffpointer - 1] == '>'){
				WifiState = IP_Data;
				/*if (gsmInfo.socket == 1) gsmState = GPRS_Write;
				if (gsmInfo.dataFlag == 1)gsmState = GPRS_Write;
				else gsmState = SMS_Text;*/
			}
			if (WprocBuff[WprocBuffpointer - 1] == 0x0A){
				/*if (mqttFlag.active == 0)*/
				//WprocBuff[WprocBuffpointer - 1] = 0;
				WprocBuff[WprocBuffpointer] = 0;
				WprocBuffpointer = 0;
				//GSM_Send(WprocBuff);
				//GSM_Send("\r\n");
				//GSM_Send("0x0A\r\n");
				//GSM_Send("1");
				//Debug_Send("A\r\n");
				WifiprocData(WprocBuff);
			}
			if (WprocBuff[WprocBuffpointer - 1] == 0x0D){
				//WprocBuff[WprocBuffpointer - 1] = 0;
				//WprocBuff[WprocBuffpointer ] = 0;
				//WprocBuffpointer = 0;
				//GSM_Send(WprocBuff);
				//GSM_Send("\r\n");
				//Debug_Send("D\r\n");
				//HAL_UART_Transmit(&huart1, "D\r\n", 3, 30);
				//HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
				//GSM_Send("proc\r\n");
				//("0x0D\r\n");
				//GSM_Send("0x0D\r\n");
				//GSM_Send("2");
			}
		}
	//}
}
void MQTT_ProcessF(uint8_t *data){
	Debug_Send("MQTT process\r\n");
	/*if (data[3] > 0){
		Debug_Send("got topic length\r\n");
	}
	if (mqttInfo.Size > 4){
		Debug_Send("Valid packet size\r\n");
	}*/
	char cnt[10];
	int counter;
	for (counter = 0; counter < mqttInfo.Size + 2; counter++){
		myLongStr(counter,cnt,10,10);

		//myLongStr(mqttReceived[counter],dummy,10,10);
		myLongStr(*data++,dummy,10,10);
		Debug_Send(cnt);
		Debug_Send(":");
		Debug_Send(dummy);
		//Debug_Send("\r\n");
		Debug_Send("| ");
	}
	mqttInfo.packetLength = mqttReceived[1] ;
	myLongStr(mqttInfo.packetLength,dummy,10,10);
	Debug_Send("\r\npacket size ");
	Debug_Send(dummy);
	Debug_Send("\r\n");
	mqttInfo.topicLength = mqttReceived[3];
	myLongStr(mqttInfo.topicLength,dummy,10,10);
	Debug_Send("topic size ");
	Debug_Send(dummy);
	Debug_Send("\r\n");
	mqttInfo.payloadLength = mqttInfo.packetLength - mqttInfo.topicLength - 2;
	myLongStr(mqttInfo.payloadLength,dummy,10,10);
	Debug_Send("payload size ");
	Debug_Send(dummy);
	Debug_Send("\r\n");
	mqttInfo.packet[0] = 0;
	mqttInfo.topic[0] = 0;
	mqttInfo.data[0] = 0;
	if ((mqttFlag.subscribe == 2)&&(mqttInfo.topicLength > 3)){	//3 to test
		byteCopy(mqttReceived, mqttInfo.packet, 4, 1 + mqttInfo.packetLength);
		byteCopy(mqttReceived, mqttInfo.topic, 4, mqttInfo.topicLength + 3);
		byteCopy(mqttReceived, mqttInfo.data, 4 + mqttInfo.topicLength, 1 + mqttInfo.packetLength);
		Debug_Send(mqttInfo.packet);
		Debug_Send(" ^ ");
		Debug_Send(mqttInfo.topic);
		Debug_Send(" ^ ");
		Debug_Send(mqttInfo.data);
		Debug_Send("\r\n");
	}
	if(strncmp(mqttInfo.data,"l1",2) == 0){
		myStrSection(mqttInfo.data, temp,30,',',1);	//get parameter
		if(strncmp(temp,"off",3) == 0){
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
			Debug_Send("Light 1 off\r\n");
		}
		if(strncmp(temp,"on",2) == 0){
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			Debug_Send("Light 1 on\r\n");
		}
	}
	if(strncmp(mqttInfo.data,"l2",2) == 0){
		myStrSection(mqttInfo.data, temp,30,',',1);	//get parameter
		if(strncmp(temp,"off",3) == 0){
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
			Debug_Send("Light 2 off\r\n");
		}
		if(strncmp(temp,"on",2) == 0){
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);
			Debug_Send("Light 2 on\r\n");
		}
	}
}

void MQTT_ConnectF1(){
	Debug_Send("Build connect msg\r\n");
	temp1[0] = 0x10;
	temp1[1] = 0x0E;
	temp1[2] = 0x00;
	temp1[3] = 0x04;
	temp1[4] = 'M';
	temp1[5] = 'Q';
	temp1[6] = 'T';
	temp1[7] = 'T';
	temp1[8] = 0x04;
	temp1[9] = 0x02;
	temp1[10] = 0x00;
	temp1[11] = 0x78;
	temp1[12] = 0x00;
	temp1[13] = 0x02;

	temp1[14] = 'P';
	temp1[15] = '2';

	mqttInfo.length = 16;
	WifiState = IP_Send;
	mqttFlag.connect = 1;
	mqttFlag.send = 1;
	//GSM_Send_Bin(temp1, 16);
}

void MQTT_SuscribeF1(){
	Debug_Send("Build subscribe msg\r\n");
	temp1[0] = 0x82;
	temp1[1] = 0x0A;
	temp1[2] = 0x00;
	temp1[3] = 0x00;
	temp1[4] = 0x00;
	temp1[5] = 0x05;
	temp1[6] = 'h';
	temp1[7] = 'o';
	temp1[8] = 'u';
	temp1[9] = 's';
	temp1[10] = 'e';
	temp1[11] = 0x00;
	temp1[12] = 0x00;

	mqttInfo.length = 13;
	WifiState = IP_Send;
	mqttFlag.subscribe = 1;
	mqttFlag.send = 1;
	//GSM_Send_Bin(tempGPRS, 13);
}

void MQTT_Publish_F1(){
	Debug_Send("Build publish msg\r\n");
	temp1[0] = 0x30;
	temp1[1] = 0x0B;
	temp1[2] = 0x00;
	temp1[3] = 0x05;
	temp1[4] = 'h';
	temp1[5] = 'o';
	temp1[6] = 'u';
	temp1[7] = 's';
	temp1[8] = 'e';
	temp1[9] = 'o';
	temp1[10] = 'f';
	temp1[11] = 'f';
	temp1[12] = '1';

	mqttInfo.length = 13;
	WifiState = IP_Send;
	mqttFlag.publish = 1;
	mqttFlag.send = 1;
	//GSM_Send_Bin(temp1, 14);
}

void MQTT_Publish_F2(uint8_t *data, uint8_t *topic){
	Debug_Send("Publish Func\n");
	int counter;
	temp1[0] = 0x30;
	temp1[1] = 0x0A;
	temp1[2] = 0x00;
	temp1[3] = 0x05;
	temp1[4] = 'h';
	temp1[5] = 'o';
	temp1[6] = 'u';
	temp1[7] = 's';
	temp1[8] = 'e';
	temp1[9] = 'o';
	temp1[10] = 'f';
	temp1[11] = 'f';

	counter = 4;
	while(*topic != 0){
		temp1[counter] = *topic++;
		counter++;
	}


	//counter = 9;
	while(*data != 0){
		temp1[counter] = *data++;
		counter++;
	}

	myLongStr(counter,temp,10,10);
	Debug_Send("Chars added:");
	Debug_Send(temp);
	Debug_Send("\r\n");

	mqttInfo.length = counter;
	/*tempGPRS[counter] = 0x1A;
	counter++;
	tempGPRS[counter] = 0x0D;
	counter++;*/
	WifiState = IP_Send;
	mqttFlag.publish = 1;
	mqttFlag.send = 1;
	//GSM_Send_Bin(tempGPRS, 14);
	//mqttInfo.length = 14;
	//mqttInfo.length = counter;
	temp1[1] = counter - 2;
	Debug_Send("Publish sent\r\n");
	/*myLongStr(mqttInfo.length,temp1,10,10);
	Debug_Send("Length:");
	Debug_Send(temp1);
	Debug_Send("\r\n");*/
}

void MQTT_Ping_F1(){
	Debug_Send("Build ping msg\r\n");
	temp1[0] = 0xC0;
	temp1[1] = 0x00;

	mqttInfo.length = 2;
	WifiState = IP_Send;
	//mqttFlag.publish = 1;
	mqttFlag.send = 1;
	//GSM_Send_Bin(temp1, 14);
}

void SendData(char * data){

}

void WifiprocData(char* data){
	//Debug_Send("Process Wifi msg\r\n");
	strcpy(temp,"Rx:");
	strcat(temp,data);
	//strcat(temp,"\r\n");
	Debug_Send(temp);

	myStrSection(WprocBuff, temp,30,',',1);
	if(strncmp((char*)temp,"CONNECT",7)==0){			//socket connected
		myStrSection(WprocBuff, ConnID,30,',',0);
		sprintf(temp, "Conn %s connected\r\n",ConnID);
		Debug_Send(temp);
		wifiFlag.connect = 1;
		if (wifiFlag.server == 0){
			MQTT_ConnectF1();
		}
	}
	if(strncmp((char*)temp,"CLOSED",6)==0){			//socket closed
		myStrSection(WprocBuff, ConnID,30,',',0);
		sprintf(temp, "Conn %s closed\r\n",ConnID);
		Debug_Send(temp);
		wifiFlag.connect = 0;
		mqttFlag.subscribe = 0;
		WifiState = Dial;
		if (wifiFlag.server == 0){
			//MQTT_ConnectF1();
		}
	}
	//GSM_Send(WprocBuff);
	//GSM_Send("\r\n");
	if(strncmp((char*)WprocBuff,"+CREG: 0,1",10)==0)HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	if(strncmp((char*)WprocBuff,"OK",2)==0) WifiOK();
	//if(strncmp((char*)WprocBuff,"Ai",2)==0) WifiState = Wifi_version;
	//if(strncmp((char*)WprocBuff,"No AP",5)==0) WifiState = Connect;

	//if(strncmp((char*)WprocBuff,"WIFI CONNECTED",14)==0) WifiState = Connect;	//connected to AP
	if(strncmp((char*)WprocBuff,"WIFI GOT IP",10)==0) WifiState = GetIP;		//connected to AP
	if(strncmp((char*)WprocBuff,"no change",9)==0) WifiState = 0;		//server running

	if(strncmp((char*)WprocBuff,"+CWJAP:",7)==0){
		myStrSection(WprocBuff, temp,30,'"',1);
		if(strncmp(temp,ssid,5)==0) WifiState = GetIP;

	}
	if(strncmp((char*)WprocBuff,"+CIFSR:STAIP",12)==0){
		//GSM_Send(WprocBuff);
		//GSM_Send("\r\n");
		myStrSection(WprocBuff, IP1,30,'"',1);
		wifiFlag.print = 1;
		//strcpy(WprocBuff1,WprocBuff);
		/*if (wifiFlag.ip == 0){
			GSM_Send("Copy1\r\n");
			strcpy(IP1,WprocBuff);
		}
		if (wifiFlag.ip == 1){
			GSM_Send("Copy2\r\n");
			strcpy(IP2,WprocBuff);
		}
		if (wifiFlag.ip == 2){
			GSM_Send("Copy3\r\n");
			strcpy((char*)IP3,WprocBuff);
		}
		if (wifiFlag.ip == 3){
			GSM_Send("Copy4\r\n");
			strcpy(IP4,WprocBuff);
			//
		}*/
		wifiFlag.ip = 1;
	}
	if(strncmp((char*)WprocBuff,"+IPD",3)==0){
		Debug_Send("Got IPD\r\n");
		char len[10];
		int ln1;
		int start = 9;
		int counter;
		myStrSection(WprocBuff, len,30,',',2);		//returned length
		if (len[1] == ':') len[1] = 0;
		else{
			len[2] = 0;
			start = 10;
		}
		//mqttInfo.length = atoi(temp);
		ln1 =  myStrLong(len,10);
		if(mqttFlag.active == 0) myStrSection(WprocBuff, temp,30,':',1);
		else{
			byteCopy(WprocBuff, temp, start, start + ln1);
		}
		myStrSection(WprocBuff, ConnID,30,',',1);
		//sprintf(temp, "%s,%i,%i\r\n",WprocBuff, WrecBuffPointerWrite, WrecBuffPointerRead);
		//GSM_Send(temp);
		//WifiState = IP_Send;
		/*char cnt[10];
		for (counter = 0; counter < ln1; counter++){
			myLongStr(counter,cnt,10,10);

			myLongStr(temp[counter],dummy,10,10);
			Debug_Send(cnt);
			Debug_Send(":");
			Debug_Send(dummy);
			//Debug_Send("\r\n");
			Debug_Send(" ");
		}

		if(temp[0] == 0x30){
			Debug_Send("Got publ\r\n");
			WifiState =  Wifi_AT;
		}
		if(temp[0] == 0x31){
			Debug_Send("Got publ r\r\n");
			WifiState =  Wifi_AT;
		}
		mqttInfo.packetLength = temp[1] ;
		myLongStr(mqttInfo.packetLength,dummy,10,10);
		Debug_Send("packet size ");
		Debug_Send(dummy);
		Debug_Send("\r\n");
		mqttInfo.topicLength = temp[3];
		myLongStr(mqttInfo.topicLength,dummy,10,10);
		Debug_Send("topic size ");
		Debug_Send(dummy);
		Debug_Send("\r\n");
		mqttInfo.packet[0] = 0;
		mqttInfo.topic[0] = 0;
		mqttInfo.data[0] = 0;
		if (mqttFlag.subscribe == 2){	//3 to test
			byteCopy(temp, mqttInfo.packet, 4, 1 + mqttInfo.packetLength);
			byteCopy(temp, mqttInfo.topic, 4, mqttInfo.topicLength + 3);
			byteCopy(temp, mqttInfo.data, 4 + mqttInfo.topicLength, 1 + mqttInfo.packetLength);
			Debug_Send(mqttInfo.packet);
			Debug_Send(" - ");
			Debug_Send(mqttInfo.topic);
			Debug_Send(" - ");
			Debug_Send(mqttInfo.data);
			Debug_Send("\r\n");
		}*/
		/**/

		/*myStrSection(WprocBuff, temp,30,' ',1);	//get parameter
		if(strncmp(temp,"/H",2) == 0){
			Debug_Send("Got click\r\n");
			HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		}
		if(strncmp(mqttInfo.data,"l1",2) == 0){
			myStrSection(mqttInfo.data, temp,30,',',1);	//get parameter
			if(strncmp(temp,"off",3) == 0){
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
				Debug_Send("Light off\r\n");
			}
			if(strncmp(temp,"on",2) == 0){
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
				Debug_Send("Light off\r\n");
			}
		}*/
		Debug_Send("Processed IPD\r\n");
		//int hlen;
		//hlen = strlen(HTML);
		//sprintf(temp, "Conn %s received:%s\r\n",ConnID, temp1);
		//GSM_Send(temp);
		wifiFlag.print = 0;
		mqttFlag.receive = 0;
	}
	if (wifiFlag.print == 1){
		//sprintf(temp, "%s,%i,%i\r\n",WprocBuff, WrecBuffPointerWrite, WrecBuffPointerRead);
		//GSM_Send(temp);
	}
	if(strncmp((char*)WprocBuff,"Accept",6)==0){
		//Debug_Send("Processed IPD\r\n");
		//WifiState = IP_Send;
	}
	wifiFlag.print = 1;
	//if(strncmp((char*)WprocBuff,"busy s...",9)==0) wifiFlag.print = 1;	//BUSY SENDING DATA
	//if(strncmp((char*)WprocBuff,"Recv 5 bytes",4)==0) wifiFlag.print = 1;	//DATA SENT
	if(strncmp((char*)WprocBuff,"SEND OK",7)==0){
		Debug_Send("SUCCESSFULLY SENT DATA\r\n");
		//wifiFlag.print = 1;	//SEND DONE
		//Debug_Send("Close socket\r\n");
		//mqttFlag.Data = 1;
		if (mqttFlag.send == 1){
			//Debug_Send("Flush data\r\n");
			WifiState =  Wifi_AT;
			mqttFlag.send = 0;
			mqttFlag.receive = 1;
			WifiState =  0;
		}
		//if (mqttFlag.active == 0) WifiState =  Close_Socket;
	}
	if(strncmp((char*)WprocBuff,"link is not valid",17)==0){
		Debug_Send("Link error\r\n");
		wifiFlag.connect = 0;
		mqttFlag.subscribe = 0;
		WifiState = Dial;
	}
	if(strncmp((char*)WprocBuff,"ERROR",5)==0){
		Debug_Send("error\r\n");
		wifiFlag.connect = 0;
		mqttFlag.subscribe = 0;
		WifiState = Dial;
		Wifi_Init();
	}
}

int byteCopy(uint8_t *source, uint8_t *dest, uint8_t start, uint8_t stop){
	 int val;
	 val = -1;
	 if (start > stop){
		 Debug_Send("Index error\r\n");
		 return -1;
	 }
	 uint8_t difference;
	 uint8_t count;
	 uint8_t* source1;
	 difference = stop - start + 1;
	 source += start;
	 source1 = source;
	 source1 += start;
	 for (count = 0; count < difference; count++){
		 *dest = *source;
		 dest++;
		 source++;
	 }
	 dest++;
	 *dest = 0;
	 //Debug_Send(dest);
	 //Debug_Send("\r\n");
	 val = 1;
	 return val;
}

void WifiOK(){
	//sprintf(temp, "%s,%i,%i,%i\r\n",WprocBuff, WrecBuffPointerWrite, WrecBuffPointerRead,wifiFlag.ip);
	//GSM_Send(temp);
	if(wifiFlag.list == 1){
		wifiFlag.list = 0;
		WifiState = GetIP;
	}
	if (wifiFlag.ip == 1){
		wifiFlag.ip = 0;
		Debug_Send(IP1);
		Debug_Send("\r\n");
		myStrSection(IP1, temp,30,'.',0);
		if (strncmp(temp,"0",1) != 0){
			wifiFlag.ip = 2;
			WifiState = CPMUX;
			Debug_Send("IP found\r\n");
		}
		else{
			Debug_Send("IP not found\r\n");
			//WifiState = Connect;
		}
		/*GSM_Send(IP2);
		GSM_Send("\r\n");
		GSM_Send(IP3);
		GSM_Send("\r\n");
		GSM_Send(IP4);
		GSM_Send("\r\n");*/
	}
	//Debug_Send("Got OK\r\n");
}
void Wifi_Receive(char in){
	//
	WrecBuff[WrecBuffPointerWrite] = (char)in;
	//HAL_UART_Transmit(&huart1, recBuff[recBuffPointerWrite], 2, 10);
	//huart1.Instance->TDR = in;
	WrecBuffPointerWrite++;
	if (WrecBuffPointerWrite > WrecBuffSize) WrecBuffPointerWrite = 0;

}
