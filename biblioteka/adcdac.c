#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void initADC(uint32_t RCC_APB2Periph_ADCx, uint16_t GPIO_Pin_x);
void configADC(ADC_TypeDef* ADCx, FunctionalState state);
void startADC(ADC_TypeDef* ADCx);
void initDAC(uint16_t GPIO_Pin_x);

unsigned int voltsFromDigital(unsigned int digitalValue);
unsigned int DigitalToVoltValue(unsigned int unit, unsigned int value);
unsigned int GetVoltUnit(unsigned int vcc);

//////////////////////////////////////          ADC          /////////////////////////////////////////////
// adc1: initADC(RCC_APB2Periph_ADC1, GPIO_Pin_1);
// adc2: initADC(RCC_APB2Periph_ADC2, GPIO_Pin_2);
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

uint16_t getADCvoltage(ADC_TypeDef* ADCx){
	return voltsFromDigital(ADC_GetConversionValue(ADCx));
}
/*///////////////////////////                  ADC Usage example
int ADC1_Result = 0;
int ADC1V_Value = 0;

int main(void)
{
	initADC(RCC_APB2Periph_ADC1, GPIO_Pin_1);
	configADC(ADC1,ENABLE);

	for(;;) {
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		ADC1_Result = ADC_GetConversionValue(ADC1);
		ADC1V_Value=ADC1_Result*7204/10000; //mV
	}
}*/

//////////////////////////////////////          DAC          /////////////////////////////////////////////

//initDAC(GPIO_Pin_5);
void initDAC(uint16_t GPIO_Pin_x) {
	//Doprowadzenie zasilania i taktowania
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE); // zegar dla portu GPIO z którego wykorzystany zostanie pin jako wejœcie ADC (PA1)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); // zegar dla modu³u DAC

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitTypeDef DAC_InitStructure;
	//wy³¹czenie zewnêtrznego wyzwalania
	//konwersja mo¿e byæ wyzwalana timerem, stanem wejœcia itd. (szczegó³y w dokumentacji)
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	//nast. 2 linie - wy³¹czamy generator predefiniowanych przebiegów //wyjœciowych (wartoœci zadajemy sami, za pomoc¹ odpowiedniej funkcji)
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	//w³¹czamy buforowanie sygna³u wyjœciowego
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;

	uint32_t DAC_Channel_x;
	if(GPIO_Pin_x == GPIO_Pin_4)
		DAC_Channel_x = DAC_Channel_1;
	else if(GPIO_Pin_x == GPIO_Pin_5)
			DAC_Channel_x = DAC_Channel_2;

	DAC_Init(DAC_Channel_x, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_x, ENABLE);
}

void setDACvalue(uint16_t GPIO_Pin_x,uint16_t value){
	if(GPIO_Pin_x == GPIO_Pin_4)
		DAC_SetChannel1Data(DAC_Align_12b_R, value);
	else if(GPIO_Pin_x == GPIO_Pin_5)
		DAC_SetChannel2Data(DAC_Align_12b_R, value);
}

//////////////////////////////////////          Helpers          /////////////////////////////////////////

unsigned int voltsFromDigital(unsigned int digitalValue){
	return 	DigitalToVoltValue(7204,digitalValue);
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

//////////////////////////////////////          Helpers DOUBLE          /////////////////////////////////////////

// digital to analog
// result returned in second parameter via pointer
uint16_t convToValue(double* volts)
{
	return *volts* 1000.0 * 2.95 / 4095.0;
}
// value to volts
// value/4095 * 2.95
// result returned in second parameter via pointer
void convToVolts(uint16_t value, double* volts)
{
	*volts = (double) value / 4095.0 * 2.95;
}
