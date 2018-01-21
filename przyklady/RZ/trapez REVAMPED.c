#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

//requires timer.c
//requires adcdac.c

void trapez();
void fillTableTrapez();
void fillTableRising();

// sampling frequency, 1 every 100ms
uint16_t table[30];
int ADC1_Result;
double ADC1_voltage =0;



int main(void)
{

	trapez();
	for(;;)
	{

	}
}

int index=0;
void TIM4_IRQHandler(void)
{
         	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
         	{
         		++index;
         		if(index==30) index =0;


        		//pobierz wartosc napiecia i przekonwertuj
        		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
        		setDACvalue(GPIO_Pin_4, table[index]);
        		ADC1_Result = ADC_GetConversionValue(ADC1);
        		// ADC1_voltage is what we wanna check
        		convToVolts_d(ADC1_Result, &ADC1_voltage);


                TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

         	}

}



// function named after Krystian, our great math tutor
void trapez()
{
	// here put the function that fills the table, every 100ms the value shown by DAC changes
	//fillTableTrapez();
	fillTableRising();

	// use ADC1
	initADC(RCC_APB2Periph_ADC1, GPIO_Pin_1);
	configADC(ADC1, DISABLE);

	// also use DAC1
	initDAC(GPIO_Pin_4);


	// timer co 100 milisekund
	initTimerTime(100, TIM4, DISABLE);
	initTimerIRQ(TIM4, TIM4_IRQn, 0);

	// start the conversion
	startADC(ADC1);



	// FIRST CONVERSION
	// set DAC value to first element of the table
	//pobierz wartosc napiecia i przekonwertuj
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	setDACvalue(GPIO_Pin_4, table[index]);
	ADC1_Result = ADC_GetConversionValue(ADC1);
	// ADC1_voltage is what we wanna check
	convToVolts_d(ADC1_Result, &ADC1_voltage);


	// start the timer
	setTimerState(TIM4, ENABLE);
	while(1)
	{
		// conversion inside of timer IRQ to make sure the sampling frequency is correct
	}
}


// w 0 ma miec 0V
// w 1.5 ma miec 1.5V
// w 3 ma miec 1.5V
void fillTableTrapez()
{
	double peak = 2.95;
	double volt;
	// increasing from 0 to peak
	for(int i=0; i < 10; ++i)
	{
		volt = peak/10 * i;
		table[i] = convToValue_d(&volt);
	}
	// stable peak
	for(int i=10; i < 20; ++i)
		{
		volt = peak;
		table[i] = convToValue_d(&volt);
		}
	// decreasing from peak to 0, mirroring
	for(int i=20; i < 30; ++i)
	{
		table[i] = table[29-i];
	}
}

void fillTableRising()
{
	double peak = 1.5;
	double volt;
	//increasing from 0 to peak
	for(int i =0; i < 14; ++i)
	{
		volt = peak/14 *i;
		table[i] = convToValue_d(&volt);
	}
	// sharp drop
	table[15] = 0;
	for(int i =15; i < 30; ++i)
	{
		table[i] = table[i-15];
	}
}

