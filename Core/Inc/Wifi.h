/*
 * Wifi.h
 *
 *  Created on: Sep 4, 2020
 *      Author: Jeefo
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "stm32f0xx_hal.h"
void Wifi_Init();
void Wifi_Service();
void WifirecData();
void WifiprocData(char* data);
void Wifi_Receive(char in);
void Wifi_Send(char* data);
void WifiOK();
void MQTT_ConnectF1();
void MQTT_SuscribeF1();
void MQTT_Publish_F1();
int byteCopy(uint8_t *source, uint8_t *dest, uint8_t start, uint8_t stop);
void MQTT_Ping_F1();
void MQTT_ProcessF(uint8_t *data);
void MQTT_Publish_F2(uint8_t *data, uint8_t *topic);
typedef enum{
	Wifi_ver = 1,
	Wifi_AT,
	ListAP,
	GetIP,
	CheckConnect,
	WifiConnect,
	WifiAT,
	IP_Stat,
	CPMUX,
	Mode,
	ServerOn,
	ServerOff,
	Dial,
	IP_Send,
	IP_Data,
	IP_Buffer,
	Close_Socket

}Wifi_State;

typedef struct{
	char connect;
	char list;
	char ip;
	char print;
	char server;
}WifiFlag;





#endif /* WIFI_H_ */
