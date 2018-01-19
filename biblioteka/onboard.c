
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void initOnBoardDiodes();
void initOnBoardButton();
void initOnBoardButtonIRQ();
void initGPIO(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins);
void initGPIOadv(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins, GPIOMode_TypeDef Mode, GPIOPuPd_TypeDef PuPd);
void initExtiIRQ(uint8_t EXTx_IRQn, uint32_t EXTI_Linex,uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex);

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
//   initExtiIRQ(EXTI0_IRQn, EXTI_Line0,EXTI_PortSourceGPIOA, EXTI_PinSource0); uruchomienie przerwania User Buttona
void initExtiIRQ(uint8_t EXTx_IRQn, uint32_t EXTI_Linex,uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex){

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = EXTx_IRQn;
	// priorytet g³ówny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kana³
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStructure);

	EXTI_InitTypeDef EXTI_InitStructure;
	// wybór numeru aktualnie konfigurowanej linii przerwañ
	EXTI_InitStructure.EXTI_Line = EXTI_Linex;
	// wybór trybu - przerwanie b¹dŸ zdarzenie
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	// wybór zbocza, na które zareaguje przerwanie
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	// uruchom dan¹ liniê przerwañ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	// zapisz strukturê konfiguracyjn¹ przerwañ zewnêtrznych do rejestrów
	EXTI_Init(&EXTI_InitStructure);

	// pod³¹czenie danego pinu portu do kontrolera przerwañ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);
}

void initOnBoardButtonIRQ(){
	initExtiIRQ(EXTI0_IRQn, EXTI_Line0,EXTI_PortSourceGPIOA, EXTI_PinSource0);
}
/* Onboard button IRQ example
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
    	//code
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}*/
