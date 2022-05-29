/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "usbd_cdc_if.h"
#include "esp8266.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
//ESP 8266: wifi name and password want to connect
/*char APname[] = "Nguyen Van Ky";
char password[] = "10071959";*/
char APname[] = "NTC";
char password[] = "12021007";
extern uint16_t Size;
//Name and password of ESP
char ssid[]= "ESP8266";
char passwords[] = "12345678";
uint8_t channelid = 1;
uint8_t ecn = 3; //WPA2_PSK
//Address to connect to MQTT
char id = '1';
char type[] = "UDP";
char address[] = "192.168.1.2";
char port[] = "8000";
char localport[] = "23";
char mode = '0';
char TCPaddress[] = "myespthesis.000webhostapp.com";
char TCPport[] = "80";
//USB
extern char ReceivedData[100];
extern uint8_t Rxcount;
extern uint32_t dataSize;
extern uint8_t usbflag;
extern uint32_t time;
extern uint32_t UsbTimeOut;
//Uart3
char Uart3Received[100];
uint16_t Uart3Count = 0;
uint8_t Uart3temp;
char Uart3temp2[100];
uint8_t RespondESP = 0;
uint8_t RespondRST = 0;
int8_t cwmodes = -1;
int8_t cipmuxs = -1;
//Others
char temp[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void loop(void);
void ClearUSBReceived(void);
int SendToWeb(void);
int test1(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*while(1) loop
*/
void loop(void)
{
	if (usbflag == 1)
	{
		usbflag = 0;
		CDC_Transmit_FS((uint8_t*)"Hello/n", 7);
		/*
		if (!strcmp(ReceivedData,"Start"))
		{
			ESPInit();
		}
		else if (!strcmp(ReceivedData,"Con"))
		{
			ESPconnect(APname,password);
			//ESPConfigAP(ssid, passwords,channelid,ecn);
			//ESPwaitRespone(1000);
		}
		else if (!strcmp(ReceivedData,"MQTT"))
		{
			MQTTbrockerConnect(address,port);
		}
		else if (!strcmp(ReceivedData,"UDP"))
		{
			ESPStartUDP(id,"UDP",address,port,localport,mode);
		}
		else if (!strcmp(ReceivedData,"TCP"))
		{
			ESPStartUDP('0',"TCP","myespthesis.000webhostapp.com","80",localport,mode);
			//This work
			//AT+CIPSTART=0,"TCP","myespthesis.000webhostapp.com",80
		}
		else if (!strcmp(ReceivedData,"IPSEND"))
		{
			ESPipsend(id,"\nHello Im ESP8266");
		}
		else if (!strcmp(ReceivedData,"test"))
		{
			test1();
		}
		else if (!strcmp(ReceivedData,"send"))
		{
			SendToWeb();
		}
		else
		{
			ESPsendCommand(ReceivedData);
			//CDC_Transmit_FS((uint8_t*)"Hello/n", 7);
			//ESPwaitRespone(1000);
		}
		*/
		HAL_UART_Transmit(&huart3, (uint8_t *)ReceivedData , dataSize, 100);
		ClearUSBReceived();
		//strcpy(temp ,ReceivedData);
	}
}
/*UART interupt
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	RespondESP = 0;
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
	if(Uart3temp == '>')
	{
		CDC_Transmit_FS((uint8_t*)"\n>", 2);
		//cipsend = 1;
	}
	if (Uart3temp != 0xd) //Stop when received carriage return (CR) \r
		Uart3Received[Uart3Count++] = Uart3temp; 
	else
	{
		//strcpy(Uart3temp2 ,Uart3Received);
		if (strstr(Uart3Received,"OK"))
			RespondESP = 1;
		else if (strstr(Uart3Received,"ERROR"))
			RespondESP = 2;
		else if (strstr(Uart3Received,"ready")) //Wait for restarting
			RespondRST = 1;
		else if (strstr(Uart3Received,"CWMODE:"))
			cwmodes = Uart3Received[9] - 48;
		else if (strstr(Uart3Received,"CIPMUX:"))
			cipmuxs = Uart3Received[9] - 48;
		//
		CDC_Transmit_FS((uint8_t*)Uart3Received, Uart3Count);
		for (int i=0; i < Uart3Count; i++)
			Uart3Received[i] = 0;
		Uart3Count = 0;
	}
}
/*Clear USB received data
*/
void ClearUSBReceived(void)
{
	for (int i=0; i<Rxcount; i++)
			ReceivedData[i] = 0;
}
int SendToWeb()//Work! Great!!!
{
	char datatx[200];
	char sendlength[20];
	ESPStartTCP("www.myespthesis.000webhostapp.com","80");
	if (ESPwaitRespone(10000) != 1)
		return 0;
	sprintf(&datatx[0],"GET /function/receive.php?update=ok&name=Cuong&NUM=1000 HTTP/1.1\r\nHost: myespthesis.000webhostapp.com\r\n");
	sprintf(&sendlength[0],"AT+CIPSEND=%d",strlen(datatx)+2);
	ESPsendCommand(sendlength);
	if (ESPwaitRespone(1000) != 1)
		return 0;
	ESPsendCommand(datatx);
	return 1;
}
int test1()
{
	char datatx[200];
	char sendlength[20];
	ESPStartTCP("www.esp82test1.000webhostapp.com","80");
	if (ESPwaitRespone(10000) != 1)
		return 0;
	//AT+CIPSEND=0,120
	sprintf(&datatx[0],"GET /xuli.php?update=ok&v1=1&v2=11&v3=1&v4=1&v5=1&v6=1 HTTP/1.1\r\nHost: esp82test1.000webhostapp.com\r\n");
	sprintf(&sendlength[0],"AT+CIPSEND=%d",strlen(datatx));
	ESPsendCommand(sendlength);
	uint32_t now = HAL_GetTick();
	if (ESPwaitRespone(1000) != 1)
		return 0;
	
	ESPsendCommand(datatx);
	if (ESPwaitRespone(1000) != 1)
		return 0;
	ESPsendCommand("AT+CIPCLOSE");
	return 1;
}
/*
"GET /add.php?update=ok&v1=%i&v2=%i&v3=%i&v4=%.2f&v5=%i&v6=%i HTTP/1.1\r\n
Host: giamsatmoitruong.iot47.com\r\n\r\n"
*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	//HAL_TIM_Base_Start(&htim4);
	//HAL_Delay(2000);
	HAL_UART_Receive_IT(&huart3,&Uart3temp, 1);
	ESPInit();
	ESPconnect(APname,password);
	ESPStartUDP(id,"UDP",address,port,localport,mode);
	
	CDC_Transmit_FS((uint8_t *)"\nESP start ccomplete!\n",22);
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		loop();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
