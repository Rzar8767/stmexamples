
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void USARTsendData(uint16_t data);
uint8_t USARTreceiveData();

void initUsart(void)
{
	// wlaczenie taktowania wybranego portu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// wlaczenie taktowania wybranego uk³adu USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// konfiguracja linii Rx i Tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// ustawienie funkcji alternatywnej dla pinów (USART)
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
	USART_InitTypeDef USART_InitStructure;

	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400, 57600, 115200, ...)
	USART_InitStructure.USART_BaudRate = 9600;
	// d³ugoœæ s³owa (USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5, USART_StopBits_2, USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// sprawdzanie parzystoœci (USART_Parity_No, USART_Parity_Even, USART_Parity_Odd)
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// sprzêtowa kontrola przep³ywu (USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS, USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// tryb nadawania/odbierania (USART_Mode_Rx, USART_Mode_Rx )
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	// konfiguracja
	USART_Init(USART3, &USART_InitStructure);

	// wlaczenie ukladu USART
	USART_Cmd(USART3, ENABLE);

}


// initUsartIRQ(USART3, USART3_IRQn, 0);
void initUsartIRQ(USART_TypeDef* USARTx, uint8_t USARTx_IRQn, uint8_t priority )
{
	//struktura do konfiguracji kontrolera NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	// wlaczenie przerwania zwi¹zanego z odebraniem danych (pozostale zrodla przerwan zdefiniowane sa w pliku stm32f4xx_usart.h)
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// konfiguracja kontrolera przerwan
	NVIC_Init(&NVIC_InitStructure);
	// wlaczenie przerwan od ukladu USART
	NVIC_EnableIRQ(USARTx_IRQn);
}

void USARTsendData(uint16_t data)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3, data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

uint8_t USARTreceiveData()
{
	// czekaj na odebranie danych
    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
   	return USART_ReceiveData(USART3);
}
/*
void USART3_IRQHandler(void)
{
	// sprawdzenie flagi zwiazanej z odebraniem danych przez USART
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
       {
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		operation [index] = USARTreceiveData();
		index++;
		if(index==3){
			index = 0;
			int result=0;
			int number1 = (int)operation[0]-(int)'0';
			int number2 = (int)operation[2]-(int)'0';
			if(operation[1]=='+'){
				result = number1+number2;
			}
			else if(operation[1]=='-'){
				result = number1-number2;
						}
			else if(operation[1]=='*'){
				result = number1*number2;
						}
			 USARTsendData('=');
			  char bufor[3];
			  memset(bufor, 0, 3);
			 itoa(result,bufor,10); //itoa konwertuje
			 for(int i=0; i < 3; ++i)
			 {
				 if(bufor[i] == 0) break;
				 USARTsendData(bufor[i]);
			 }
		}
		// odebrany bajt znajduje sie w rejestrze USART3->DR
	}
}
*/
