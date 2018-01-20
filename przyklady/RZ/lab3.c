
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void initOnBoardDiodes();
void initExternalDisplay();

void enableTimer(uint16_t presc, uint32_t period, uint32_t RCC_APB1Periph_TIMx, TIM_TypeDef* TIMx, FunctionalState state );
void zad2_part1();
void zad3_part1();



int main(void)
{
	//zad2_part1();
	zad3_part1();
	for(;;);
}

// konfiguracja wybranego timera i mruganie kropka
void zad3_part1()
{
	initExternalDisplay();
	enableTimer(8399, 9999, RCC_APB1Periph_TIM3, TIM3, ENABLE);
	uint32_t counter;
	GPIO_SetBits(GPIOA,  GPIO_Pin_8);
	while(1)
	{
		counter = TIM3->CNT;
		if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
			GPIO_ToggleBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		}
	}
}

// konfiguracja wybranego timera i mrugac diodami
void zad2_part1()
{
	initOnBoardDiodes();
	enableTimer(8399, 9999, RCC_APB1Periph_TIM3, TIM3, ENABLE);
	uint32_t counter;
	while(1)
	{
		counter = TIM3->CNT;
	//	if(counter > 3000)
		//	GPIO_Set(GPIOD, GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		}

	}
}

// wyzwlanie co sekunde timera 4: (uzywac na timerach 2-5 i 12-14)
//enableTimer(8399, 9999, RCC_APB1Periph_TIM4, TIM4, ENABLE);
void enableTimer(uint16_t presc, uint32_t period, uint32_t RCC_APB1Periph_TIMx, TIM_TypeDef* TIMx, FunctionalState state )
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

// piny PD12, PD13, PD14, PD15
void initOnBoardDiodes()
{
	 /* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;
    /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*
// piny PA8 do zasilania wyswietlacza
// piny PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7 for segments
// P0 = a itd.
     a
   f   b
	 g
   e   c
	 d   h
 */
void initExternalDisplay()
{
	 /* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
   /* Configure PA... in output pushpull mode */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

}
