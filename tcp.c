#include "tcp.h"
/*Start connect TCP
CIPMUX=0:
AT+CIPSTART=<type>,<addr>,<port>[,<local port>,<mode>]
CIPMUX=1:
AT+CIPSTART=<id>,<type>,<addr>,<port>[,(<local port>),(<mode>)]

--<id> 0-4 , id of connection
--<type> string, “TCP” or “UDP”
--<addr> string, remote ip
--<port> string, remote port
--[<mode>] for UDP only
0 : destination peer entity of UDP will not change.
1 : destination peer entity of UDP can change once.
2 : destination peer entity of UDP is allowed to change.
*/
uint8_t ESPStartTCP(char address[],char port[])
{
	char data[3+  //type
						strlen(address)+
						strlen(port)+17];
	sprintf(&data[0],"AT+CIPSTART=\"TCP\",\"%s\",%s",&address[0],&port[0]);
	//Connect to a access point
	ESPsendCommand(data);
	if (ESPwaitRespone(10000)!=1)
		return 0;
	return 1;
}
/*Start connect UDP
*/
uint8_t ESPStartUDP(char id,char type[], char address[],char port[],char localport[],char mode)
{
	int8_t mux = ESPgetcipmux();
	if (mux == -1)
		return 0;
	else if (mux == 0)
	{
		char data[4+ //type + mode
							strlen(address)+
							strlen(port)+
							strlen(localport) + 18 + 1 + 1]; // "," for mode and localport
		sprintf(&data[0],"AT+CIPSTART=\"%s\",\"%s\",%s",&type[0],&address[0],&port[0]);
		if(strlen(localport) != 0)
		{
			strcat(data,",");
			strcat(data,localport);
		}
		if(mode >= 48 && mode <= 50 )
		{
			strcat(data,",");
			strcat(data,&mode);
		}
		ESPsendCommand(data);
	}
	else if (mux == 1)
	{
		char data[3+ 
							strlen(address)+ 
							strlen(port)+
							strlen(localport)+
							1+ //mode
							19 + 1 // 1 is id, 
							+ 1 + 1]; // "," for mode and localport
		sprintf(&data[0],"AT+CIPSTART=\"%x\",\"%s\",%s,%s",id,&type[0],&address[0],&port[0]);
		if(strlen(localport) != 0)
		{
			strcat(data,",");
			strcat(data,localport);
		}
		if(mode >= 48 && mode <= 50 )
		{
			strcat(data,",");
			strcat(data,&mode);
		}
		ESPsendCommand(data);
	}
	if (ESPwaitRespone(10000)!=1)
		return 0;
	return 1;
}
/*
(+CIPMUX=0)
AT+CIPSEND=<length>
(+CIPMUX=1)
AT+CIPSEND=<id>,<length>

<id> 			ID no. of transmit connection
<length> 	data length, MAX 2048 bytes
*/
uint8_t ESPipsend(uint8_t id, char data[])
{
	int8_t mux = ESPgetcipmux();
	uint16_t length = strlen(data);
	if (length == 0)
		return 0;
	if (mux == 0)
	{
		uint8_t num=1;
		uint16_t i = length;
		while(i>=10)
		{
			i = i / 10;
			num++;
		}
		char datatx[num + 11];
		sprintf(&datatx[0],"AT+CIPSEND=%d",length);
		ESPsendCommand(datatx);
		if (ESPwaitRespone(1000)!=1)
			return 0;
		//HAL_Delay(20);
		ESPsendCommand(data);
		if (ESPwaitRespone(1000)!=1)
			return 0;
	}
	if (mux == 1)
	{
		uint8_t num=1;
		uint16_t i = length;
		while(i>=10)
		{
			i = i / 10;
			num++;
		}
		char datatx[num + 13];
		sprintf(&datatx[0],"AT+CIPSEND=%1d,%d",id,length);
		ESPsendCommand(datatx);
		if (ESPwaitRespone(1000)!=1)
			return 0;
		ESPsendCommand(data);
		if (ESPwaitRespone(1000)!=1)
			return 0;
	}
	else 
		return 0;
	return 1;
}
