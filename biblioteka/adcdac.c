#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void initADC(uint32_t RCC_APB2Periph_ADCx, uint16_t GPIO_Pin_x);
void configADC(ADC_TypeDef* ADCx, FunctionalState state);
void startADC(ADC_TypeDef* ADCx);


void initADC(uint32_t RCC_APB2Periph_ADCx, uint16_t GPIO_Pin_x) {

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); // GPIO clock init
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADCx, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	// niezale�ny tryb pracy przetwornik�w
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
	// zegar g��wny podzielony przez 2
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	// opcja istotna tylko dla trybu multi ADC
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	// czas przerwy pomi�dzy kolejnymi konwersjami
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
}


void configADC(ADC_TypeDef* ADCx, FunctionalState state) {
	ADC_InitTypeDef ADC_InitStructure;
	//KONFIGURACJA DANEGO PRZETWORNIKA ADC
	//ustawienie rozdzielczo�ci przetwornika na maksymaln� (12 bit�w)
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//wy��czenie trybu skanowania (odczytywa� b�dziemy jedno wej�cie ADC
	//w trybie skanowania automatycznie wykonywana jest konwersja na wielu //wej�ciach/kana�ach)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //jesli wiecej niz jeden kanal to wlacz
	//w��czenie ci�g�ego trybu pracy
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//wy��czenie zewn�trznego wyzwalania
	//konwersja mo�e by� wyzwalana timerem, stanem wej�cia itd. (szczeg�y w //dokumentacji)
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//warto�� binarna wyniku b�dzie podawana z wyr�wnaniem do prawej
	//funkcja do odczytu stanu przetwornika ADC zwraca warto�� 16-bitow�
	//dla przyk�adu, warto�� 0xFF wyr�wnana w prawo to 0x00FF, w lewo 0x0FF0
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//liczba konwersji r�wna 1, bo 1 kana�
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	// zapisz wype�nion� struktur� do rejestr�w przetwornika numer 1
	ADC_Init(ADCx, &ADC_InitStructure);

	uint8_t ADC_Channel_x;
			if(ADCx == ADC1)
				ADC_Channel_x = ADC_Channel_1;
			else if(ADCx ==  ADC2)
				ADC_Channel_x = ADC_Channel_2;

	ADC_RegularChannelConfig(ADCx, ADC_Channel_x, 1, ADC_SampleTime_84Cycles);

	ADC_Cmd(ADCx, state);
	if(state == ENABLE)
		ADC_SoftwareStartConv(ADCx);
}

//starts ADC when you set it to DISABLED mode
void startADC(ADC_TypeDef* ADCx) {
	ADC_Cmd(ADCx, ENABLE);
	ADC_SoftwareStartConv(ADCx);
}
