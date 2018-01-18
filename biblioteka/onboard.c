
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void initOnBoardDiodes();
void GPIO_Config(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins);
void GPIO_ConfigIO(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins, GPIOMode_TypeDef Mode, GPIOPuPd_TypeDef PuPd);

// PD12 = GREEN; PD13 = ORANGE; PD14 = RED; PD15 = BLUE;
// piny PD12, PD13, PD14, PD15
void initOnBoardDiodes() {
	GPIO_Config(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

// Enables GPIO in input/output mode
// GPIO_Config(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_12 | GPIO_Pin_13,GPIO_Mode_OUT,GPIO_PuPd_NOPULL);
void GPIO_ConfigIO(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins, GPIOMode_TypeDef Mode, GPIOPuPd_TypeDef PuPd){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = Pins;
	GPIO_InitStructure.GPIO_Mode = Mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = PuPd;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

// Enables GPIO in output mode
// GPIO_Config(RCC_AHB1Periph_GPIOD,GPIOD,GPIO_Pin_12 | GPIO_Pin_13);
void GPIO_Config(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins){
			GPIO_ConfigIO(RCC_AHB1Periph_GPIOx,GPIOx,Pins, GPIO_Mode_OUT,GPIO_PuPd_NOPULL);
}

