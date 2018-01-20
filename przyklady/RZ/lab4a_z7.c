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



int activeNumDis[4];

void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{

    			++activeNumDis[0];
    			for(int i=0; i < 3; ++i)
    			{
					if(activeNumDis[i] > 9)
						{
						activeNumDis[i] =0;
						++activeNumDis[i+1];
						}
					else
						break;
    			}
    			// overflow of the counter
    			if(activeNumDis[3] > 9)
    				activeNumDis[3] = 0;

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
            	displayNumber(activeNumDis[4-activeDisplay]);
            	setDisplay(activeDisplay);

            	// NEVER LET THIS LINE GO
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

void zad7();

int main(void)
{
	zad7();


	while(1)
	{

	}

}


void zad7()
{
	initExternDisplay();

	// set which part of LED screen you want to see the number on
	setDigit(1);

	// in charge of changing numbers
	initTimerTime(10, TIM3, DISABLE);
	initTimerIRQ(TIM3, TIM3_IRQn, 4);

	// in charge of displaying numbers
	initTimerPresc(8399, 24, TIM4, DISABLE);
	initTimerIRQ(TIM4, TIM4_IRQn, 4);

	activeNumDis[0] = 0;
	activeNumDis[1] = 0;
	activeNumDis[2] = 0;
	activeNumDis[3] = 0;

	setTimerState(TIM3, ENABLE);
	setTimerState(TIM4, ENABLE);
	while(1)
	{}
}

