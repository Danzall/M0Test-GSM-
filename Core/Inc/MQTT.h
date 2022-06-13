/*
 * MQTT.h
 *
 *  Created on: Jun 9, 2022
 *      Author: Danzall
 */

#ifndef INC_MQTT_H_
#define INC_MQTT_H_

typedef enum{
	Connect = 1,
	Subscribe,
	Publish,
	Unsubscribe,
	Ping
}MQTT_State;

typedef struct{
	char data;
	char subscribe;
	char active;
	char connect;
	char send;
	char receive;
	char publish;
	char busy;
	MQTT_State state;
}MQTTFlag;

typedef struct{
	char Size;
	int Buffpointer;
	int length;
	int packetLength;
	int topicLength;
	int payloadLength;
	char packet[50];
	char topic[20];
	char data[20];
	int32_t timer;
}MQTTInfo;

typedef struct{
	uint32_t state;
};

void MQTT_ConnectF();
void MQTT_SuscribeF();
void MQTT_Publish_F(uint8_t *data);
void MQTT_Ping_F();
MQTTFlag* MQTT_Service(uint8_t data, uint32_t state);
void MQTT_ProcessF1(uint8_t *data);

#endif /* INC_MQTT_H_ */
