#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void initExternalButtons();
uint8_t readExternalButton(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


// pins PD0 = K0, PD1 = K1, PD2 = K2, PD3 = K3
void initExternalButtons()
{
	 /* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	 GPIO_InitTypeDef  GPIO_InitStructure;
	/* Configure PD0, PD1, PD2 and PD3 in input push down mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

// connected in Pull Up mode so requires a negation of bits because 0 is the high (button pressed) state
// example: readExternalButton(GPIOD, GPIO_Pin_0)
uint8_t readExternalButton(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	return !GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}
