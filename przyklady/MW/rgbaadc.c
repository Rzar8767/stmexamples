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
#include "adcdac.c"

int ADC1_Result = 0;
int ADC1V_Value = 0;

int brightnes = 0;
void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
         		brightnes = ADC1V_Value/29.50;
         		setPWMWidthMiliseconds(TIM3,1,brightnes,10);
                TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}

int main(void)
{
	initGPIO(RCC_AHB1Periph_GPIOC,GPIOC,GPIO_Pin_6);

	initPWMTime(10,TIM3);

	initPWMChannel(TIM3,1,GPIOC,GPIO_PinSource6,GPIO_Pin_6);

	setPWMWidthMiliseconds(TIM3,1,1,10);

	initTimerIRQ(TIM3,TIM3_IRQn,0);

	initADC(RCC_APB2Periph_ADC1, GPIO_Pin_1);
	configADC(ADC1,ENABLE);

	startPWM(TIM3);

	for(;;) {
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		ADC1_Result = ADC_GetConversionValue(ADC1);
		ADC1V_Value=ADC1_Result*7204/10000; //mV
	}
}

