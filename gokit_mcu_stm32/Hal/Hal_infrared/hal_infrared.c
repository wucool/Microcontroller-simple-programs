/**
********************************************************
*
* @file      Hal_infrared.c
* @author    Gizwtis
* @version   V2.3
* @date      2015-07-06
*
* @brief     机智云 只为智能硬件而生
*            Gizwits Smart Cloud  for Smart Products
*            链接|增值|开放|中立|安全|自有|自由|生态
*            www.gizwits.com
*
*********************************************************/

#include <stm32f10x.h>
#include "Hal_infrared/Hal_infrared.h"
void IR_GPIO_interrupt_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;  
    EXTI_InitTypeDef EXTI_InitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;   
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);//??GPIO AFIO???  
    GPIO_InitStructure.GPIO_Pin = Infrared_GPIO_PIN;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;  
		GPIO_Init(Infrared_GPIO_PORT, &GPIO_InitStructure);   
          
    EXTI_ClearITPendingBit(EXTI_Line7);  
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);//PC11  ?GPIOC?PIN11  
    EXTI_InitStructure.EXTI_Line= EXTI_Line7; //PC11,?:EXTI_Line11  
    EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;   
   // EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising_Falling;   //???????????
		EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling; 
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;  
    EXTI_Init(&EXTI_InitStructure);  
          
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   //NVIC  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority= 2;          
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;     
    NVIC_Init(&NVIC_InitStructure);  
}
void IR_Init(void)
{
	 	/*
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(Infrared_GPIO_CLK,ENABLE);//使能PORTA,PORTE时钟
    GPIO_InitStructure.GPIO_Pin  = Infrared_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
    GPIO_Init(Infrared_GPIO_PORT, &GPIO_InitStructure);
	*/
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(Infrared_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;


    GPIO_InitStructure.GPIO_Pin = Infrared_GPIO_PIN;
    GPIO_Init(Infrared_GPIO_PORT, &GPIO_InitStructure);
		IR_GPIO_interrupt_Init();
}
bool IR_Handle(void)
{
    if(GPIO_ReadInputDataBit(Infrared_GPIO_PORT, Infrared_GPIO_PIN))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
