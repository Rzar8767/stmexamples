
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// combination of pins required to set specific number
uint16_t numberSegment[10];



void initExternDisplay();
void setDigit(int num);
void displayNumber(int num);
void displayNothing();
void initNumbers();





// set digit of LED Display, choose from 1 to 4, negative version to reset segment
void setDigit(int num)
{
	switch(num)
	{
		case 1:
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_12);
			break;
		}
		case 2:
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_13);
			break;
		}
		case 3:
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_14);
			break;
		}
		case 4:
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_15);
			break;
		}
		case -1:
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_12);
			break;
		}
		case -2:
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_13);
			break;
		}
		case -3:
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
			break;
		}
		case -4:
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_15);
			break;
		}
		default:
		{
			GPIO_SetBits(GPIOE, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		}
	}
}

void initNumbers()
{
	numberSegment[0] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	numberSegment[1] = GPIO_Pin_1 | GPIO_Pin_2;
	numberSegment[2] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;
	numberSegment[3] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
	numberSegment[4] = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
	numberSegment[5] = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 |  GPIO_Pin_5 | GPIO_Pin_6;
	numberSegment[6] = GPIO_Pin_0 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	numberSegment[7] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	numberSegment[8] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	numberSegment[9] = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6;
}

// display digit on LED screen
void displayNumber(int num)
{
	if(num > 9 || num < 0)
		return;
	GPIO_SetBits(GPIOE, numberSegment[8]);
	GPIO_ResetBits(GPIOE, numberSegment[num]);
}

void displayNothing()
{
	GPIO_SetBits(GPIOE, numberSegment[8]);
}

/*
 Set active segments with LOW, set active screen with HIGH

PE0 = segment a
PE1 = segment b
PE2 = segment c
PE3 = segment d
PE4 = segment e
PE5 = segment f
PE6 = segment g
PE7 = segment h

PE12 = 1
PE13 = 2
PE14 = 3
PE15 = 4
 */
void initExternDisplay()
{
	 /* GPIOD Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;
   /* Configure PA... in output pushpull mode */
   GPIO_InitStructure.GPIO_Pin =
		   GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
		   GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOE, &GPIO_InitStructure);

   GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
   GPIO_ResetBits(GPIOE, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

   initNumbers();
}
