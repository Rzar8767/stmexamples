#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// example:	initTimerAdv(8399, 9999, RCC_APB1Periph_TIM4, TIM4, ENABLE);  - wyzwalanie co sekunde timera4
void initTimerAdv(uint16_t presc, uint32_t period, TIM_TypeDef* TIMx, FunctionalState state)
{
	uint32_t RCC_APB1Periph_TIMx;
	if(TIMx == TIM2)
		  	  RCC_APB1Periph_TIMx = RCC_APB1Periph_TIM2;
	else if(TIMx ==  TIM3)
	  		  RCC_APB1Periph_TIMx = RCC_APB1Periph_TIM3;
	else if(TIMx ==  TIM4)
	  		  RCC_APB1Periph_TIMx = RCC_APB1Periph_TIM4;
	else if(TIMx ==  TIM5)
	  		  RCC_APB1Periph_TIMx = RCC_APB1Periph_TIM5;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE );
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //84mhz 84000000
	TIM_TimeBaseStructure.TIM_Prescaler = presc;		TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	TIM_Cmd(TIMx, state);
}

// example: initTime(250, TIM4, ENABLE); wlaczy timer na 0.25s
void initTime(uint16_t miliseconds, TIM_TypeDef* TIMx, FunctionalState state)
{
	uint32_t period;
	period = (miliseconds*10)-1;

	initTimerAdv(8399, period, TIMx, ENABLE);
}
