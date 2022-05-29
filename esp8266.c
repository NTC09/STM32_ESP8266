#include "esp8266.h"
//Variables
extern uint8_t RespondESP;
extern uint8_t RespondRST;
extern int8_t cwmodes;
extern int8_t cipmuxs;
extern UART_HandleTypeDef huart3;
UART_HandleTypeDef *uartx = &huart3;
//uint16_t Size;
//uint16_t waitTime = 1000; // milisecond
uint16_t SendTime = 1000; // milisecond
/*Functions
*/
/*Send a command or data, included \r\n*/
void ESPsendCommand(char Command[])
{
	uint16_t Size = strlen(Command);
	char command[Size+2];
	sprintf(&command[0],"%s\r\n",&Command[0]);
	RespondESP = 0;
	HAL_UART_Transmit(uartx, (uint8_t *)command, Size+2, SendTime);
}
/*Connect to a accesspiont
AT+CWJAP?
Response:
+ CWJAP:<ssid>
AT+CWJAP = <ssid>,< pwd >
Param description:
	<ssid> string, AP’s SSID 
	<pwd> string, MAX: 64 bytes ASCII
This configuration will store in Flash system parameter area
*/
uint8_t ESPconnect(char name[], char pass[])
{
	int8_t mode = ESPgetcwmode();
	if (mode == -1)
		return 0;
	else if (mode == 1)
		mode =1;
	else if (mode == 2)
		mode = 0;
	else 
		mode = 2;
	char data1[11+strlen(name)+strlen(pass)];
		/*AT+CWDHCP=<mode>,<en>
		<mode>
		0 : set ESP8266 softAP
		1 : set ESP8266 station
		2 : set both softAP and station
		<en>
		0 : Enable DHCP
		1 : Disable DHCP*/
	sprintf(&data1[0],"AT+CWDHCP=%d,%d",mode,1);
	ESPsendCommand(data1);
	if (ESPwaitRespone(1000)!=1)
		return 0;
	char dat2[14+strlen(name)+strlen(pass)];
	sprintf(&dat2[0],"AT+CWJAP=\"%s\",\"%s\"",&name[0],&pass[0]);
	//Connect to a access point
	ESPsendCommand(dat2);
	if (ESPwaitRespone(10000)!=1)
		return 0;
	return 1;
}
/*Configuration of softAP mode
<ssid> string, ESP8266 softAP’ SSID 
<pwd>  string, MAX: 64 bytes ASCII
<chl> channel id
	ecn=0 OPEN
			2 WPA_PSK
			3 WPA2_PSK
			4 WPA_WPA2_PSK
This configuration will store in Flash system parameter area
Response:
OK
ERROR
AT+ CWSAP?
Response:
+ CWSAP:<ssid>,<pwd>,<chl>,<ecn>*/
uint8_t ESPConfigAP(char ssid[], char pwd[],uint8_t chl,uint8_t ecn)
{
	char data[18+strlen(ssid)+strlen(pwd)];
	sprintf(&data[0],"AT+CWSAP=\"%s\",\"%s\",%1d,%1d",&ssid[0],&pwd[0],chl,ecn);
	ESPsendCommand(data);
	if (ESPwaitRespone(2000)!=1)
		return 0;
	return 1;
}
/*ESP initialization
*/
uint8_t ESPInit(void)
{
	//Reset
	ESPsendCommand(AT_RST);
	if (ESPwaitRestart(5000) != 1)
		return 0;
	//Switches echo off
	ESPsendCommand(ATE0);
	if (ESPwaitRespone(500)!=1)
		return 0;
	//Station mode
	ESPsendCommand(CWMODE1);
	if (ESPwaitRespone(500)!=1)
		return 0;
	ESPsendCommand(CIPMUX0);
	if (ESPwaitRespone(500)!=1)
		return 0;
	//CDC_Transmit_FS((uint8_t*)"\nESP intialition complete\n", 20);
	return 1;
}
/*Wait ESP respond "OK" after time miliseconds
*/
uint8_t ESPwaitRespone(uint32_t waitTime)
{
	uint32_t now = HAL_GetTick();
	while(HAL_GetTick() - now < waitTime)
	{
		if (RespondESP==1)
			return 1;
		if (RespondESP==2)
			return 0;
	}
	CDC_Transmit_FS((uint8_t*)"\nTimeout", 8);
	return 0;
}
/*Wait ESP respond "ready" after time miliseconds
*/
uint8_t ESPwaitRestart(uint32_t waitTime)
{
	uint32_t now = HAL_GetTick();
	while(HAL_GetTick() - now < waitTime)
	{
		if (RespondRST==1) //Restart complete
		{	
			RespondRST = 0;
			return 1;
		}
	}
	CDC_Transmit_FS((uint8_t*)"\nTimeout", 8);
	RespondRST = 0;
	return 0;
}
/*Get station mode and accesspoint mode
*/
uint8_t ESPgetcwmode(void)
{
	ESPsendCommand("AT+CWMODE?");
	ESPwaitRespone(100);
	if (cwmodes != -1)
		return cwmodes;
	return 3;
}
/*Multiple connection or single connection
*/
uint8_t ESPgetcipmux(void)
{
	ESPsendCommand("AT+CIPMUX?");
	if(ESPwaitRespone(100) != 1)
		return 3;
	return cipmuxs;
}
/*UART configuration 
<baudrate> UART baudrate, range: 110~115200*40
<databits> data bits
		5:5 bits data
		6:6 bits data
		7:7 bits data
		8:8 bits data
<stopbits> stop bits
		 1:1 bit stop bit
		 2:1.5 bit stop bit
		 3:2 bit stop bit
<parity> parity
		 0:None
		 1:Odd
		 2:EVEN
<flow control> flow control
		 0:disable flow control
		 1:enable RTS
		 2:enable CTS
		 3:enable both RTS and CTS
 Example: AT+UART=115200,8,1,0,3
*/
uint8_t ESpPuartConfig(uint32_t baudrate, 
												uint8_t databits, 
												uint8_t stopbits, 
												uint8_t parity, 
												uint8_t flowcontrol)
{
	
	return 1;
}
