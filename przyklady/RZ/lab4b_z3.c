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

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
    	setTimerState(TIM3, DISABLE);
    	GPIO_ResetBits(GPIOE, GPIO_Pin_7);
    	setTimerState(TIM2, ENABLE);


        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void TIM2_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
         	{
         		setTimerState(TIM2, DISABLE);
         		TIM_SetCounter(TIM2, 0);
         		GPIO_SetBits(GPIOE, GPIO_Pin_7);


         		setTimerState(TIM3, ENABLE);
            	// DONT DELETE THIS LINE BY ACCIDENT
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
         	}
}

uint16_t activeSeg = 1;
void TIM3_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
         	{
    			GPIO_SetBits(GPIOE, activeSeg);
    			activeSeg = activeSeg << 1;
    			if(activeSeg > 32) activeSeg =1;
    			GPIO_ResetBits(GPIOE, activeSeg);


    			// NEVER LET THIS LINE GO
    			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
         	}
}

void zad3();

int main(void)
{
	zad3();


	while(1)
	{

	}

}

// move onboardButtonLogic() to EXTI0 and get rid of setTimerState in it to have exercise 1 :)
void zad3()
{
	initExternDisplay();
	setDisplay(1);
	initOnBoardButton();

	GPIO_ResetBits(GPIOE, activeSeg);

	// initialize the 1s pause timer
	initTimerTime(1000, TIM2, DISABLE);
	initTimerIRQ(TIM2, TIM2_IRQn, 0);

	// initialize EXTI IRQ for the blue button
	initExtiIRQ(EXTI0_IRQn, EXTI_Line0,EXTI_PortSourceGPIOA, EXTI_PinSource0);

	// set timer3 to 0,125s
	initTimerTime(125, TIM3, ENABLE);
	initTimerIRQ(TIM3, TIM3_IRQn, 1);

	while(1)
	{}
}


