#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

//Timer
void initTimerAdv(uint16_t presc, uint32_t period, TIM_TypeDef* TIMx, FunctionalState state);
void initTimerTime(uint16_t miliseconds, TIM_TypeDef* TIMx, FunctionalState state);
void initTimerIRQ(TIM_TypeDef* TIMx, uint8_t TIMx_IRQn, uint8_t priority );
void setTimerState(TIM_TypeDef* TIMx, FunctionalState state);

//PWM
void initPWM(uint32_t period, TIM_TypeDef* TIMx);
void initPWMTime(uint32_t miliseconds, TIM_TypeDef* TIMx);
void initPWMChannel(TIM_TypeDef* TIMx, uint8_t channel, GPIO_TypeDef* GPIOx,uint8_t GPIO_PinSourcex,uint32_t GPIO_Pin_x);
void setPWMWidthPeriod(TIM_TypeDef* TIMx, uint8_t channel, uint8_t percent, uint16_t period);
void setPWMWidthMiliseconds(TIM_TypeDef* TIMx, uint8_t channel, uint8_t percent, uint16_t miliseconds);
void startPWM(TIM_TypeDef* TIMx);

//////////////////////////////////////          TIMER          /////////////////////////////////////////////

// unsigned int counter = TIMx->CNT; reading timer value

//First of all you have to init your Timer, this one uses prescaler and period
//initTimerAdv(8399, 9999, TIM4, ENABLE);  - initialize timer for 1s
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
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up; //counter direction
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	TIM_Cmd(TIMx, state);
}

//enable or disable timer
//setTimerState(TIM4, ENABLE);
void setTimerState(TIM_TypeDef* TIMx, FunctionalState state){
	TIM_Cmd(TIMx, state);
}

//////    FREQUENCY   ///////////
//int freq = 1000/4;
//initTime(freq,TIM4,ENABLE); initialize timer with 4Hz frequency

//First of all you have to init your Timer, this one uses miliseconds
//initTimerTime(250, TIM4, ENABLE); - initialize timer for 0.25s
void initTimerTime(uint16_t miliseconds, TIM_TypeDef* TIMx, FunctionalState state)
{
	uint32_t period;
	period = (miliseconds*10)-1;

	initTimerAdv(8399, period, TIMx, state);
}

//Here you can set timer interrupt
//initTimerIRQ(TIM4,TIM4_IRQn, 0);
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

//////////////////////////////////////          PWM          /////////////////////////////////////////////

//     || Remember to INITIALIZE GPIO
//     ||  First of all you have to initialize PWM Timer
//	   \/  initPWM(9999, TIM4);
void initPWM(uint32_t period, TIM_TypeDef* TIMx) {
	initTimerAdv(8399, period, TIMx, DISABLE);
}

//     || Remember to INITIALIZE GPIO
//     ||  First of all you have to initialize PWM Timer
//	   \/  initPWMTime(100, TIM4);
void initPWMTime(uint32_t miliseconds, TIM_TypeDef* TIMx) {
	initTimerTime(miliseconds, TIMx, DISABLE);
}

//Then you have to initialize PWM Channel (there are only 4 timer channels)
//initPWMChannel(TIM4,1,GPIOD,GPIO_PinSource12,GPIO_Pin_12);
void initPWMChannel(TIM_TypeDef* TIMx, uint8_t channel, GPIO_TypeDef* GPIOx,uint8_t GPIO_PinSourcex,uint32_t GPIO_Pin_x){

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/* PWM1 Mode configuration: */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	if(channel == 1){
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if(channel == 2){
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if(channel == 3){
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}
	else if(channel == 4){
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
	}

	GPIO_PinAFConfig(GPIOx, GPIO_PinSourcex, GPIO_AF_TIM4);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

}

//You can set PWM channel width in percent whenever you want (but for sure after PWM init), this one uses period
//setPWMWidthPeriod(TIM4,1,20, 9999);
void setPWMWidthPeriod(TIM_TypeDef* TIMx, uint8_t channel, uint8_t percent, uint16_t period){
	if(channel == 1){
		TIMx->CCR1 = (percent*(period+1))/100;

	}
	else if(channel == 2){
		TIMx->CCR2 = (percent*(period+1))/100;
	}
	else if(channel == 3){
		TIMx->CCR3 = (percent*(period+1))/100;
	}
	else if(channel == 4){
		TIMx->CCR4 = (percent*(period+1))/100;
	}
}

//You can set PWM channel width in percent whenever you want (but for sure after PWM init), this one uses period
//setPWMWidthMiliseconds(TIM4,1,20, 100);
void setPWMWidthMiliseconds(TIM_TypeDef* TIMx, uint8_t channel, uint8_t percent, uint16_t miliseconds){
	setPWMWidthPeriod(TIMx,channel,percent,(miliseconds*10)-1);
}

//When everything with PWM is ready, you can start it
//startPWM(TIM4);
void startPWM(TIM_TypeDef* TIMx){
	TIM_ARRPreloadConfig(TIMx,ENABLE);
	TIM_Cmd(TIMx, ENABLE);
}
