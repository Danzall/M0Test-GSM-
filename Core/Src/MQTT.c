/*
 * MQTT.c
 *
 *  Created on: Jun 9, 2022
 *      Author: Danzall
 */

#include "main.h"
#include "cmsis_os.h"
#include "MQTT.h"
#include "Wifi.h"

MQTTFlag mqttFlag;
MQTTInfo mqttInfo;
MQTT_State mqttState1;
char tempGPRS[30];

MQTTFlag* MQTT_Service(uint8_t data, uint32_t state){
	int val = 0;
	val = state;
	mqttFlag.send = 0;
	if (data == 0x02){
		Debug_Send("conn new\r\n");
		mqttFlag.data = 1;
		mqttInfo.Buffpointer = 0;


	}
	if (data == 0x90){
		Debug_Send("subscribed new!!!!\r\n");
		mqttInfo.Buffpointer = 0;
		mqttFlag.data = 1;
		mqttFlag.subscribe = 1;
		mqttState1 = Publish;
		mqttFlag.state = Publish;
		val = Publish;
	}
	if ((data == '1')&&(mqttFlag.subscribe == 1)&&(mqttFlag.busy == 0)){
		//Debug_Send("pusblish retain!!!\r\n");
		mqttFlag.data = 1;
		mqttInfo.Buffpointer = 0;
		mqttFlag.busy = 1;
	}
	if ((data == '0')&&(mqttFlag.subscribe == 1)&&(mqttFlag.busy == 0)){
		//Debug_Send("pusblish normal!!!\r\n");
		mqttFlag.data = 1;
		mqttInfo.Buffpointer = 0;
		mqttFlag.busy = 1;
	}
	char temp1[20];
	if (mqttFlag.data == 1){
		mqttInfo.packet[mqttInfo.Buffpointer] = data;
		/*myLongStr(mqttInfo.Buffpointer,temp1,10,10);
		Debug_Send("pos n: ");
		Debug_Send(temp1);
		//Debug_Send("\r\n");
		myLongStr(mqttInfo.packet[mqttInfo.Buffpointer],temp1,10,10);
		Debug_Send(" ");
		Debug_Send(temp1);
		Debug_Send("\r\n");*/
		if ((mqttInfo.packet[mqttInfo.Buffpointer] == 0x02)&&(mqttFlag.connect == 2)){
			Debug_Send("Connected new\r\n");
			mqttState1 = Subscribe;
			mqttFlag.state = Subscribe;
			mqttFlag.connect = 3;
			val = Subscribe;

			//gsmState = GPRS_Send;
		}
		if (mqttInfo.Buffpointer == 1){
			mqttInfo.Size = mqttInfo.packet[mqttInfo.Buffpointer];
			/*myLongStr(mqttInfo.Size,temp1,10,10);
			Debug_Send("Size n: ");
			Debug_Send(temp1);
			Debug_Send("\r\n");*/
		}

		if (mqttInfo.Buffpointer == 4){	//check when byte 3 arrives
			//Debug_Send("check packet validity n\r\n");
			if((mqttInfo.packet[1] > 30)||(mqttInfo.packet[3] > 30)){
				//Debug_Send("packet invalid,topic or packet size 2 large n\r\n");
				//mqttFlag.Data = 0;
				mqttFlag.data = 0;
				//mqttInfo.Buffpointer = 0;
				mqttInfo.Buffpointer = 0;
			}
			if((mqttInfo.packet[1] > 30)||(mqttInfo.packet[3] > 30)){

			}
		}

		if(mqttInfo.Buffpointer>mqttInfo.Size){
			if ((mqttInfo.packet[0] == 2)&&(mqttInfo.Size == 0)){
				//Debug_Send("Confirmed connect\r\n");
				val = 2;
				mqttFlag.send = 1;
			}
			mqttInfo.Buffpointer = 0;
			//Debug_Send("mqtt stop new\r\n");
			mqttFlag.busy = 0;
			if ((mqttFlag.subscribe == 1)&&(mqttInfo.Size > 3)) MQTT_ProcessF1("tst");
		}
		mqttInfo.Buffpointer++;
		//if ((MQTTBuffpointer > gsmInfo.MQTT_Size + 2)||(gsmInfo.MQTT_Size == 0)) MQTT_Process();
	}
	return &mqttFlag;
}

void MQTT_ProcessF1(uint8_t *data){
	Debug_Send("Process MQTT new\r\n");
	int counter;
	char cnt[10];
	int start = 4;
	int len;
	char temp[30];
	char temp1[30];
	for (counter = 0; counter < mqttInfo.Size; counter++){
		myLongStr(counter,cnt,10,10);

		myLongStr(mqttInfo.packet[mqttInfo.Buffpointer],temp,10,10);
		Debug_Send(cnt);
		Debug_Send(":");
		Debug_Send(temp);
		//Debug_Send("\r\n");
		Debug_Send(" ");
		mqttInfo.Buffpointer++;
	}
	len = mqttInfo.Size;
	//byteCopy(mqttReceived, temp, start, start + len);
	byteCopy(mqttInfo.packet, temp, start, len);
	Debug_Send("\r\nPacket:");
	Debug_Send(temp);
	Debug_Send("\r\n");
	mqttInfo.packetLength = mqttInfo.packet[1] ;
	myLongStr(mqttInfo.packetLength,temp1,10,10);
	Debug_Send("packet size ");
	Debug_Send(temp1);
	Debug_Send("\r\n");
	mqttInfo.topicLength = mqttInfo.packet[3];
	myLongStr(mqttInfo.topicLength,temp1,10,10);
	Debug_Send("topic size ");
	Debug_Send(temp1);
	Debug_Send("\r\n");
	myLongStr(mqttInfo.packetLength - mqttInfo.topicLength - 2,temp1,10,10);
	Debug_Send("msg size ");
	Debug_Send(temp1);
	Debug_Send("\r\n");
	mqttInfo.packet[0] = 0;
	mqttInfo.topic[0] = 0;
	mqttInfo.data[0] = 0;
	if ((mqttFlag.subscribe == 1)&&(mqttInfo.topicLength > 2)){
		byteCopy(mqttInfo.packet, temp1, 4, mqttInfo.packetLength + 1);
		byteCopy(mqttInfo.packet, mqttInfo.topic, 4, mqttInfo.topicLength + 3);
		byteCopy(mqttInfo.packet, mqttInfo.data, 4 + mqttInfo.topicLength, mqttInfo.packetLength + 1);
		//strncpy(mqttInfo.packet,)
		Debug_Send(temp1);
		Debug_Send(" - ");
		Debug_Send(mqttInfo.topic);
		Debug_Send(" - ");
		Debug_Send(mqttInfo.data);
		Debug_Send("\r\n");
		if (strncmp(mqttInfo.data,"mag",3) == 0){
			Debug_Send("Got mag\r\n");
			//mqttState = Publish;
			//gsmState = GPRS_Send;
			//MQTT_Publish_F("got mag");
		}
		if (strncmp(mqttInfo.data,"sync",3) == 0){
			//MQTT_Publish_F("got sync");
		}
		int ln2;
		ln2 = mqttInfo.packetLength - mqttInfo.topicLength - 2;
		if (strncmp(mqttInfo.data,"l1",2) == 0){
			strncpy(temp1,mqttInfo.data,ln2);
			myStrSection(temp1,temp,10,',',1);
			Debug_Send(temp);
			Debug_Send("\r\n");
			if(strncmp(temp,"on",2) == 0){
				//MQTT_Publish_F("got light on");
				RelayOn();
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED3_Pin,GPIO_PIN_RESET);
			}
			if(strncmp(temp,"off",3) == 0){
				//MQTT_Publish_F("got light off");
				RelayOff();
				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin,GPIO_PIN_SET);
			}
		}
		if (strncmp(mqttInfo.data,"l2",2) == 0){
			strncpy(temp1,mqttInfo.data,ln2);
			myStrSection(temp1,temp,10,',',1);
			Debug_Send(temp);
			Debug_Send("\r\n");
			if(strncmp(temp,"on",2) == 0){
				//MQTT_Publish_F("got light on");
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_RESET);
			}
			if(strncmp(temp,"off",3) == 0){
				//MQTT_Publish_F("got light off");
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_SET);
			}
		}
		if (strncmp(mqttInfo.data,"l3",2) == 0){
			strncpy(temp1,mqttInfo.data,ln2);
			myStrSection(temp1,temp,10,',',1);
			Debug_Send(temp);
			Debug_Send("\r\n");
			if(strncmp(temp,"on",2) == 0){
				//MQTT_Publish_F("got light on");
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_RESET);
			}
			if(strncmp(temp,"off",3) == 0){
				//MQTT_Publish_F("got light off");
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_SET);
			}
		}
		if (strncmp(mqttInfo.data,"l4",2) == 0){
			strncpy(temp1,mqttInfo.data,ln2);
			myStrSection(temp1,temp,10,',',1);
			Debug_Send(temp);
			Debug_Send("\r\n");
			if(strncmp(temp,"on",2) == 0){
				//MQTT_Publish_F("got light on");
				HAL_GPIO_WritePin(Sensor1_GPIO_Port, Sensor1_Pin,GPIO_PIN_RESET);
			}
			if(strncmp(temp,"off",3) == 0){
				//MQTT_Publish_F("got light off");
				HAL_GPIO_WritePin(Sensor1_GPIO_Port, Sensor1_Pin,GPIO_PIN_SET);
			}
		}
		if (strncmp(mqttInfo.data,"l5",2) == 0){
			strncpy(temp1,mqttInfo.data,ln2);
			myStrSection(temp1,temp,10,',',1);
			Debug_Send(temp);
			Debug_Send("\r\n");
			if(strncmp(temp,"on",2) == 0){
				//MQTT_Publish_F("got light on");
				HAL_GPIO_WritePin(Sensor2_GPIO_Port, Sensor2_Pin,GPIO_PIN_RESET);
			}
			if(strncmp(temp,"off",3) == 0){
				//MQTT_Publish_F("got light off");
				HAL_GPIO_WritePin(Sensor2_GPIO_Port, Sensor2_Pin,GPIO_PIN_SET);
			}
		}
		if (strncmp(mqttInfo.data,"l6",2) == 0){
			strncpy(temp1,mqttInfo.data,ln2);
			myStrSection(temp1,temp,10,',',1);
			Debug_Send(temp);
			Debug_Send("\r\n");
			if(strncmp(temp,"on",2) == 0){
				//MQTT_Publish_F("got light on");
				HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin,GPIO_PIN_RESET);
			}
			if(strncmp(temp,"off",3) == 0){
				//MQTT_Publish_F("got light off");
				HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin,GPIO_PIN_SET);
			}
		}
		if (strncmp(mqttInfo.data,"retain",3) == 0){
			//MQTT_Publish_F("got retain");
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin,GPIO_PIN_RESET);
		}
		if (strncmp(mqttInfo.data,"mag",3) == 0){

		}
		if (strncmp(mqttInfo.data,"mag",3) == 0){

		}
	}
	mqttFlag.data = 0;
}

void MQTT_ConnectF(){
	tempGPRS[0] = 0x10;
	tempGPRS[1] = 0x0E;
	tempGPRS[2] = 0x00;
	tempGPRS[3] = 0x04;
	tempGPRS[4] = 'M';
	tempGPRS[5] = 'Q';
	tempGPRS[6] = 'T';
	tempGPRS[7] = 'T';
	tempGPRS[8] = 0x04;
	tempGPRS[9] = 0x02;
	tempGPRS[10] = 0x0A;
	tempGPRS[11] = 0x78;
	tempGPRS[12] = 0x00;
	tempGPRS[13] = 0x02;

	tempGPRS[14] = 'P';
	tempGPRS[15] = '3';

	tempGPRS[16] = 0x1A;
	tempGPRS[17] = 0x0D;
	GSM_Send_Bin(tempGPRS, 18);
	mqttFlag.connect = 1;
	//mqttState = Connect;
	//gsmState = GPRS_Send;
}

void MQTT_SuscribeF(){
	tempGPRS[0] = 0x82;
	tempGPRS[1] = 0x0A;
	tempGPRS[2] = 0x00;
	tempGPRS[3] = 0x00;
	tempGPRS[4] = 0x00;
	tempGPRS[5] = 0x05;
	tempGPRS[6] = 'h';
	tempGPRS[7] = 'o';
	tempGPRS[8] = 'u';
	tempGPRS[9] = 's';
	tempGPRS[10] = 'e';
	tempGPRS[11] = 0x00;
	tempGPRS[12] = 0x00;

	tempGPRS[13] = 0x1A;
	tempGPRS[14] = 0x0D;
	GSM_Send_Bin(tempGPRS, 15);
	//mqttState = Subscribe;
	//gsmState = GPRS_Send;
}

void MQTT_Publish_F(uint8_t *data){
	//Debug_Send("Publish Func\n");
	//Debug_Send(data);
	//Debug_Send("\r\n");
	tempGPRS[0] = 0x30;
	tempGPRS[1] = 0x0A;
	tempGPRS[2] = 0x00;
	tempGPRS[3] = 0x05;
	tempGPRS[4] = 'h';
	tempGPRS[5] = 'o';
	tempGPRS[6] = 'u';
	tempGPRS[7] = 's';
	tempGPRS[8] = 'e';
	tempGPRS[9] = 'o';
	tempGPRS[10] = 'f';
	tempGPRS[11] = 'f';

	int counter;
	counter = 9;
	while(*data != 0){
		tempGPRS[counter] = *data++;
		counter++;
	}
	char temp1[20];
	myLongStr(counter,temp1,10,10);
	Debug_Send("Chars count:");
	Debug_Send(temp1);
	Debug_Send("\r\n");


	tempGPRS[counter] = 0x1A;
	counter++;
	tempGPRS[counter] = 0x0D;
	counter++;

	//GSM_Send_Bin(tempGPRS, 14);
	//mqttInfo.length = 14;
	mqttInfo.length = counter;
	tempGPRS[1] = counter - 4;
	GSM_Send_Bin(tempGPRS, counter);
	Debug_Send("Publish sent\r\n");
	/*myLongStr(mqttInfo.length,temp1,10,10);
	Debug_Send("Length:");
	Debug_Send(temp1);
	Debug_Send("\r\n");*/
}

void MQTT_Ping_F(){
	Debug_Send("Build ping msg\r\n");
	tempGPRS[0] = 0xC0;
	tempGPRS[1] = 0x00;

	mqttInfo.length = 2;
	tempGPRS[2] = 0x1A;
	tempGPRS[3] = 0x0D;
	GSM_Send_Bin(tempGPRS, 4);
	//WifiState = IP_Send;
	//mqttFlag.publish = 1;
	//mqttFlag.send = 1;
	//GSM_Send_Bin(temp1, 14);
}
