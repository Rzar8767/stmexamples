#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


// requires spi.c
// requires onboard.c

void initSPI();
void SPIsendData(uint16_t data);
uint16_t SPIreceiveData();


int main(void)
{
	initOnBoardDiodes();
	//zad1();
	initSPI();
	uint16_t test = 15;

	SPIsendData(test);
	uint16_t test2 = SPIreceiveData();
	if(test2 == test)
		GPIO_SetBits(GPIOD, GPIO_Pin_12);


	while(1)
	{

	}

}



