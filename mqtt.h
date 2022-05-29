#ifndef __MQTT_H_
#define __MQTT_H_

#include "esp8266.h"
/*MQTT Control Header
*/
#define CONNECT			0x10
#define CONNACK 		0x20
#define PUBLISH 		0x30
#define PUBACK 			0x40
#define PUBREC			0x50
#define PUBREL 			0x60
#define PUBCOMP 		0x70
#define SUBSCRIBE 	0x80
#define SUBACK 			0x90
#define UNSUBSCRIBE	0xA0
#define UNSUBACK 		0xB0
#define PINGREQ 		0xC0
#define PINGRESP		0xD0
#define DISCONNECT	0xE0
/*Functions
*/
uint8_t MQTTbrockerConnect(char address[],char port[]);
#endif
