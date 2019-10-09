#include "main.h"
#include "DigPot_X9C.h"

/*
 * INC = PA8
 * UD  = PA9
 * CS  = PA10
 */


void DigPot_init(void)
{
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA -> CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10);
	GPIOA -> CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10);
	GPIOA -> BSRR |= (GPIO_BSRR_BS8 | GPIO_BSRR_BS9 | GPIO_BSRR_BS10); 			//CLK; DAT; FSYNK

  	RCC -> APB1ENR |= (RCC_APB1ENR_TIM4EN);	//enable clock source
	TIM4 -> PSC = 0;
	TIM4 -> ARR = 200;

	  for (int i = 0; i<100; i++)  //Put wiper on MAX Resistance
	  {
	  DigPot_write(1);
	  HAL_Delay(3);
	  }
}

void DigPot_write(uint8_t UP)
{
	TIM4 -> CR1 |= TIM_CR1_CEN; 						//enable timer

	GPIOA -> BSRR = (GPIO_BSRR_BS8 | GPIO_BSRR_BS9  | GPIO_BSRR_BS10);	//All outputs HIGH
	(UP) ? (GPIOA -> BSRR = GPIO_BSRR_BR9) : (GPIOA -> BSRR = GPIO_BSRR_BS9);
	while (TIM4 -> CNT < 10) {}			// wait 31 us

	GPIOA -> BSRR |= GPIO_BSRR_BR10;	//CS low
	TIM4 -> CNT = 0u;
	while (TIM4 -> CNT < 10) {}			// wait 31 us

	GPIOA -> BSRR = GPIO_BSRR_BR8;					//CLK down
	TIM4 -> CNT = 0u;
	while (TIM4 -> CNT < 10) {}			// wait 31 us
	GPIOA -> BSRR = GPIO_BSRR_BS8;					//CLK UP
	TIM4 -> CNT = 0u;
	while (TIM4 -> CNT < 10) {}			// wait 31 us
	GPIOA -> BSRR |= GPIO_BSRR_BS10;	//CS HIGH


	TIM4 -> CR1 &= ~TIM_CR1_CEN; 										//disable timer
	GPIOA -> BSRR |= (GPIO_BSRR_BS8 | GPIO_BSRR_BS9 | GPIO_BSRR_BS10);
}
