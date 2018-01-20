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


uint16_t activeSeg = 1;
void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
    			GPIO_SetBits(GPIOE, activeSeg);
    			activeSeg = activeSeg << 1;
    			if(activeSeg > 32) activeSeg =1;
    			GPIO_ResetBits(GPIOE, activeSeg);


    			// NEVER LET THIS LINE GO
    			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}

void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{

            	GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
            	// wait a second
            	for(int i=0; i<10000000; i++);

            	GPIO_ToggleBits(GPIOE, GPIO_Pin_7);

            	// NEVER LET THIS LINE GO
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

void zad3();

int main(void)
{
	zad3();


	while(1)
	{

	}

}


void zad3()
{
	initExternDisplay();

	// set which part of LED screen you want to see the number on
	setDigit(1);

	GPIO_ResetBits(GPIOE, activeSeg);

	initTimerTime(125, TIM3, ENABLE);
	initTimerIRQ(TIM3, TIM3_IRQn, 1);

	// to have exercise 3 comment these 2 lines :)
	initTimerTime(5000, TIM4, ENABLE);
	initTimerIRQ(TIM4, TIM4_IRQn, 0);

	while(1)
	{}
}

