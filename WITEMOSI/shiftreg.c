#include "main.h"
#include "shiftreg.h"


uint8_t numero[] = {0xee,0x28,0xcd,0x6d,0x2b,0x67,0xe7,0x2c,0xef,0x2f, 0xaf, 0xe3, 0xc6, 0xe9, 0xc7, 0x87};
//uint8_t numero[] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x5d,0x7d,0x6e,0x63,0x3e,0x6b,0x69};
void shiftReg_init(void)
{
	  GPIOA -> MODER |= (GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0);
	  GPIOA -> MODER &= ~(GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1);
	  GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6  | GPIO_OSPEEDER_OSPEEDR7); //high speed
	  GPIOA -> BSRR = (GPIO_BSRR_BR4 | GPIO_BSRR_BR5 | GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7);	//CLK, DAT; ~OE, ~clr low
	  HAL_Delay(1);
	  GPIOA -> BSRR = GPIO_BSRR_BS_7;
	  HAL_Delay(1);
}

void shiftReg_write(uint32_t value)
{
	uint32_t valor = 0;
	GPIOA -> BSRR = (GPIO_BSRR_BR4 | GPIO_BSRR_BS_6 | GPIO_BSRR_BR_7);	//CLK, DAT; ~OE
	GPIOA -> BSRR = GPIO_BSRR_BS_7;


	TIM6 -> CR1 |= TIM_CR1_CEN; 						//enable timer

	for (int j=0; j<4; j++)
	{
//	  valor = 0xff & ( value>>(8*j));
	  valor = 0xff & ( value>>(8*j));
	  valor = numero[valor];
	  for (int i=0; i<8; i++)
	  {
		(valor & 0x01) ? (GPIOA -> BSRR = GPIO_BSRR_BS5) : (GPIOA -> BSRR = GPIO_BSRR_BR5);
		valor >>= 1;
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BSRR = GPIO_BSRR_BS4; 					//CLK up
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BSRR = GPIO_BSRR_BR4;					//CLK down
	  }
	}
	GPIOA -> BSRR = GPIO_BSRR_BR5;
	valor >>= 1;
	while (TIM6 -> CNT < 200) {}
	TIM6 -> CNT = 0u;								//counter at zero
	GPIOA -> BSRR = GPIO_BSRR_BS4; 					//CLK up
	while (TIM6 -> CNT < 200) {}
	TIM6 -> CNT = 0u;								//counter at zero
	GPIOA -> BSRR = GPIO_BSRR_BR4;					//CLK down
	TIM6 -> CR1 &= ~TIM_CR1_CEN; 						//disable timer
	GPIOA -> BSRR = (GPIO_BSRR_BR_6 |  GPIO_BSRR_BR5);	//~OE
}
