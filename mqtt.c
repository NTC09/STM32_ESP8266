#include "mqtt.h"

uint8_t MQTTbrockerConnect(char address[],char port[])
{
	ESPStartTCP(address,port);
	return 1;
}
