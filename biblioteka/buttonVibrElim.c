#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

//requires onboard.c
//requires timer.c

void onBoardButtonLogic();
void initVibrElim();


// CONFIGURED FOR THE BLUE BUTTON ON THE BOARD
// USES TIMER2 and EXT0
// CONFIGURE FUNCTION onBoardButtonLogic(); FOR THE BEHAVIOR WHEN BUTTON IS PRESSED
// PRIOR TO USING initVibrElim(); REMEMBER TO USE initOnBoardButton(); !

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
    	setTimerState(TIM2, ENABLE);

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void TIM2_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
         	{

         		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
         			onBoardButtonLogic();
         		setTimerState(TIM2, DISABLE);
         		TIM_SetCounter(TIM2, 0);

            	// DONT DELETE THIS LINE BY ACCIDENT
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
         	}
}

void initVibrElim()
{
	initExtiIRQ(EXTI0_IRQn, EXTI_Line0,EXTI_PortSourceGPIOA, EXTI_PinSource0);

	// the time you are required to press the button for it to be a valid press, in ms
	initTimerTime(20, TIM2, DISABLE);
	initTimerIRQ(TIM2, TIM2_IRQn, 0);
}

// here's what the button is supposed to do, define it in your file
/*
void onBoardButtonLogic()
{

}
*/
