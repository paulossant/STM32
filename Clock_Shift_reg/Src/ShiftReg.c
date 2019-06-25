#include "main.h"
#include "ShiftReg.h"

extern numero[];
void shiftReg_init(void)
{
	  GPIOA -> MODER |= (GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0);
	  GPIOA -> MODER &= ~(GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1);
	  GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6);		//PA9, PA10 and PA11 high speed
}

void shiftReg_write(uint16_t value)
{
	int i=0;
	GPIOA -> BSRR = (GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BS_6);	//CLK, DAT; ~OE
	value = numero[value];
	TIM6 -> CR1 |= TIM_CR1_CEN; 						//enable timer
	TIM6 -> CNT = 0u;									//counter at zero

	  for (i=0; i<8; i++)								//WRITE TO ADRESS
	  {
		(value & 1U) ? (GPIOA -> BSRR = GPIO_BSRR_BS5) : (GPIOA -> BSRR = GPIO_BSRR_BR5);
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BSRR = GPIO_BSRR_BS4; 					//CLK up
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BSRR = GPIO_BSRR_BR4;					//CLK down
		value >>= 1;
	  }
	GPIOA -> BSRR = GPIO_BSRR_BR_6;	//CLK, DAT; ~OE
}
