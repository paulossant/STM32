#include "main.h"
#include "touch.h"
#include "9481.h"

#define YP  GPIO_PIN_0
#define YM  GPIO_PIN_10
#define XP  GPIO_PIN_9
#define XM  GPIO_PIN_1

#define YP_HIGH()     GPIOB->BSRR = YP
#define YP_LOW()      GPIOB->BSRR = YP 	<<16

#define YM_HIGH()     GPIOB->BSRR = YM
#define YM_LOW()      GPIOB->BSRR = YM 	<<16

#define XP_HIGH()	  GPIOB->BSRR = XP
#define XP_LOW()      GPIOB->BSRR = XP 	<<16

#define XM_HIGH()	  GPIOB->BSRR = XM
#define XM_LOW()      GPIOB->BSRR = XM 	<<16


ADC_HandleTypeDef hadc2;
/*
 * YP - WR 0
 * YM - D7 10
 * XP - D6 9
 * XM - RS 1
*/

//const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
//const int XP=6,XM=A2,YP=A1,YM=7; //320x480 ID=0x9090


void touch_read(volatile uint16_t ADval[]){
	/*	GPIO to data bus pin connections
	 * D0		->	B3
	 * D1		-> 	B4
	 * D2		->	B5
	 * D3		->	B6
	 * D4		->	B7
	 * D5		->	B8
	 * D6		->	B9
	 * D7		->	B10
	 * RST		->	B2
	 * CS		->	B11
	 * RS/CD	->	B1
	 * WR		->	B0
	 * RD		->	VCC
	 *
	 * YP - WR 0
	 * YM - D7 10
	 * XP - D6 9
	 * XM - RS 1
	 */

	//READ X
	//XP=1, XM=0,YM=Z,YP=ADC(D6-PB6)
	XP_HIGH();
	XM_LOW();
	YM_LOW();
	YP_LOW();

	GPIOB -> CRH &= ~GPIO_CRH_MODE10;
	GPIOB -> CRH |= GPIO_CRH_CNF10_0;
	GPIOB -> CRL &= ~ GPIO_CRL_MODE0;
	GPIOB -> CRL |= GPIO_CRL_CNF0 ;

	HAL_ADC_Start(&hadc2);
	    if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) == HAL_OK)
	    {
	    ADval[0] = HAL_ADC_GetValue(&hadc2);	//THIS ONE
	    }

	GPIOB -> CRH |= GPIO_CRH_MODE10;
	GPIOB -> CRH &= ~GPIO_CRH_CNF10;
	GPIOB -> CRL |=  GPIO_CRL_MODE0;
	GPIOB -> CRL &= ~GPIO_CRL_CNF0 ;

	//ReadY
	//YP=1, YM=0,XM=Z,XM=ADC
	YP_LOW();
	YM_HIGH();
	XP_LOW();
	XM_LOW();

	GPIOB -> CRH &= ~GPIO_CRH_MODE9 ;
	GPIOB -> CRH |= GPIO_CRH_CNF9_0 ;
	GPIOB -> CRL &= ~GPIO_CRL_MODE1;
	GPIOB -> CRL |= GPIO_CRL_CNF1;

	HAL_Delay(0);
	if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) == HAL_OK)
	{
	ADval[1] = HAL_ADC_GetValue(&hadc2);
	}
   HAL_ADC_Stop(&hadc2);

	GPIOB -> CRH |= GPIO_CRH_MODE9;
	GPIOB -> CRH &= ~GPIO_CRH_CNF9;
    GPIOB -> CRL |= GPIO_CRL_MODE1;
	GPIOB -> CRL &= ~GPIO_CRL_CNF1;


	//Read Pressure
	/*
	 * XP -> GND
	 * YM -> VCC
	 * READ YP and XM
	 */
	GPIOB -> CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
	GPIOB -> CRL |= (GPIO_CRL_CNF0 | GPIO_CRL_CNF1);
	XP_LOW();
	YM_HIGH();

	HAL_ADC_Start(&hadc2);
	    if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) == HAL_OK)
	    {
	    ADval[2] = HAL_ADC_GetValue(&hadc2);
	    }
	HAL_ADC_Stop(&hadc2);
    __NOP();

	GPIOB -> CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
	GPIOB -> CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);

	GPIOB -> BSRR |= (GPIO_BSRR_BS1 | GPIO_BSRR_BS0);
}
