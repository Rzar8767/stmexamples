#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


// requires usart.c


void usartSimple();

char input[5];
int inputINDEX=0;
char first[] = "First option";
void USART3_IRQHandler(void)
{
	// sprawdzenie flagi zwiazanej z odebraniem danych przez USART
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
       {
		char data = USARTreceiveData();
		input[inputINDEX] = data;
		++inputINDEX;
		if(inputINDEX > 2)
		{
			inputINDEX = 0;
			if(input[0] == '1' && input[1] == 'S' && input[2] == 'T')
				for(int i =0; ; ++i)
				{
					if(first[i] == 0) break;
					USARTsendData(first[i]);
				}


		}

		//USARTsendData(data);
       }
		// odebrany bajt znajduje sie w rejestrze USART3->DR
}


int main(void)
{
	usartSimple();

	for(;;)
	{

	}
}



void usartSimple()
{
	input[5] = 0;
	initUsart();
	initUsartIRQ(USART3, USART3_IRQn, 0);

	while(1);
}
