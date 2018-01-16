#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"

/*
 USTAW
 CMSIS->DEVICE->stm32f4xx.h
 #define HSE_VALUE    ((uint32_t)8000000)
 */

/*
 * typy memesa
typedef enum {
    TM_LIS302DL_LIS3DSH_Device_Error,
    TM_LIS302DL_LIS3DSH_Device_LIS302DL,
    TM_LIS302DL_LIS3DSH_Device_LIS3DSH
} TM_LIS302DL_LIS3DSH_Device_t;
 */
/*
 * struktura wyniku
typedef struct {
    int16_t X;
    int16_t Y;
    int16_t Z;
} TM_LIS302DL_LIS3DSH_t;
*/

void initOnBoardDiodes();

// w zaleznosci od przyspieszenia osi Z zapalaj diody
void zad3(int16_t z);

// wykrywanie potrzasania plytka
void zad4(const TM_LIS302DL_LIS3DSH_t* val, int graniczna);

void initializeAccele();
void convert_to_g(const TM_LIS302DL_LIS3DSH_t* val, TM_LIS302DL_LIS3DSH_t* result);

void convert_to_m_s2(const TM_LIS302DL_LIS3DSH_t* val, TM_LIS302DL_LIS3DSH_t* result);

// globalny dla funkcji
TM_LIS302DL_LIS3DSH_Device_t IMU_Type;

TM_LIS302DL_LIS3DSH_t Axes_Data;

TM_LIS302DL_LIS3DSH_t Axes_Data_g;

TM_LIS302DL_LIS3DSH_t Axes_Data_m_s2;


int main(void)
{
	initOnBoardDiodes();
	initializeAccele();
	TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);

	for(;;)
	{
		TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
		convert_to_g(&Axes_Data, &Axes_Data_g);
		convert_to_m_s2(&Axes_Data_g, &Axes_Data_m_s2);
	//	zad3(Axes_Data_m_s2.Z);
		zad4(&Axes_Data_m_s2, 5000);
	}
}

void initializeAccele()
{
	// sprawdz typ memesa
	IMU_Type = TM_LIS302DL_LIS3DSH_Detect();

	switch(IMU_Type)
	{
	case TM_LIS302DL_LIS3DSH_Device_LIS302DL:
	{
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
		break;
	}
	case TM_LIS302DL_LIS3DSH_Device_LIS3DSH:
	{
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
		break;
	}
	default:
	{break;}
	}
}

void convert_to_g(const TM_LIS302DL_LIS3DSH_t* val, TM_LIS302DL_LIS3DSH_t* result)
{
	// sprawdz typ memesa
	IMU_Type = TM_LIS302DL_LIS3DSH_Detect();
	switch(IMU_Type)
	{
	case TM_LIS302DL_LIS3DSH_Device_LIS302DL:
	{
		// -2.3 do 2.3
		// INT8_MAX
		// val/127 * 2.3(przedzial) = 0,01811023622047244094488188976378
		// * 1000 (do 3 miejsc po przecinku)
		// 18 <- od -2300 do +2300

		result->X = val->X * 18;
		result->Y = val->Y * 18;
		result->Z = val->Z * 18;
		break;
	}
	case TM_LIS302DL_LIS3DSH_Device_LIS3DSH:
	{
		// -2.0 do 2.0
		//INT16_MAX;
		// val/32767 * 2.0(przedzial)= 6,1037018951994384594256416516617 / 100000
		//  * 1000 (do 3 miejsc po przecinku)
		// 6104/100000 <- od -2000 do +2000

		result->X = val->X * 6104/ 100000;
		result->Y = val->Y * 6104/ 100000;
		result->Z = val->Z * 6104/ 100000;
		break;
	}
	default:
	{
		break;
	}
	}

}

void convert_to_m_s2(const TM_LIS302DL_LIS3DSH_t* val, TM_LIS302DL_LIS3DSH_t* result)
{
	// 1 g = 9.81 m/s^2
	// do 3 miejsc po przecinku ok. 19620 dla LIS3DSH, ok. 22563 dla LIS302DL
	result->X = val->X * 981 / 100;
	result->Y = val->Y * 981 / 100;
	result->Z = val->Z * 981 / 100;
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

void zad3(int16_t z)
{
	// (- niesk do -5> PD12
	if(z <= -5000)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	}
	// (-5 do 0> PD13
	else if(z <= 0)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15);
	}
	// (0 do 5> PD14
	else if(z <= 5000)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15);
	}
	else
	// (5 do niesk) PD15
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
	}

}

void zad4(const TM_LIS302DL_LIS3DSH_t* val, int graniczna)
{
	// int actualValue = val->X*val->X+val->Y*val->Y+val->Z + val->Z;
	if(val->X*val->X >= graniczna && val->Y*val->Y >= graniczna && val->Z + val->Z >= graniczna)
		GPIO_SetBits(GPIOD, GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	else
		GPIO_ResetBits(GPIOD, GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}
