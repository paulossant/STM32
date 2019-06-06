#include "RTC.h"
#include "main.h"

#define w_sec			0x80
#define w_min			0x82
#define w_hr			0x84
#define w_date			0x86
#define w_month			0x88
#define w_day			0x8a
#define w_year			0x8c
#define w_control		0x8e
#define w_charger		0x90
#define w_clkburst		0xbe

void rtc_init(void)
{
//GPIO ports configuration
	/*PA9 - CLK
	 * PA10 - SDA
	 * PA11 - CE
	 */
	  GPIOA -> MODER |= (GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0 | GPIO_MODER_MODE11_0);					//PA9, PA10 and PA11 output
	  GPIOA -> MODER &= ~(GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1 |  GPIO_MODER_MODE11_1);					//PA9, PA10 and PA11 output
	  GPIOA -> OTYPER &= ~(GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10 | GPIO_OTYPER_OT_11) ;						//PA9, PA10 and PA11 push-pull
	  GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11);		//PA9, PA10 and PA11 high speed
	  GPIOA -> PUPDR &= ~(GPIO_PUPDR_PUPDR9 | GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR11);						//PA9, PA10 and PA11 no pull-up/down
}

void rtc_w(unsigned char  adress)
{
	  int i=0;
	  adress = adress & 0xFE;	//D0 sempre igual a zero (write)

	GPIOA -> BSRR = (GPIO_BSRR_BR9 | GPIO_BSRR_BR10 | GPIO_BSRR_BS_11);	//CLK, SDA = 0 ; CE = 1

	TIM6 -> CR1 |= TIM_CR1_CEN; 				//enable timer
	TIM6 -> CNT = 0u;							// counter at zero
	  for (i=0; i<8; i++)
	  {
		(adress & 1) ? (GPIOA -> BSRR = GPIO_BSRR_BS10) : (GPIOA -> BRR = GPIO_BRR_BR10);
		while (TIM6 -> CNT < 200) {}
		GPIOA -> BSRR = GPIO_BSRR_BS9; 			//CLK up
		TIM6 -> CNT = 0u;						// counter at zero
		while (TIM6 -> CNT < 200) {}
		GPIOA -> BRR = GPIO_BRR_BR9;			//CLK down
		TIM6 -> CNT = 0u;						// counter at zero
		adress >>= 1;
	  }
	TIM6 -> CR1 &= ~TIM_CR1_CEN; 				//disable timer
	GPIOA -> BSRR = ( GPIO_BSRR_BR11 | GPIO_BSRR_BR9 | GPIO_BSRR_BR10);	///CLK, SDA, CE = 0
}


