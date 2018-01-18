#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

//Timer

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
	TIM_TimeBaseStructure.TIM_Prescaler = presc;
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	TIM_Cmd(TIMx, state);
}

//timerState(TIM4, ENABLE);
void timerState(TIM_TypeDef* TIMx, FunctionalState state){
	TIM_Cmd(TIMx, state);
}

//int freq = 1000/4;
//initTime(freq,TIM4,ENABLE); wlaczy timer z czestotliwoscia 4Hz
//
//initTime(250, TIM4, ENABLE); wlaczy timer na 0.25s
//reinitialize to change time
void initTime(uint16_t miliseconds, TIM_TypeDef* TIMx, FunctionalState state)
{
	uint32_t period;
	period = (miliseconds*10)-1;

	initTimerAdv(8399, period, TIMx, state);
}

// Timer_IRQ(TIM4,TIM4_IRQn, 0);
void initTimerIRQ(TIM_TypeDef* TIMx, uint8_t TIMx_IRQn, uint8_t priority )
{
	// ustawienie trybu pracy priorytetów przerwañ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	// priorytet g³ówny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = priority;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kana³
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStructure);
	// wyczyszczenie przerwania od timera 3 (wyst¹pi³o przy konfiguracji timera)
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	// zezwolenie na przerwania od przepe³nienia dla timera 3
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
}

/* TIMER IRQHANDLER EXAMPLE
void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
            	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}*/

//PWM
