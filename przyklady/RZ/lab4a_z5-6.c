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


int activeNum = 1;
void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
    			displayNumber(activeNum);
    			++activeNum;
    			if(activeNum > 9) activeNum =0;

    			// NEVER LET THIS LINE GO
    			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}

int activeDisplay = 1;
void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{

            	setDisplay(-1*activeDisplay);
            	if(activeDisplay >= 4) activeDisplay =0;
            	++activeDisplay;
            	setDisplay(activeDisplay);

            	// NEVER LET THIS LINE GO
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

void zad5();

int main(void)
{
	zad5();


	while(1)
	{

	}

}


void zad5()
{
	initExternDisplay();

	// set which part of LED screen you want to see the number on
	setDigit(1);

	displayNumber(0);

	initTimerTime(1000, TIM3, DISABLE);
	initTimerIRQ(TIM3, TIM3_IRQn, 4);

	initTimerTime(1000, TIM4, DISABLE);
	initTimerIRQ(TIM4, TIM4_IRQn, 4);

	setTimerState(TIM3, ENABLE);
	// comment this line to have exercies 5 :)
	setTimerState(TIM4, ENABLE);
	while(1)
	{}
}

