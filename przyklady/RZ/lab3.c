#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// requires timer.c
// requires onboard.c
// requires externDisplay.c
// requires externButtons.c

void initOnBoardDiodes();
void initOnBoardButton();

void initExternDisplay();
void setDigit(int num);
void displayNumber(int num);
void initNumbers();

//PWM
void initPWM(uint32_t period, TIM_TypeDef* TIMx);
void initPWMTime(uint32_t miliseconds, TIM_TypeDef* TIMx);
void initPWMChannel(TIM_TypeDef* TIMx, uint8_t channel, GPIO_TypeDef* GPIOx,uint8_t GPIO_PinSourcex,uint32_t GPIO_Pin_x);
void setPWMWidthPeriod(TIM_TypeDef* TIMx, uint8_t channel, uint8_t percent, uint16_t period);
void setPWMWidthMiliseconds(TIM_TypeDef* TIMx, uint8_t channel, uint8_t percent, uint16_t miliseconds);
void startPWM(TIM_TypeDef* TIMx);

void initTimerTime(uint16_t miliseconds, TIM_TypeDef* TIMx, FunctionalState state);

void zad2_part1();
void zad3_part1();
void zad4_part1();
void zad5_part1();

void zad1_part2();
void zad2_part2();
void zad3_part2();
void zad4_part2();

void numbersFrom0To9();

// te takie plywajace po LED w kolo
void swimmingSomething();

int main(void)
{
	//zad2_part1();
	//zad3_part1();

	//swimmingSomething();
	//numbersFrom0To9();
	//zad2_part1();
	//zad3_part1();
	//zad4_part1();
	//zad5_part1();

	//zad1_part2();
	//zad2_part2();
	//zad3_part2();
	zad4_part2();
	while(1)
	{

	}

}

void numbersFrom0To9()
{
	initOnBoardButton();

	initExternDisplay();

	// set which part of LED screen you want to see the number on
	setDigit(1);
	setDigit(2);
	setDigit(3);
	setDigit(4);

	int buttonstate_o = 0;
	int buttonstate=0;

	int activeNum = 0;
	displayNumber(activeNum);
	++activeNum;
	while(1)
	{
		buttonstate = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		// when button pressed, contact oscillation not eliminated
		if(buttonstate == 1 && buttonstate_o == 0)
		{
			displayNumber(activeNum);
			++activeNum;
			if(activeNum > 9) activeNum =0;
		}
		buttonstate_o = buttonstate;
	}
}

void swimmingSomething()
{
	initOnBoardButton();

	initExternDisplay();

	// set which part of LED screen you want to see the number on
	setDigit(1);


	int buttonstate_o = 0;
	int buttonstate=0;

	uint16_t activeSeg = 1;
	GPIO_ResetBits(GPIOE, activeSeg);
	while(1)
	{
		buttonstate = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
		// when button pressed, contact oscillation not eliminated
		if(buttonstate == 1 && buttonstate_o == 0)
		{
			GPIO_SetBits(GPIOE, activeSeg);
			activeSeg = activeSeg << 1;
			if(activeSeg > 32) activeSeg =1;
			GPIO_ResetBits(GPIOE, activeSeg);

		}
		buttonstate_o = buttonstate;
	}
}








// konfiguracja wybranego timera i mrugac diodami co sekunde
void zad2_part1()
{
	initOnBoardDiodes();
	initTimerTime(1000, TIM3, ENABLE);
	while(1)
	{
		//TIM3-> CCR1;
		if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
			GPIO_ToggleBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		}

	}
}

// konfiguracja wybranego timera i mruganie kropka co sekunde
void zad3_part1()
{
	initExternDisplay();
	initTimerTime(1000, TIM3, ENABLE);
	setDigit(2);
	while(1)
	{
		if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
			GPIO_ToggleBits(GPIOE,  GPIO_Pin_7);
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		}
	}
}

// symulacja PWM na diodach bez PWM
void zad4_part1()
{
	initOnBoardDiodes();
	initTimerTime(1000, TIM3, ENABLE);

	uint8_t D1passed =0;
	uint8_t D2passed =0;
	uint8_t D3passed =0;

	while(1)
	{
		// 0.5 swieci
		if(TIM3->CNT > 4999 && !D1passed)
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_12);
			D1passed = 1;
		}
		// 0.2 swieci
		if(TIM3->CNT > 1999 && !D2passed)
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			D2passed = 1;
		}
		// 0.7 swieci
		if(TIM3->CNT > 6999 && !D3passed)
		{
			GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			D3passed = 1;
		}
		if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)) {
			GPIO_SetBits(GPIOD,  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
			D1passed = 0;
			D2passed = 0;
			D3passed = 0;
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		}
	}
}

// wyswietlanie kolejnych cyfr co 0.5s
void zad5_part1()
{

	initExternDisplay();

	initTimerTime(500,TIM3,ENABLE);

	// set which part of LED screen you want to see the number on
	setDigit(1);
	setDigit(2);
	setDigit(3);
	setDigit(4);

	int activeNum = 0;
	displayNumber(activeNum);
	++activeNum;
	while(1)
	{

		// change number
		if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update))
		{
			displayNumber(activeNum);
			++activeNum;
			if(activeNum > 9) activeNum =0;
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		}

	}
}

// swiecenie diodami za pomoca PWM, pierwsza 0.5s druga 0.2s, trzecia 0.7  na sekunde
void zad1_part2()
{
	initOnBoardDiodes();

	initPWMTime(1000, TIM4);

	// TIM4 CH1 to dioda LED 12
	initPWMChannel(TIM4, 1, GPIOD, GPIO_PinSource12, GPIO_Pin_12);
	setPWMWidthMiliseconds(TIM4, 1, 50 , 1000);

	// TIM4 CH2 to dioda LED 13
	initPWMChannel(TIM4, 2, GPIOD, GPIO_PinSource13, GPIO_Pin_13);
	setPWMWidthMiliseconds(TIM4, 2, 20 , 1000);

	// TIM4 CH3 to dioda LED 14
	initPWMChannel(TIM4, 3, GPIOD, GPIO_PinSource14, GPIO_Pin_14);
	setPWMWidthMiliseconds(TIM4, 3, 70 , 1000);

	startPWM(TIM4);

	while(1)
	{}
}


// swiecenie dioda za pomoca PWM, steruje jej jasnoscia, 1kHz
void zad2_part2()
{
	initOnBoardDiodes();

	//int hz = 1000;
	//int freq = 1000/hz;

	//initTimer
	initPWMTime(1, TIM4);
	initTimerTime(100, TIM3, ENABLE);


	initPWMChannel(TIM4, 1, GPIOD, GPIO_PinSource12, GPIO_Pin_12);
	//setPWMWidthMiliseconds(TIM4, 1, 44 , freq-1);

	TIM4->CCR1 = 0;
	startPWM(TIM4);

	int dir=1;
	int setCCR = 1;
	while(1)
	{
     	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) != RESET)
     	{
        	TIM4->CCR1 = setCCR;
        	// 9 is max brightness, 0 is off
        	if(setCCR > 8 || setCCR < 1)
        	{
        		dir *= -1;

        	}
        	setCCR += dir;
        	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
     	}
	}
}

// swiecenie dioda RGB za pomoca PWM, steruje jej jasnoscia, 1kHz
void zad3_part2()
{
	initGPIO(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);


	//int hz = 1000;
	//int freq = 1000/hz;

	// 1kHz, can configure PWM from 1 to 100
	initTimerPresc(839, 99, TIM3, DISABLE);

	// RGB diode R
	initPWMChannel(TIM3, 1, GPIOC, GPIO_PinSource6, GPIO_Pin_6);

	// RGB diode G
	initPWMChannel(TIM3, 2, GPIOC, GPIO_PinSource7, GPIO_Pin_7);

	// RGB diode B
	initPWMChannel(TIM3, 3, GPIOC, GPIO_PinSource8, GPIO_Pin_8);


	TIM3->CCR1 = 10;
	TIM3->CCR2 = 50;
	TIM3->CCR3 = 75;
	startPWM(TIM3);

	int bDir =-1;

	while(1)
	{
	}
}

// swiecenie dioda RGB za pomoca PWM, steruje jej jasnoscia, 1kHz, zmiana jasnosci co 10ms
void zad4_part2()
{
	initGPIO(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8);


	//int hz = 1000;
	//int freq = 1000/hz;

	// 1kHz, can configure PWM from 1 to 100
	initTimerPresc(839, 99, TIM3, DISABLE);

	// RGB diode R
	initPWMChannel(TIM3, 1, GPIOC, GPIO_PinSource6, GPIO_Pin_6);

	// RGB diode G
	initPWMChannel(TIM3, 2, GPIOC, GPIO_PinSource7, GPIO_Pin_7);

	// RGB diode B
	initPWMChannel(TIM3, 3, GPIOC, GPIO_PinSource8, GPIO_Pin_8);


	TIM3->CCR1 = 10;
	TIM3->CCR2 = 50;
	TIM3->CCR3 = 75;
	startPWM(TIM3);
	int current = 0;
	const int change = 10;
	int rDir =1;
	int gDir =1;
	int bDir =-1;

	while(1)
	{
     	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update) != RESET)
     	{
			if(current > change)
			{

				if(TIM3->CCR1 > 99 || TIM3->CCR1 < 1)
					rDir *= -1;
				if(TIM3->CCR2 > 99 || TIM3->CCR2 < 1)
					gDir *= -1;
				if(TIM3->CCR3 > 99 || TIM3->CCR3 < 1)
					bDir *= -1;

				TIM3->CCR1 += rDir;
				TIM3->CCR2 += gDir;
				TIM3->CCR3 += bDir;

				current = 0;
			}
			++current;
			TIM_ClearFlag(TIM3, TIM_FLAG_Update);
     	}
	}
}


