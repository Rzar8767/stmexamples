
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
			
void initOnBoardDiodes();
void initExternalButtons();
uint8_t readExternalButton(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);


void zad2_init();
void zad2_loop();
void zad3_init();
void zad3_loop();
void zad4();
void zad5();

// zad2 zad3
uint8_t button0;
uint8_t button1;
uint8_t button2;
uint8_t button3;

// zad 3
uint8_t button0_toggle;
uint8_t button1_toggle;
uint8_t button2_toggle;
uint8_t button3_toggle;

// zad3
uint8_t button0_prevState =0;
uint8_t button1_prevState =0;
uint8_t button2_prevState =0;
uint8_t button3_prevState =0;

int main(void)
{
	//zad2_init();
	//zad3_init();
	//zad4();
	zad5();

	for(;;)
	{
		//zad2_loop();
		//zad3_loop();
	}
}

// sterowanie diodami w oparciu o czas, bez timerow na wyczucie od 12 do 15, przycisk K0
void zad5()
{
	initOnBoardDiodes();
	initExternalButtons();
	uint8_t button_prevState =0;
	uint16_t diodes[4];
	uint8_t current = 0;
	diodes[0] = GPIO_Pin_12;
	diodes[1] = GPIO_Pin_13;
	diodes[2] = GPIO_Pin_14;
	diodes[3] = GPIO_Pin_15;
	while(1)
	{
		button0 = readExternalButton(GPIOD, GPIO_Pin_0);
		// zmien stan diody w momencie wcisniecia
		if(button_prevState == 0 && button0 == 1)
		{
			do{
			for(int i =0; i < 10000000; ++i);
			button_prevState = button0;
			button0 = readExternalButton(GPIOD, GPIO_Pin_0);

			if(button_prevState == 1 && button0 == 1)
			{
				if(current == 4)
				{
					current = 0;
				}
				else
				{
					GPIO_SetBits(GPIOD, diodes[current]);
					++current;
				}
			}
			}while(button_prevState == 1 && button0 == 1);
		}
		else
		{
			GPIO_ResetBits(GPIOD, diodes[0] | diodes[1] | diodes[2] | diodes[3]);
			current = 0;
		}

		button_prevState = button0;
	}
}

// zalaczanie diod po kolei od 12 do 15, nastepna dioda po wcisnieciu przycisku ponownie, przycisk K0
void zad4()
{
	initOnBoardDiodes();
	initExternalButtons();
	uint8_t button_prevState =0;
	uint16_t diodes[4];
	uint8_t current = 0;
	diodes[0] = GPIO_Pin_12;
	diodes[1] = GPIO_Pin_13;
	diodes[2] = GPIO_Pin_14;
	diodes[3] = GPIO_Pin_15;
	while(1)
	{
		button0 = readExternalButton(GPIOD, GPIO_Pin_0);
		// zmien stan diody w momencie wcisniecia
		if(button_prevState == 0 && button0 == 1)
		{
			if(current == 4)
			{
				GPIO_ResetBits(GPIOD, diodes[0] | diodes[1] | diodes[2] | diodes[3]);
				current = 0;
			}
			else
			{
				GPIO_SetBits(GPIOD, diodes[current]);
				++current;
			}
		}


		button_prevState = button0;
	}
}

void zad3_init()
{
	initOnBoardDiodes();
	initExternalButtons();
}
void zad3_loop()
{
	button0 = readExternalButton(GPIOD, GPIO_Pin_0);
	button1 = readExternalButton(GPIOD, GPIO_Pin_1);
	button2 = readExternalButton(GPIOD, GPIO_Pin_2);
	button3 = readExternalButton(GPIOD, GPIO_Pin_3);

	// zmien stan diody w momencie wcisniecia
	if(button0_prevState == 0 && button0 == 1)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
	}
	if(button1_prevState == 0 && button1 == 1)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	}
	if(button2_prevState == 0 && button2 == 1)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
	}
	if(button3_prevState == 0 && button3 == 1)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
	}

	button0_prevState = button0;
	button1_prevState = button1;
	button2_prevState = button2;
	button3_prevState = button3;
}

void zad2_init()
{
	initOnBoardDiodes();
	initExternalButtons();

}

void zad2_loop()
{
	button0 = readExternalButton(GPIOD, GPIO_Pin_0);
	button1 = readExternalButton(GPIOD, GPIO_Pin_1);
	button2 = readExternalButton(GPIOD, GPIO_Pin_2);
	button3 = readExternalButton(GPIOD, GPIO_Pin_3);
	if(button0)
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	else
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	if(button1)
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
	else
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	if(button2)
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
	else
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	if(button3)
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
	else
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);

}

void initOnBoardDiodes()
{
	 /* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;
    /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

// piny PD0 = K0, PD1 = K1, PD2 = K2, PD3 = K3
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

// pod³¹czone jak Pull Up wiêc nalezy zanegowac dla bardziej logicznych stanow
uint8_t readExternalButton(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	return !GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}

