/**
********************************************************
*
* @file      hal_key.c
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
#include "Hal_key/hal_key.h"

extern uint8_t									     	 SN;
uint8_t 				KeyCountTime;


/*******************************************************************************
* Function Name  : KEY_GPIO_Init
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

void KEY1_GPIO_interrupt_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;  
    EXTI_InitTypeDef EXTI_InitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;   
      
    RCC_APB2PeriphClockCmd(GPIO_KEY1_CLK |RCC_APB2Periph_AFIO, ENABLE);//??GPIO AFIO???  
    GPIO_InitStructure.GPIO_Pin = GPIO_KEY1_PIN;  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
		//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;  
		GPIO_Init(GPIO_KEY1_PORT, &GPIO_InitStructure);   
          
    EXTI_ClearITPendingBit(EXTI_Line10);  
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);//PC11  ?GPIOC?PIN11  
    EXTI_InitStructure.EXTI_Line= EXTI_Line10; //PC11,?:EXTI_Line11  
    EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;   
   // EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising_Falling;   //???????????
		EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling; 
    EXTI_InitStructure.EXTI_LineCmd=ENABLE;  
    EXTI_Init(&EXTI_InitStructure);  
          
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   //NVIC  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority= 2;          
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;     
    NVIC_Init(&NVIC_InitStructure);  
		
}
void KEY_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
   // RCC_APB2PeriphClockCmd(GPIO_KEY1_CLK |GPIO_KEY2_CLK | GPIO_KEY3_CLK, ENABLE);
		RCC_APB2PeriphClockCmd(GPIO_KEY2_CLK | GPIO_KEY3_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

//    GPIO_InitStructure.GPIO_Pin = GPIO_KEY1_PIN;
//    GPIO_Init(GPIO_KEY1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_KEY2_PIN;
    GPIO_Init(GPIO_KEY2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_KEY3_PIN;
    GPIO_Init(GPIO_KEY3_PORT, &GPIO_InitStructure);
		KEY1_GPIO_interrupt_Init();
}

/*******************************************************************************
* Function Name  : TIM3_Int_Init
* Description    : TIM3 initialization function
* Input          : arr 重装初值
									 psc 预分频
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
    TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}


/*******************************************************************************
* Function Name  : Get_Key
* Description    : Read the KEY state
* Input          : None
* Output         : None
* Return         : uint8_t KEY state
* Attention		 : None
*******************************************************************************/

uint8_t Get_Key(void)
{
#if 0   
	uint8_t ReadKey;

    if(!GPIO_ReadInputDataBit(GPIO_KEY1_PORT,GPIO_KEY1_PIN))
    {
        ReadKey |= PRESS_KEY1;
    }
    if(!GPIO_ReadInputDataBit(GPIO_KEY2_PORT,GPIO_KEY2_PIN))
    {
        ReadKey |= PRESS_KEY2;
    }
    if(!GPIO_ReadInputDataBit(GPIO_KEY3_PORT,GPIO_KEY3_PIN))
    {
        ReadKey |= PRESS_KEY3;
    }

    return ReadKey;
		#endif
		    uint8_t ReadKey=0;

    if(!GPIO_ReadInputDataBit(GPIO_KEY1_PORT,GPIO_KEY1_PIN))
    {
        ReadKey = PRESS_KEY1;
    }
    if(!GPIO_ReadInputDataBit(GPIO_KEY2_PORT,GPIO_KEY2_PIN))
    {
        ReadKey = PRESS_KEY2;
    }
    if(!GPIO_ReadInputDataBit(GPIO_KEY3_PORT,GPIO_KEY3_PIN))
    {
        ReadKey = PRESS_KEY3;
    }

    return ReadKey;
}

/*******************************************************************************
* Function Name  : ReadKeyValue
* Description    : Read the KEY value
* Input          : None
* Output         : None
* Return         : uint8_t KEY value
* Attention		 : None
*******************************************************************************/
uint8_t ReadKeyValue(void)
{
    static uint8_t Key_Check;
    static uint8_t Key_State;
    static uint16_t Key_LongCheck;
    static uint8_t Key_Prev    = 0;        							//上一次按键

    uint8_t Key_press;
    uint8_t Key_return = 0;

    if(KeyCountTime >= 10)  //KeyCountTime 1MS+1  按键消抖10MS
    {
        KeyCountTime = 0;
        Key_Check = 1;
    }
    if(Key_Check == 1)
    {
        Key_Check = 0;
        Key_press = Get_Key();
        switch (Key_State)
        {
        case 0:
            if(Key_press != 0)
            {
                Key_Prev = Key_press;
                Key_State = 1;
            }

            break;
        case 1:
            if(Key_press == Key_Prev)
            {
                Key_State = 2;
                Key_return= Key_Prev | KEY_DOWN;
            }
            else 																					//按键抬起,是抖动,不响应按键
            {
                Key_State = 0;
            }
            break;
        case 2:

            if(Key_press != Key_Prev)
            {
                Key_State = 0;
                Key_LongCheck = 0;
                Key_return = Key_Prev | KEY_UP;
                return Key_return;
            }

            if(Key_press == Key_Prev)
            {
                Key_LongCheck++;
                if(Key_LongCheck >= 100)    //长按2S
                {
                    Key_LongCheck = 0;
                    Key_State = 3;
                    Key_return= Key_press |  KEY_LONG;
                    return Key_return;
                }
            }
            break;

        case 3:
            if(Key_press != Key_Prev)
            {
                Key_State = 0;
            }
            break;
        }
    }

    return  NO_KEY;
}
