
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void initOnBoardDiodes();
void initGPIO(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins);
void initGPIOadv(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins, GPIOMode_TypeDef Mode, GPIOPuPd_TypeDef PuPd);

// PD12 = GREEN; PD13 = ORANGE; PD14 = RED; PD15 = BLUE;
// pins PD12, PD13, PD14, PD15
void initOnBoardDiodes() {
	initGPIO(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

// to check the state
// GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
// 1 when pushed
void initOnBoardButton() {
	initGPIOadv(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_0, GPIO_Mode_IN, GPIO_PuPd_NOPULL);
}

// Initializes GPIO in input/output mode
// initGPIOadv(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_12 | GPIO_Pin_13,GPIO_Mode_OUT,GPIO_PuPd_NOPULL);
void initGPIOadv(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins, GPIOMode_TypeDef Mode, GPIOPuPd_TypeDef PuPd){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Pins;
	GPIO_InitStructure.GPIO_Mode = Mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = PuPd;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

// Initializes GPIO in output mode
// initGPIO(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_12 | GPIO_Pin_13);
void initGPIO(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins){
	initGPIOadv(RCC_AHB1Periph_GPIOx,GPIOx,Pins, GPIO_Mode_OUT,GPIO_PuPd_NOPULL);
}

