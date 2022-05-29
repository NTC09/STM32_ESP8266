#ifndef __TCP_H_
#define __TCP_H_
//Include libraries
#include "esp8266.h"
/*Defines
*/
#define CIPSTATUS 	"AT+CIPSTATUS"		//Get connection status
#define CIPSTART 		"AT+CIPSTART"			//Establish TCP connection or register UDP port 
#define CIPSEND 		"AT+CIPSEND"			//Send data
#define CIPCLOSE		"AT+CIPCLOSE"			//Close TCP/UDP connection
#define CIFSR				"AT+CIFSR"				//Get local IP address (return IP and MAC)
#define CIPMUX1 		"AT+CIPMUX=1" 		//Multiple connection
#define CIPMUX0 		"AT+CIPMUX=0" 		//Single connection
#define CIPSERVER 	"AT+CIPSERVER" 		//Configure as server
#define CIPMODE 		"AT+CIPMODE" 			//Set transmission mode
#define CIPSTO 			"AT+CIPSTO" 			//Set timeout when ESP8266 runs as TCP server
#define CIUPDATE 		"AT+CIUPDATE"			//Upgrade firmware through network
#define PING 				"AT+PING" 				//Function PING
/*Functions
*/
uint8_t ESPStartTCP(char address[],char port[]);
uint8_t ESPStartUDP(char id,char type[], char address[],char port[],char localport[],char mode);
uint8_t ESPipsend(uint8_t id, char data[]);
#endif
