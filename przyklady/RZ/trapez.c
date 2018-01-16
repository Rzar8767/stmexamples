
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config(void);
void GPIO_Config(void);
void NVIC_Config(void);
void TIM_Config(void);
void DAC_Config(void);
void ADC_CommonInit_Config(void);
void ADC1_Config(void);


unsigned int DigitalToVoltValue(unsigned int unit, unsigned int value);
unsigned int VoltToDigitalValue(unsigned int vcc, unsigned int volt);
unsigned int GetVoltUnit(unsigned int vcc);

unsigned int ADC1_Result=0;

unsigned int ADC1V_Value=0;

void enableTimer(uint16_t presc, uint32_t period, uint32_t RCC_APB1Periph_TIMx, TIM_TypeDef* TIMx, FunctionalState state )
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE );
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; //84mhz 84000000
		TIM_TimeBaseStructure.TIM_Prescaler = presc;
		TIM_TimeBaseStructure.TIM_Period = period;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
		TIM_Cmd(TIMx, state);
}

void enableInterrupt(TIM_TypeDef* TIMx, uint8_t TIMx_IRQn, uint8_t priority )
{
	// ustawienie trybu pracy priorytetów przerwań
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = TIMx_IRQn;
	// priorytet główny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = priority;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kanał
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wypełnioną strukturę do rejestrów
	NVIC_Init(&NVIC_InitStructure);

	// wyczyszczenie przerwania od timera 3 (wystąpiło przy konfiguracji timera)
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	// zezwolenie na przerwania od przepełnienia dla timera 3
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

}

int tablica[30];

void fillTable(int max)
{
	for(int i=0; i < 10; ++i)
	{
		tablica[i] = max/10 * i;
	}
	for(int i=10; i < 20; ++i)
		{
			tablica[i] = max;
		}
	for(int i=20; i < 30; ++i)
	{
		tablica[i] = tablica[29-i];
	}

}

int index=0;
void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
         		++index;
         		if(index==30) index =0;

                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

         	}

}

int main(void)
{
	/*
	 * WCZYTANIE KONFIGURACJI MODULOW
	 */
	RCC_Config();
	GPIO_Config();
	ADC_CommonInit_Config();
	ADC1_Config();
	DAC_Config();
	/*
	 * WCZYTANIE KONFIGURACJI MODULOW - KONIEC
	 */

	/*
	 * PROGRAM GLOWNY
	 */

	unsigned int vcc=295*100000; //2,95V
	unsigned int volt=100*100000; //1,00V
	unsigned int unit=GetVoltUnit(vcc);
	unsigned int digitalValue=0;

	DAC_SetChannel1Data(DAC_Align_12b_R, digitalValue);
	ADC_SoftwareStartConv(ADC1);

	enableTimer(8399, 999, RCC_APB1Periph_TIM4, TIM4, ENABLE);
	enableInterrupt(TIM4,TIM4_IRQn, 0);


	fillTable(2700);


	for(;;) {
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		DAC_SetChannel1Data(DAC_Align_12b_R, tablica[index]);
		ADC1_Result = ADC_GetConversionValue(ADC1);
		ADC1V_Value=DigitalToVoltValue(unit,ADC1_Result);
	}

	/*
	 * PROGRAM GLOWNY - KONIEC
	 */
}
void RCC_Config(void) {
	//Doprowadzenie zasilania i taktowania
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); // zegar dla portu GPIO z którego wykorzystany zostanie pin jako wejście ADC (PA1)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // zegar dla modułu ADC1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); // zegar dla modułu DAC

}

void GPIO_Config(void) {

	//Wyjscie analogowe DAC PA4 i wejscie analogowe ADC1 PA1
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
void TIM_Config(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 8400-1;
		TIM_TimeBaseStructure.TIM_Prescaler = 10000-1;
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		GPIO_InitTypeDef GPIO_InitStructure;
			TIM_OCInitTypeDef TIM_OCInitStructure;
			/* PWM1 Mode configuration: */
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_Pulse = 0;
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			//
			//tim3 RGB
			//
			TIM_OC1Init(TIM3, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

			GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			TIM_OC2Init(TIM3, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

			GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			TIM_ARRPreloadConfig(TIM3,ENABLE);
			TIM_Cmd(TIM3, ENABLE);
}

void NVIC_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
	// numer przerwania
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	// priorytet główny
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	// subpriorytet
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kanał
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// zapisz wypełnioną strukturę do rejestrów
	NVIC_Init(&NVIC_InitStructure);

	// wyczyszczenie przerwania od timera 3 (wystąpiło przy konfiguracji timera)
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	// zezwolenie na przerwania od przepełnienia dla timera 3
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

void DAC_Config(void) {
	DAC_InitTypeDef DAC_InitStructure;
	//wyłączenie zewnętrznego wyzwalania
	//konwersja może być wyzwalana timerem, stanem wejścia itd. (szczegóły w dokumentacji)
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	//nast. 2 linie - wyłączamy generator predefiniowanych przebiegów //wyjściowych (wartości zadajemy sami, za pomocą odpowiedniej funkcji)
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	//włączamy buforowanie sygnału wyjściowego
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);



	DAC_Cmd(DAC_Channel_1, ENABLE);

}

void ADC_CommonInit_Config(void) {

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	//KONFIGURACJA WSZYSTKICH PRZETWORNIKOW ADC (COMMON)
	// niezależny tryb pracy przetworników
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;

	// zegar główny podzielony przez 2
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	// opcja istotna tylko dla trybu multi ADC
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	// czas przerwy pomiędzy kolejnymi konwersjami
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
}


void ADC1_Config(void) {
	ADC_InitTypeDef ADC_InitStructure;
	//KONFIGURACJA DANEGO PRZETWORNIKA ADC
	//ustawienie rozdzielczości przetwornika na maksymalną (12 bitów)
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//wyłączenie trybu skanowania (odczytywać będziemy jedno wejście ADC
	//w trybie skanowania automatycznie wykonywana jest konwersja na wielu //wejściach/kanałach)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //jesli wiecej niz jeden kanal to wlacz
	//włączenie ciągłego trybu pracy
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//wyłączenie zewnętrznego wyzwalania
	//konwersja może być wyzwalana timerem, stanem wejścia itd. (szczegóły w //dokumentacji)
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//wartość binarna wyniku będzie podawana z wyrównaniem do prawej
	//funkcja do odczytu stanu przetwornika ADC zwraca wartość 16-bitową
	//dla przykładu, wartość 0xFF wyrównana w prawo to 0x00FF, w lewo 0x0FF0
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//liczba konwersji równa 1, bo 1 kanał
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	// zapisz wypełnioną strukturę do rejestrów przetwornika numer 1
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);

	ADC_Cmd(ADC1, ENABLE);

	/*
	//Reset rejestrow kalibracyjnych ADC1
	ADC_ResetCalibraton(ADC1);

	//Odczekanie na wykonanie resetu
	while(ADC_GetResetCalibrationStatus(ADC1));

	//Kalibracja ADC1
	ADC_StartCalibraton(ADC1);

	//Odczekanie na zakonczenie kalibracji
	while (ADC_GetCalibratonStatus(ADC1));
	*/
}

unsigned int DigitalToVoltValue(unsigned int unit, unsigned int value) {
	//for 2,95V 1unit is 0,7204mV, we take 7204, so thats why we divide by 10000 (4 zeros)
	unsigned int result = value*unit/10000;
	return result; //mV

}

unsigned int VoltToDigitalValue(unsigned int vcc, unsigned int volt) {
	//volt & vcc must be multiplied by 10000000 (4 zeros accuracy + 3 zeros from mV)
	unsigned int result = (volt*4095)/vcc;
		return result;
}

unsigned int GetVoltUnit(unsigned int vcc) {
	//vcc must be multiplied by 10000000  (4 zeros accuracy + 3 zeros from mV)
	unsigned int result = vcc/4095;
	return result; //mV
}

