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

int direction = 1;
int brightness = 6;

void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		setPWMWidthMiliseconds(TIM4,1,brightness, 2);
		brightness+= direction;
		if(brightness>99 || brightness <6){
			if(direction == 1)
			    		direction = -1;
			 else
			    direction = 1;
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

int main(void)
{
	initOnBoardDiodes();

	initPWMTime(2, TIM4);

	initPWMChannel(TIM4,1,GPIOD,GPIO_PinSource12,GPIO_Pin_12);

	startPWM(TIM4);

	initTimerTime(10, TIM3, ENABLE);
	initTimerIRQ(TIM3, TIM3_IRQn, 0);

	for(;;){

	}
}
