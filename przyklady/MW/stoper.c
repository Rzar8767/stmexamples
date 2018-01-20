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
#include "externDisplay.c"

int position = 1;
int jednosci = 0;
int dziesiatki = 0;
int setki = 0;
int tysiace = 0;

void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
         		jednosci++;
         		if(jednosci>9){
         			dziesiatki++;
         			jednosci = 0;
         		}
         		if(dziesiatki>9){
         			setki++;
         		    dziesiatki = 0;
         		}
         		if(setki>9){
         			tysiace++;
         			setki = 0;
         		}

                TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}

void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
         		displayOnePosition(position);
         		if(position == 4){
         			displayNumber(jednosci);
         		}
         		else if(position == 3){
         		         			displayNumber(dziesiatki);
         		         		}
         		else if(position == 2){
         		         			displayNumber(setki);
         		         		}
         		else if(position == 1){
         		         			displayNumber(tysiace);
         		         		}

         		position++;
         		    	         		if(position>4)position =1;
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

int main(void)
{
	initExternDisplay();
	initTimerTime(1,TIM4, ENABLE);
	initTimerIRQ(TIM4,TIM4_IRQn,0);

	initTimerTime(100,TIM3, ENABLE);
		initTimerIRQ(TIM3,TIM3_IRQn,0);
	//initOnBoardButton();
	//initOnBoardButtonIRQ();
	//setDigit(0);

	for(;;){}
}
