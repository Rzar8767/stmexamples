
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "adcdac.c"
#include "timer.c"

unsigned int ADC1_Result=0;

unsigned int ADC1V_Value=0;

int tablica[30];

void fillTable(int max)
{
	int digital = VoltToDigitalValue(max);
	for(int i=0; i < 10; ++i)
	{
		tablica[i] = digital/10 * i;
	}
	for(int i=10; i < 20; ++i)
		{
			tablica[i] = digital;
		}
	for(int i=20; i < 30; ++i)
	{
		tablica[i] = tablica[29-i];
	}

}

int index=0;
void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
         		++index;
         		if(index==30) index =0;

                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

         	}

}

int main(void)
{

	initADC(RCC_APB2Periph_ADC1, GPIO_Pin_1);
	configADC(ADC1, DISABLE);

	initDAC(GPIO_Pin_5);


	setDACvalue(GPIO_Pin_5, 0);

	startADC(ADC1);

	initTimerTime(100, TIM4, ENABLE);
	initTimerIRQ(TIM4, TIM4_IRQn,0);

	fillTable(2950);


	for(;;) {
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		setDACvalue(GPIO_Pin_5, tablica[index]);
		ADC1V_Value=getADCvoltage(ADC1);
	}

	/*
	 * PROGRAM GLOWNY - KONIEC
	 */
}




