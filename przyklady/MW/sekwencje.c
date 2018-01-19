/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "onboard.c"
#include "timer.c"

int index = 5;
int direction = 1;
uint16_t sequence[6];

void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
         		GPIO_ResetBits(GPIOD, sequence[index]);
         		index += direction;
         		if(index > 5)
         			index = 0;
         		else if( index < 0)
         			index = 5;
         		GPIO_SetBits(GPIOD, sequence[index]);

                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

void fillTable(){
	sequence[0] = GPIO_Pin_12;
	sequence[1] = GPIO_Pin_14;
	sequence[2] = GPIO_Pin_13;
	sequence[3] = 0x0;
	sequence[4] = GPIO_Pin_12 | GPIO_Pin_13;
	sequence[5] = GPIO_Pin_14 | GPIO_Pin_15;
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
    	if(direction == 1)//changes sequence direction
    		direction = -1;
    	else
    		direction = 1;

    	initTimerTime(500,TIM4,ENABLE);//shorter sequence time periods
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
int main(void)
{
	initOnBoardDiodes();
	initOnBoardButton();

	fillTable();

	initTimerTime(1000,TIM4,ENABLE);
	initTimerIRQ(TIM4,TIM4_IRQn, 0);

	initOnBoardButtonIRQ();

	for(;;){}
}
