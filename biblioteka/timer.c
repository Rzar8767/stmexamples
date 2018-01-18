#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void Timer_Config(uint16_t presc, uint32_t period, uint32_t RCC_APB1Periph_TIMx, TIM_TypeDef* TIMx, FunctionalState state)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE );
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //84mhz 84000000
		TIM_TimeBaseStructure.TIM_Prescaler = presc;
		TIM_TimeBaseStructure.TIM_Period = period;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
		TIM_Cmd(TIMx, state);
}
