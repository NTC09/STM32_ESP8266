#ifndef __ESP8266_H
#define __ESP8266_H
/*
This library use:
AT version:1.6.2.0(Apr 13 2018 11:10:59)
SDK version:2.2.1(6ab97e9)
compile time:Jun  7 2018 19:34:26
Bin version(Wroom 02):1.6.2
ESP NODE MCU v3
STM32F103C8T6 KIT BLUEPILL
*/
//Include libraries
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "usbd_cdc_if.h"
//Esp libraries
#include "tcp.h"
#include "mqtt.h"
/*Defines
*/
#define AT 			"AT" 							//Tests AT startup
#define AT_RST 	"AT+RST" 					//Restarts the module
#define AT_GMR 	"AT+GMR" 					//Checks version information
#define GSLP 		"AT+GSLP" 				//Enter deep-sleep mode
#define ATE0 		"ATE0"						//Switches echo off
#define ATE1 		"ATE1"						//Switches echo on
#define RESTORE "AT+RESTORE" 			//Factory Reset
#define UART		"AT+UART UART" 		//configuration 
//Wifi
#define CWMODE1 "AT+CWMODE=1" 		//Station mode
#define CWMODE2 "AT+CWMODE=2" 		//Accesspoint mode
#define CWMODE3 "AT+CWMODE=3" 		//Both Station and AP mode
#define CWJAP 	"AT+CWJAP"			 	//Connect to AP
#define CWLAP 	"AT+CWLAP" 				//Lists available APs
#define CWQAP		"AT+CWQAP"				//Disconnect from AP
#define CWLIF 	"AT+CWLIF" 				//Get stations’ ip which are connected to ESP8266 softAP
#define CWDHCP 	"AT+CWDHCP" 			//Enable/Disable DHCP
#define AUTOC		"AT+CWAUTOCONN" 	//Connect to AP automatically or not when power on
#define STMAC		"AT+CIPSTAMAC" 		//Set mac address of ESP8266 station
#define APMAC 	"AT+CIPAPMAC" 		//Set mac address of ESP8266 softAP
#define CIPSTA	"AT+CIPSTA" 			//Set ip address of ESP8266 station
#define CIPAP		"AT+CIPAP" 				//Set ip address of ESP8266 softAP
/*Functions
*/
uint16_t datasize(char data[]);
void ESPsendCommand(char Command[]);
uint8_t ESPInit(void);
uint8_t ESPconnect(char name[], char pass[]);
uint8_t ESPwaitRespone(uint32_t waitTime);
uint8_t ESPwaitRestart(uint32_t waitTime);
uint8_t ESPConfigAP(char ssid[], char pwd[],uint8_t chl,uint8_t ecn);
uint8_t ESPgetcipmux(void);
uint8_t ESPgetcwmode(void);
//End
#endif
