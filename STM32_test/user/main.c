#include "Hal_led.h"
#include "Hal_key.h"
#include "Hal_motor.h"

int main(void)
{	 uint8_t key; 
	SystemInit();
	  Delay_Init(72);	
		LED_GPIO_Init();	
	  KEY_GPIO_Init();
	//TIM3_Int_Init(7199,9);   //ms interrupt	
		Motor_Init();

	while(1)
	{

		Delay_ms(500);

		Delay_ms(500);

		key=ReadKeyValue();
		if(key==1)
		{
			    LED_ON(LED1);
    LED_ON(LED2);
    LED_ON(LED3);
    LED_ON(LED4);
		}
		if(key==2)
		{
		LED_OFF(LED1);
    LED_OFF(LED2);
    LED_OFF(LED3);
    LED_OFF(LED4);
		}
		//if(key==2)
		//	Motor_status(5);
	}
}