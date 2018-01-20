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
	// niezale¿ny tryb pracy przetworników
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;
	// zegar g³ówny podzielony przez 2
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	// opcja istotna tylko dla trybu multi ADC
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	// czas przerwy pomiêdzy kolejnymi konwersjami
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
}


void configADC(ADC_TypeDef* ADCx, FunctionalState state) {
	ADC_InitTypeDef ADC_InitStructure;
	//KONFIGURACJA DANEGO PRZETWORNIKA ADC
	//ustawienie rozdzielczoœci przetwornika na maksymaln¹ (12 bitów)
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//wy³¹czenie trybu skanowania (odczytywaæ bêdziemy jedno wejœcie ADC
	//w trybie skanowania automatycznie wykonywana jest konwersja na wielu //wejœciach/kana³ach)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //jesli wiecej niz jeden kanal to wlacz
	//w³¹czenie ci¹g³ego trybu pracy
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//wy³¹czenie zewnêtrznego wyzwalania
	//konwersja mo¿e byæ wyzwalana timerem, stanem wejœcia itd. (szczegó³y w //dokumentacji)
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//wartoœæ binarna wyniku bêdzie podawana z wyrównaniem do prawej
	//funkcja do odczytu stanu przetwornika ADC zwraca wartoœæ 16-bitow¹
	//dla przyk³adu, wartoœæ 0xFF wyrównana w prawo to 0x00FF, w lewo 0x0FF0
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//liczba konwersji równa 1, bo 1 kana³
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	// zapisz wype³nion¹ strukturê do rejestrów przetwornika numer 1
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
