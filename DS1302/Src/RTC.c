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

int bcd2dec(uint16_t val)
{
	uint16_t mult = 0;

    mult = (val & 0x08 ? 10 : 0) +  (val & 0x04 ? 20 : 0) + (val & 0x02 ? 40 : 0);
    val = (val>>7) + ((val&0x40)>>5) + ((val&0x20)>>3) + ((val&0x10)>>1);
    val += mult;
	return val;
}

//GPIO ports configuration
	/*
	 *PA9 - CLK;
	 *PA10 - SDA
	 *PA11 - CE
	 */
void rtc_init(void)
{
	  GPIOA -> MODER |= (GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0 | GPIO_MODER_MODE11_0);					//PA9, PA10 and PA11 output
	  GPIOA -> MODER &= ~(GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1 |  GPIO_MODER_MODE11_1);					//PA9, PA10 and PA11 output
	  GPIOA -> OTYPER &= ~(GPIO_OTYPER_OT_9 | GPIO_OTYPER_OT_10 | GPIO_OTYPER_OT_11) ;						//PA9, PA10 and PA11 push-pull
	  GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11);		//PA9, PA10 and PA11 high speed
	  GPIOA -> PUPDR &= ~(GPIO_PUPDR_PUPDR9 | GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR11);						//PA9, PA10 and PA11 no pull-up/down

//	  rtc_write(0x80,  0x00);
}

void rtc_write(unsigned char  adress, unsigned char buffer)
{
	  int i=0;
	  adress = adress & 0xFE;	//D0 sempre igual a zero (write)

	GPIOA -> BSRR = (GPIO_BSRR_BR9 | GPIO_BSRR_BR10 | GPIO_BSRR_BS_11);	//CLK, SDA = 0 ; CE = 1

	TIM6 -> CR1 |= TIM_CR1_CEN; 						//enable timer
	TIM6 -> CNT = 0u;									//counter at zero

	  for (i=0; i<8; i++)								//WRITE TO ADRESS
	  {
		(adress & 1) ? (GPIOA -> BSRR = GPIO_BSRR_BS10) : (GPIOA -> BRR = GPIO_BRR_BR10);
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BSRR = GPIO_BSRR_BS9; 					//CLK up
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BRR = GPIO_BRR_BR9;					//CLK down
		adress >>= 1;
	  }

	  for (i=0; i<8; i++)								//DATA WRITE
	  {
		(buffer & 1) ? (GPIOA -> BSRR = GPIO_BSRR_BS10) : (GPIOA -> BRR = GPIO_BRR_BR10);
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BSRR = GPIO_BSRR_BS9; 					//CLK up
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;								//counter at zero
		GPIOA -> BRR = GPIO_BRR_BR9;					//CLK down
		buffer >>= 1;
	  }

	TIM6 -> CR1 &= ~TIM_CR1_CEN; 						//disable timer
	GPIOA -> BSRR = ( GPIO_BSRR_BR11 | GPIO_BSRR_BR9 | GPIO_BSRR_BR10);	///CLK, SDA, CE = 0
}


/*
 * Returns the required data
 */
int rtc_read(unsigned char  adress)

{
    int i=0;
    uint16_t buffer = 0x00;

	GPIOA -> BSRR = (GPIO_BSRR_BR9 | GPIO_BSRR_BR10 | GPIO_BSRR_BS_11);	//CLK, SDA = 0 ; CE = 1

	TIM6 -> CR1 |= TIM_CR1_CEN; 						//enable timer
	TIM6 -> CNT = 0u;									//counter at zero

	  for (i=0; i<8; i++)								//WRITE TO ADRESS
	  {
		(adress & 1) ? (GPIOA -> BSRR = GPIO_BSRR_BS10) : (GPIOA -> BRR = GPIO_BRR_BR10);
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;										//counter at zero
		GPIOA -> BSRR = GPIO_BSRR_BS9; 							//CLK up
		while (TIM6 -> CNT < 200) {}
		TIM6 -> CNT = 0u;										//counter at zero
		GPIOA -> BRR = GPIO_BRR_BR9;							//CLK down
		adress >>= 1;
	  }
	  GPIOA -> MODER &= ~(GPIO_MODER_MODE10);					//Set PA10 as input
	  GPIOA -> PUPDR &= ~(GPIO_PUPDR_PUPDR10);					//PA10 no pull-up/down
	  GPIOA -> OTYPER |= (GPIO_OTYPER_OT10);					//Open-drain


	  for (i=0; i<7; i++)										//READ
	  	  {
		  while (TIM6 -> CNT < 10) {}							//holding the data
		  buffer |=  (GPIOA -> IDR & GPIO_IDR_ID10) != 0u;		//read the value at PA10 (SDA)
		  while (TIM6 -> CNT < 200) {}							//wait for down cycle to end
		  TIM6 -> CNT = 0u;										//counter at zero
		  GPIOA -> BSRR = GPIO_BSRR_BS9; 						//CLK up
		  while (TIM6 -> CNT < 200) {}
		  TIM6 -> CNT = 0u;										//counter at zero
		  GPIOA -> BRR = GPIO_BRR_BR9;							//CLK down
		  buffer <<=1;											//Shift buffer one place
	  	  }
	TIM6 -> CR1 &= ~TIM_CR1_CEN; 								//disable timer

    GPIOA -> MODER |= (GPIO_MODER_MODE10_0);					//PA10 output
    GPIOA -> OTYPER &= ~(GPIO_OTYPER_OT_10) ;					//PA10 no push-pull
    GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10);				//PA10 high speed
    GPIOA -> PUPDR &= ~(GPIO_PUPDR_PUPDR10);					//PA10 no pull-up/down
	GPIOA -> BSRR = ( GPIO_BSRR_BR11 | GPIO_BSRR_BR9 | GPIO_BSRR_BR10);	///CLK, SDA, CE = 0

	return buffer;
}

//Read seconds
int rtc_sec(void)
{
	uint16_t seg = 0;
	uint16_t segd = 0;
	seg = rtc_read(0x81);
	segd = bcd2dec(seg);
	return segd;
}

int rtc_min(void)
{
	uint16_t min = 0;
	uint16_t mind = 0;
	min = rtc_read(0x83);
	mind = bcd2dec(min);
	return mind;
}

int rtc_hor(void)
{
	uint16_t hor = 0;
	uint16_t hord = 0;
	hor = rtc_read(0x85);
	hord = bcd2dec(hor & 0xf8);
	return hord;
}

int rtc_day(void)
{
	uint16_t dia = 0;
	uint16_t diad = 0;
	dia = rtc_read(0x87);
	diad = bcd2dec(dia);
	return diad;
}

int rtc_mon(void)
{
	uint16_t mes = 0;
	uint16_t mesd = 0;
	mes = rtc_read(0x89);
	mesd = bcd2dec(mes);
	return mesd;
}

int rtc_yer(void)
{
	uint16_t ano = 0;
	uint16_t anod = 0;
	ano = rtc_read(0x8d);
	anod = bcd2dec(ano);
	return anod;
}
