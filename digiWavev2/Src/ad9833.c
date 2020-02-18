#include "main.h"
#include "ad9833.h"

/*
 * PA4 -> CLK
 * PA5 -> DAT
 * PA6 -> FSYNK
 */

/***********************************************************************
						Control Register
------------------------------------------------------------------------
D15,D14(MSB)	10 = FREQ1 write, 01 = FREQ0 write,
 				11 = PHASE write, 00 = control write
D13	If D15,D14 = 00, 0 = individual LSB and MSB FREQ write,
 					 1 = both LSB and MSB FREQ writes consecutively
	If D15,D14 = 11, 0 = PHASE0 write, 1 = PHASE1 write
D12	0 = writing LSB independently
 	1 = writing MSB independently
D11	0 = output FREQ0,
	1 = output FREQ1
D10	0 = output PHASE0
	1 = output PHASE1
D9	Reserved. Must be 0.
D8	0 = RESET disabled
	1 = RESET enabled
D7	0 = internal clock is enabled
	1 = internal clock is disabled
D6	0 = onboard DAC is active for sine and triangle wave output,
 	1 = put DAC to sleep for square wave output
D5	0 = output depends on D1
	1 = output is a square wave
D4	Reserved. Must be 0.
D3	0 = square wave of half frequency output
	1 = square wave output
D2	Reserved. Must be 0.
D1	If D5 = 1, D1 = 0.
	Otherwise 0 = sine output, 1 = triangle output
D0	Reserved. Must be 0.
***********************************************************************/

uint32_t wvf_bits = 0;
uint32_t FMSB = 0;
uint32_t FLSB = 0;

void ad9833_init(void)
{
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA -> CRL |= (GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6);
	GPIOA -> CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6);
	GPIOA -> BSRR |= (GPIO_BSRR_BS4 | GPIO_BSRR_BS5 | GPIO_BSRR_BS6); 			//CLK; DAT; FSYNK

  	RCC -> APB1ENR |= (RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM3EN);	//enable clock source
	TIM4 -> PSC = 0;
	TIM4 -> ARR = 200;

	TIM3 -> PSC = 0;
	TIM3 -> ARR = 64;
	/*
	 * TIM3
	 * 64 ARR
	 * 320 kHz -> 1 us/div
	 */


  ad9833_write(0x2100);
  ad9833_write(0x50c7);
  ad9833_write(0x4000);
  ad9833_write(0xc000);
  ad9833_write(0x2000);
}

void ad9833_write(uint16_t value)
{
	int i;
	TIM4 -> CR1 |= TIM_CR1_CEN; 						//enable timer
	GPIOA -> BSRR = (GPIO_BSRR_BS4 | GPIO_BSRR_BR6);	//FSYNK wait mininum 5ns
	while (TIM4 -> CNT < 25) {}

	for (i=0; i<16; i++)								//WRITE TO ADRESS
	  {
		(value & 0x8000) ? (GPIOA -> BSRR = GPIO_BSRR_BS5) : (GPIOA -> BSRR = GPIO_BSRR_BR5);
		while (TIM4 -> CNT < 100) {}
		TIM4 -> CNT = 0u;
		GPIOA -> BSRR = GPIO_BSRR_BR4;					//CLK down
		value <<= 1;
		while (TIM4 -> CNT < 100) {}
		TIM4 -> CNT = 0u;
		GPIOA -> BSRR = GPIO_BSRR_BS4;					//CLK UP
		if (i == 15) {GPIOA -> BSRR = GPIO_BSRR_BS6;}
	  }
	while (TIM4 -> CNT < 150) {}
	TIM4 -> CR1 &= ~TIM_CR1_CEN; 										//disable timer
	GPIOA -> BSRR = (GPIO_BSRR_BS4 | GPIO_BSRR_BS5 | GPIO_BSRR_BS6);
}

void ad9833_freq(uint32_t value)
{
	float k = 10.73741824;
	int LSB, MSB;

	value *= k;
    LSB = (value & 0x00003fff) | 0x4000;
    MSB = (value & 0x0fffc000) >> 14U | 0x4000;
	ad9833_write(LSB);
	ad9833_write(MSB);

	FMSB = MSB;
	FLSB = LSB;
}

void ad9833_waveform(uint8_t wvf)
{
	 if (wvf == 0)								//Sin
	{
	ad9833_write(0x1000);
	wvf_bits = 0b00;
	}
	else if (wvf == 1)							//Tri
	{
	ad9833_write(0x1002);
	wvf_bits = 0b10;
	}
	else if (wvf == 2)							//Square
	{
	ad9833_write(0x1028);
	wvf_bits = 0x28;
	}
}

void ad9833_phase(uint32_t value)
{
	// Value in degrees
	float mult = 11.377777778;
	value *= mult;

	ad9833_write(0x2100);
    ad9833_write(0xc000 | value);
    ad9833_write(0x2000);

}

void add9833_DACON(uint8_t state)
{
	if (state == 0)								//OFF
	{
		ad9833_write(0b0001000001001000);

	}
	else if (state == 1)							//ON
	{
		ad9833_write(0b0001000000001000 | wvf_bits);
	}
	else{}
}

void ad9833_reset(uint8_t res)
{
	if (res == 1)
	{
		  ad9833_write(0x2100);
	}
	else if (res == 0)
	{
		  ad9833_write(0x2000);
	}
	else{}
}

void ad9833_sweep(uint32_t fstart, uint32_t fstop, uint32_t fstep, uint32_t fdelay)
{
	/*
	 *
	 * fstart and fstop in Hz
	 * fstep in Hz
	 * fdelay in microseconds
	 */

	TIM3 -> CR1 |= TIM_CR1_CEN; 						//enable timer
	while (fstart < fstop){
		TIM4 -> CNT = 0u;
		ad9833_freq(fstart);
		for (int i = 0; i < fdelay; i++)
		{
		while (TIM3 -> CNT < 62) {}
		}
		fstart+=fstep;
	}
	TIM3 -> CR1 &= ~TIM_CR1_CEN;
}
