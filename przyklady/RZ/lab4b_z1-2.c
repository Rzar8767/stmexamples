#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


// requires externDisplay.c
// requires onboard.c

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

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
    	setTimerState(TIM2, ENABLE);

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void TIM2_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
         	{

         		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
         			onBoardButtonLogic();
         		setTimerState(TIM2, DISABLE);
         		TIM_SetCounter(TIM2, 0);

            	// DONT DELETE THIS LINE BY ACCIDENT
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
         	}
}

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
	initExtiIRQ(EXTI0_IRQn, EXTI_Line0,EXTI_PortSourceGPIOA, EXTI_PinSource0);

	initTimerTime(200, TIM2, DISABLE);
	initTimerIRQ(TIM2, TIM2_IRQn, 0);

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

