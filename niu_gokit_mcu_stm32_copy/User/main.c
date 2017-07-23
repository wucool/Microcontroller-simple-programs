/**
********************************************************
*
* @file      main.c
* @author    Gizwtis
* @version   V2.3
* @date      2015-07-06
*
* @brief     ?ú???? ???a???üó2?t??éú
*            Gizwits Smart Cloud  for Smart Products
*            á′?ó|???μ|?a·?|?Dá￠|°2è?|×?óD|×?óé|éúì?
*            www.gizwits.com
*
*********************************************************/

/* Includes ------------------------------------------------------------------*/


#include "gokit.h"

/*Global Variable*/
uint32_t ReportTimeCount = 0;
uint8_t gaterSensorFlag = 0;
uint8_t Set_LedStatus = 0;
uint8_t NetConfigureFlag = 0;
uint8_t curTem = 0, curHum = 0;
uint8_t lastTem = 0,lastHum = 0;

extern RingBuffer u_ring_buff;
uint8_t p0Flag = 0;

WirteTypeDef_t	WirteTypeDef;
ReadTypeDef_t	ReadTypeDef;
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
uint8_t led_control = 0;
uint8_t key_pre = 0;
int main(void)
{
	uint8_t p0_control_buf[MAX_P0_LEN];		
	SystemInit();	
	HW_Init();
	Printf_SystemRccClocks();
	SW_Init();
	while(1)
	{		
		HANDLE_Key_After_Press();
		//Delay_ms(2000);
		//KEY_Handle();
	}
 //  	KEY_Handle();
 }
/** @addtogroup GizWits_HW_Init
  * @{
  */
void HW_Init(void)
{
	Delay_Init(72);	
	UARTx_Init();
    RGB_KEY_GPIO_Init();
	RGB_LED_Init();	
	LED_GPIO_Init();	
	KEY_GPIO_Init();
	TIM3_Int_Init(7199,9);   //ms interrupt	
	Motor_Init();	
	DHT11_Init();	
	IR_Init();	
}
/** @addtogroup GizWits_SW_Init
  * @{
  */
void SW_Init()
{
	ReadTypeDef.Alert = 0;
	ReadTypeDef.LED_Cmd = 0;
	ReadTypeDef.LED_R = 0;
	ReadTypeDef.LED_G = 0;  
	ReadTypeDef.LED_B = 0;
	ReadTypeDef.Motor = 5;
	ReadTypeDef.Infrared = 0;
	ReadTypeDef.Temperature = 0;
	ReadTypeDef.Humidity = 0;
	ReadTypeDef.Alert = 0;
	ReadTypeDef.Fault = 0;
	GizWits_init(sizeof(ReadTypeDef_t));
	printf("Gokit Init Ok ...\r\n");
}
/** @addtogroup Printf_SystemRccClocks
  * @{
  */
void Printf_SystemRccClocks(void)
{
	uint8_t SYSCLKSource;

	RCC_ClocksTypeDef  SystemRCC_Clocks;
	printf("System start...\r\n");
	SYSCLKSource = RCC_GetSYSCLKSource();
	if(SYSCLKSource==0x04)
		printf("SYSCLKSource is HSE\r\n");
	else if(SYSCLKSource==0x00)
		printf("SYSCLKSource is HSI\r\n");
	else if(SYSCLKSource==0x08)
		printf("SYSCLKSource is PL!\r\n");
	
	RCC_GetClocksFreq(&SystemRCC_Clocks);
	printf("SYS clock =%dMHz \r\n",(uint32_t)SystemRCC_Clocks.SYSCLK_Frequency/1000000);
	printf("HCLK clock =%dMHz \r\n",(uint32_t)SystemRCC_Clocks.HCLK_Frequency/1000000);
	printf("PCLK1 clock =%dMHz \r\n",(uint32_t)SystemRCC_Clocks.PCLK1_Frequency/1000000);
	printf("PCLK2_clock =%dMHz \r\n",(uint32_t)SystemRCC_Clocks.PCLK2_Frequency/1000000);	
	printf("SADCCLK_Frequencyclock =%dMHz \r\n",(uint32_t)SystemRCC_Clocks.ADCCLK_Frequency/1000000);

}

/*******************************************************************************
* Function Name  : GizWits_ControlDeviceHandle
* Description    : Analy P0 Package
* Input          : None
* Output         : None
* Return         : Bit , Attr_Flags
* Attention		 : 
*******************************************************************************/
void GizWits_ControlDeviceHandle()
{
	if( (WirteTypeDef.Attr_Flags & (1<<0)) == (1<<0))
	{
		if(Set_LedStatus != 1)
		{
			if(WirteTypeDef.LED_Cmd == LED_OnOff)
			{
				LED_RGB_Control(0,0,0);
				ReadTypeDef.LED_Cmd = LED_OnOff;
				printf("SetLED_Off \r\n");
			}
			if(WirteTypeDef.LED_Cmd == LED_OnOn)
			{
				ReadTypeDef.LED_Cmd = LED_OnOn;
				LED_RGB_Control(254,0,0);
				printf("SetLED_On \r\n");
			}
		}
		
	}
	if( (WirteTypeDef.Attr_Flags & (1<<1)) == (1<<1))
	{
		if(WirteTypeDef.LED_Cmd == LED_Costom)
			{
				ReadTypeDef.LED_Cmd = LED_Costom;
				ReadTypeDef.LED_R = 0;
				ReadTypeDef.LED_G = 0;
				ReadTypeDef.LED_B = 0;
				Set_LedStatus = 0;
				LED_RGB_Control(0, 0, 0);
				printf("SetLED LED_Costom \r\n");
			}
			if(WirteTypeDef.LED_Cmd == LED_Yellow)
			{
				Set_LedStatus = 1;
				ReadTypeDef.LED_Cmd = LED_Yellow;
				ReadTypeDef.LED_R = 254;
				ReadTypeDef.LED_G = 254;
				ReadTypeDef.LED_B = 0;
				
				LED_RGB_Control(254, 254, 0);
				printf("SetLED LED_Yellow \r\n");
			}
					
			if(WirteTypeDef.LED_Cmd == LED_Purple)
			{
				ReadTypeDef.LED_Cmd = LED_Purple;
				ReadTypeDef.LED_R = 254;
				ReadTypeDef.LED_G = 0;
				ReadTypeDef.LED_B = 70;
				Set_LedStatus = 1;
				LED_RGB_Control(254, 0, 70);	
				printf("SetLED LED_Purple \r\n");
			}
			if(WirteTypeDef.LED_Cmd == LED_Pink)
			{
				ReadTypeDef.LED_Cmd = LED_Pink;
				ReadTypeDef.LED_R = 238;
				ReadTypeDef.LED_G = 30;
				ReadTypeDef.LED_B = 30;
				Set_LedStatus = 1;
				LED_RGB_Control(238 ,30 ,30);
				printf("SetLED LED_Pink \r\n");
			}
	}
	if( (WirteTypeDef.Attr_Flags & (1<<2)) == (1<<2))
	{
		if(Set_LedStatus != 1)
		{
			ReadTypeDef.LED_R = WirteTypeDef.LED_R;
			printf("W2D Control LED_R = %d \r\n",WirteTypeDef.LED_R);
			LED_RGB_Control(ReadTypeDef.LED_R,ReadTypeDef.LED_G,ReadTypeDef.LED_B);
		}
		
	}
	if( (WirteTypeDef.Attr_Flags & (1<<3)) == (1<<3))
	{
		if(Set_LedStatus != 1)
		{
			ReadTypeDef.LED_G = WirteTypeDef.LED_G;
			printf("W2D Control LED_G = %d \r\n",WirteTypeDef.LED_G);
			LED_RGB_Control(ReadTypeDef.LED_R,ReadTypeDef.LED_G,ReadTypeDef.LED_B);
		}
		
	}
	if( (WirteTypeDef.Attr_Flags & (1<<4)) == (1<<4))
	{
		if(Set_LedStatus != 1)
		{
			ReadTypeDef.LED_B = WirteTypeDef.LED_B;
			printf("W2D Control LED_B = %d \r\n",WirteTypeDef.LED_B);
			LED_RGB_Control(ReadTypeDef.LED_R,ReadTypeDef.LED_G,ReadTypeDef.LED_B);
		}
		
	}
	if( (WirteTypeDef.Attr_Flags & (1<<5)) == (1<<5))
	{
		ReadTypeDef.Motor = WirteTypeDef.Motor;
#ifdef MOTOR_16
		printf("W2D Control Motor = %d \r\n",exchangeBytes(WirteTypeDef.Motor));
		Motor_status(exchangeBytes(WirteTypeDef.Motor));
#else
		printf("W2D Control Motor = %d \r\n",WirteTypeDef.Motor);
		Motor_status(WirteTypeDef.Motor);
#endif
	}
}
/*******************************************************************************
* Function Name  : GizWits_GatherSensorData();
* Description    : Gather Sensor Data
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GizWits_GatherSensorData(void)
{
	ReadTypeDef.Infrared = IR_Handle();
	DHT11_Read_Data(&curTem, &curHum);
	ReadTypeDef.Temperature = (curTem + lastTem) /2;
	ReadTypeDef.Humidity = (curHum + lastHum)/2;

	ReadTypeDef.Temperature = ReadTypeDef.Temperature + 13;//Temperature Data Correction
	lastTem = curTem;
	lastHum = curHum;
}
/*******************************************************************************
* Function Name  : KEY_Handle
* Description    : Key processing function
* Input          : None
* Output         : None
* Return         : None
* Attention		 	 : None
*******************************************************************************/
void KEY_Handle(void)
{
	uint8_t Key_return =0;	
	Key_return = ReadKeyValue();	
	printf("################### KEY_Handle--->ReadKeyValue()= %d  \r\n", Key_return);
	printf("################### KEY_Handle---->Get_Key()=%d       \r\n",Get_Key());
	if(Key_return & KEY_UP)
	{
		if(Key_return & PRESS_KEY1)
		{
  		printf("KEY1 PRESS\r\n");
		}
		if(Key_return & PRESS_KEY2)
		{
#ifdef PROTOCOL_DEBUG
//			printf("KEY2 PRESS ,Soft AP mode\r\n");
			  printf("KEY2 PRESS ,LED mode start\r\n");
#endif	
			//Soft AP mode, RGB red
//			LED_RGB_Control(255, 0, 0);
			GizWits_D2WConfigCmd(SoftAp_Mode);
			printf("(SoftAp_Mode start\r\n");
			NetConfigureFlag = 1;
		}				
}

	if(Key_return & KEY_LONG)
	{
		if(Key_return & PRESS_KEY1)
		{
#ifdef PROTOCOL_DEBUG
				printf("KEY1 PRESS LONG ,Wifi Reset\r\n");
#endif			
			GizWits_D2WResetCmd();
			Motor_status(5);
		}
		if(Key_return & PRESS_KEY2)
		{
			//AirLink mode, RGB Green
#ifdef PROTOCOL_DEBUG
				printf("KEY2 PRESS LONG ,AirLink mode\r\n");
#endif	
			LED_RGB_Control(0, 128, 0);
			GizWits_D2WConfigCmd(AirLink_Mode);
			NetConfigureFlag = 1;
		}
	}
/*  if(!(Key_return))
	{
	 	uint8_t i;
		uint8_t NUM[]={LED1,LED2,LED3,LED4};	
    while(1)
		{
			for(i=0;i<4;i++)
				{
			LED_ON(NUM[i]);
			Delay_us(1000000);
			LED_OFF(NUM[i]);
				}
		}
	}  */
}
/*******************************************************************************
* Function Name  : GizWits_WiFiStatueHandle
* Description    : Callback function , Judge Wifi statue
* Input          : None
* Output         : None
* Return         : Bit , Attr_Flags
* Attention		 : 
*******************************************************************************/
void GizWits_WiFiStatueHandle(uint16_t wifiStatue) 
{	
	if(((wifiStatue & Wifi_ConnClouds) == Wifi_ConnClouds) && (NetConfigureFlag == 1))
	{
		printf("W2M->Wifi_ConnClouds\r\n");
		NetConfigureFlag = 0;
		LED_RGB_Control(0,0,0);
	}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

/**
** 按键处理函数，根据上次状态来确定是点亮马灯还是 启动马达和RGB—LED
** 
** 
* 参数：num ,代表多个个10ms
**/
void HANDLE_Key_After_Press(void)
{
	if(!led_control)
		{
			printf("######HANDLE_Key_After_Press  status:1111 Open LED  close motor start \r\n");
			Motor_status(5);//关马达
		  LED_RUNNING();//开跑马灯
			printf("######HANDLE_Key_After_Press  status:1111 Open LED  close motor end \r\n");
		}
		else
		{
			printf("######HANDLE_Key_After_Press  status:2222 Open motor  close RGB start \r\n");
			Motor_status(6);// 开马达
			RGB();          // 开RGB灯	
			printf("######HANDLE_Key_After_Press  status:2222 Open motor  close RGB end \r\n");
		//}
	}
}

/**
** 延时函数，在延时函数过程中增加了按键状态的判断，
** 当有key1按键按下的时候，执行按键处理函数HANDLE_Key_After_Press()
* 参数：num ,代表多个个10ms
**/
void DELAY_TIME_Brake(uint8_t  num)
{
	for(uint8_t i=0; i<num; i++)
	{
		Delay_ms(10);
		
		uint8_t Key_return =0;	
		Key_return = ReadKeyValue();			
		if(Key_return & KEY_UP)// 以前用Get_Key函数来判断key1键的按下，错误的
													//  其实Get_Key返回值为1的时候用来判断key1键的按下并不准确,整个按键动作,会返回好几次Get_Key为1
		{
			if(Key_return & PRESS_KEY1)
			{
				//key_pre = Get_Key();
				printf("####DELAY_TIME_Brake  now Get_Key= %d\r\n",Get_Key());
				led_control=~led_control;
				printf("####DELAY_TIME_Brake  now led_control= %d\r\n",led_control);
				Delay_ms(200);//按键200ms后才处理动作
				HANDLE_Key_After_Press();//此时有按键，先去处理按键事件
				break;
			}

		}		
	}
}

void RGB(void)
{
	  uint8_t i;
		for(i=1;i<5;i++)
			{
			  LED_OFF(i);
			}
//		printf("the second key number is %d\r\n",Key);
			LED_RGB_Control(255,0,0);			
//				  Delay_ms(150);
			DELAY_TIME_Brake(15);

			//LED_RGB_Control(0,0,0);
			   // Delay_ms(150);
			
			LED_RGB_Control(255,255,0);					
				  //Delay_ms(150);
			DELAY_TIME_Brake(15);
			 
			//LED_RGB_Control(0,0,0);
			//Delay_ms(150);			
			 
			LED_RGB_Control(0,128,0);
			    //Delay_ms(150);
			//Delay_ms(150);
			DELAY_TIME_Brake(15);
			
			//LED_RGB_Control(0,0,0);
}


/** 跑马灯 LED1~4循环点亮**/
void LED_RUNNING(void)
{
	   uint8_t i;
     for(i=1;i<5;i++)
		 {
		  LED_ON(i);
			DELAY_TIME_Brake(50);
			LED_OFF(i);
			DELAY_TIME_Brake(50); 
		}
}