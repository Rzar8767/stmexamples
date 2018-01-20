#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// requires timer.c
// requires externDisplay.c


void TIM2_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
         	{
            	GPIO_ToggleBits(GPIOE, GPIO_Pin_0);
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
         	}
}

void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
            	GPIO_ToggleBits(GPIOE, GPIO_Pin_6);
                TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}

void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
            	GPIO_ToggleBits(GPIOE, GPIO_Pin_3);
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

void zad2();

int main(void)
{
	zad2();


	while(1)
	{

	}

}


void zad2()
{
	initExternDisplay();
	setDigit(2);
	initTimerTime(500, TIM4, ENABLE);
	initTimerIRQ(TIM4, TIM4_IRQn, 0);

	initTimerTime(250, TIM3, ENABLE);
	initTimerIRQ(TIM3, TIM3_IRQn, 0);

	initTimerTime(125, TIM2, ENABLE);
	initTimerIRQ(TIM2, TIM2_IRQn, 0);

	while(1)
	{}
}
