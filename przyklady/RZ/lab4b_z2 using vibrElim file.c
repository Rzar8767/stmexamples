#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


// requires externDisplay.c
// requires onboard.c
// requires buttonVibrElim.c

void initExternDisplay();
void setDigit(int num);
void displayNumber(int num);
void displayNothing();
void initNumbers();
void initOnBoardDiodes();
void initOnBoardButton();
void initOnBoardButtonIRQ();
void initGPIO(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins);
void initGPIOadv(uint32_t RCC_AHB1Periph_GPIOx,GPIO_TypeDef* GPIOx , uint32_t Pins, GPIOMode_TypeDef Mode, GPIOPuPd_TypeDef PuPd);
void initExtiIRQ(uint8_t EXTx_IRQn, uint32_t EXTI_Linex,uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex);


int display = 1;
void onBoardButtonLogic();

void zad2();

int main(void)
{
	zad2();


	while(1)
	{

	}

}

// move onboardButtonLogic() to EXTI0 and get rid of setTimerState in it to have exercise 1 :)
void zad2()
{
	initExternDisplay();
	setDisplay(1);
	displayNumber(8);
	initOnBoardButton();
	initVibrElim();

	while(1)
	{}
}

// here's what the button is supposed to do
void onBoardButtonLogic()
{
	setDisplay(-display);
	++display;
	if(display > 2) display = 1;
	setDisplay(display);
}

