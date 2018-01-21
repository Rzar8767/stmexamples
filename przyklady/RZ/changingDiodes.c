#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// requires timer.c
// requires onboard.c
// requires buttonVibrElim.c

void timerTest();
void setDiodes();

int seqIndex=0;

uint16_t diodes[7];

int timerMode =0;

void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
         		GPIO_ResetBits(GPIOD, diodes[seqIndex]);
         		++seqIndex;
         		if(seqIndex >= 7) seqIndex =0;
         		GPIO_SetBits(GPIOD, diodes[seqIndex]);
                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
         	}
}

int main(void)
{

	timerTest();
	for(;;)
	{

	}
}


void timerTest()
{
	initOnBoardDiodes();
	initOnBoardButton();
	initVibrElim();

	initTimerTime(500, TIM4, DISABLE);
	initTimerIRQ(TIM4, TIM4_IRQn, 0);

	// set the combinations
	setDiodes();


	// set first at the beginning
	GPIO_SetBits(GPIOD, diodes[seqIndex]);

	// run timer
	setTimerState(TIM4, ENABLE);
	while(1);
}

// 12 = 1, 13 = 2, 14 = 3, 15 = 4
void setDiodes()
{
	//- 1, 3, 4, 2, (sekwencja pusta),  1234, 34
	diodes[0] = GPIO_Pin_12;
	diodes[1] = GPIO_Pin_14;
	diodes[2] = GPIO_Pin_15;
	diodes[3] = GPIO_Pin_13;
	diodes[4] = 0;
	diodes[5] = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	diodes[6] = GPIO_Pin_14 | GPIO_Pin_15;
}


void onBoardButtonLogic()
{
	++timerMode;
	if(timerMode > 1) timerMode = 0;
	if(timerMode)
	{
		initTimerTime(250, TIM4, ENABLE);
	}
	else
	{
		initTimerTime(500, TIM4, ENABLE);
	}
	TIM_SetCounter(TIM4, 0);
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}
