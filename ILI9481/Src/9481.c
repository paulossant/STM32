/*
 * ili9481.h
 *
 * Created: 04 Jul 2019
 *
 * Grafic LCD Function (Chip=ILI9481)
 *
 * P. S. S. dos Santos
 *
 */

#include "main.h"
#include "9481.h"

#define TFTWIDTH   240
#define TFTHEIGHT  320

#define TFTLCD_DELAY 0xFF

/*	GPIO to data bus pin connections
 * D0		->	B0
 * D1		-> 	B1
 * D2		->	B12
 * D3		->	B3
 * D4		->	B4
 * D5		->	B5
 * D6		->	B6
 * D7		->	B7
 * RST		->	B8
 * CS		->	B9
 * RS/CD	->	B10
 * WR		->	B11
 * RD		->	B13
 */

#define LCD_CS_PIN  GPIO_PIN_9	// PB0 -> A3 // Chip Select goes to Analog 3
#define LCD_CD_PIN  GPIO_PIN_10	// PA4 -> A2 // Command/Data goes to Analog 2
#define LCD_WR_PIN  GPIO_PIN_11	// PA1 -> A1 // LCD Write goes to Analog 1
#define LCD_RD_PIN  GPIO_PIN_13	// PA0 -> A0 // LCD Read goes to Analog 0
#define LCD_RST_PIN GPIO_PIN_8	// PC1 -> RESET

#define LCD_CS_HIGH()     GPIOB -> BSRR = LCD_CS_PIN
#define LCD_CS_LOW()      GPIOB-> BSRR = LCD_CS_PIN  <<16

#define LCD_RD_HIGH()     GPIOB->BSRR = LCD_RD_PIN
#define LCD_RD_LOW()      GPIOB->BSRR = LCD_RD_PIN<<16

#define LCD_WR_HIGH()	  GPIOB->BSRR = LCD_WR_PIN
#define LCD_WR_LOW()      GPIOB->BSRR = LCD_WR_PIN <<16

#define LCD_CD_HIGH()     GPIOB->BSRR = LCD_CD_PIN
#define LCD_CD_LOW()      GPIOB->BSRR = LCD_CD_PIN <<16

#define LCD_RST_HIGH()    GPIOB->BSRR = LCD_RST_PIN
#define LCD_RST_LOW()     GPIOB->BSRR = LCD_RST_PIN <<16

#define LCD_WR_STROBE() { LCD_WR_LOW(); LCD_WR_HIGH();}	//If always stable remove delay
#define LCD_RD_STROBE() { LCD_RD_LOW(); LCD_RD_HIGH();}	//If always stable remove delay

void delay(unsigned int t)
{
	for (; t > 0; t-- )
	{
	__asm("nop");
	}
}


/**
 * \brief GPIO Initialization
 *
 * \param
 *
 * \return void
 */
void LCD_GPIO_Init(void)
{
	/*	GPIO to data bus pin connections
	 * D0		->	B0
	 * D1		-> 	B1
	 * D2		->	B12
	 * D3		->	B3
	 * D4		->	B4
	 * D5		->	B5
	 * D6		->	B6
	 * D7		->	B7
	 * RST		->	B8
	 * CS		->	B9
	 * RS/CD	->	B10
	 * WR		->	B11
	 * RD		->	B13
	 */
	RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;	/* Enable clock for GPIOB */
	GPIOB -> CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7 );		/*Output Max speed 50 MHz */
	GPIOB -> CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);		/* Gen Purpose out push-pull */
	GPIOB -> CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_MODE12 | GPIO_CRH_MODE13);
	GPIOB -> CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12 | GPIO_CRH_CNF13 );

	GPIOB -> BSRR |= (  GPIO_BSRR_BS8 | GPIO_BSRR_BS9 | GPIO_BSRR_BS10 | GPIO_BSRR_BS11 | GPIO_BSRR_BS13);
}

/**
 * \brief Writes 8-Bit data
 *
 * \param data	8-Bit Data
 *
 * \return void
 */
void LCD_Write8(uint8_t data)
{
	/*	GPIO to data bus pin connections
	 * D0		->	B0
	 * D1		-> 	B1
	 * D2		->	B12
	 * D3		->	B3
	 * D4		->	B4
	 * D5		->	B5
	 * D6		->	B6
	 * D7		->	B7
	 * RST		->	B8
	 * CS		->	B9
	 * RS/CD	->	B10
	 * WR		->	B11
	 * RD		->	B13
	 */
    GPIOB -> ODR = (GPIOB -> ODR & 0xef00) | (data & 0xfb) | ( (data & 0x0004) << 10);
	LCD_WR_STROBE();
}

void LCD_Write16(uint16_t data)
{
	LCD_Write8(data >> 8);
	LCD_Write8(data);

}
void LCD_WriteCMD(uint8_t data)
{
	/*	GPIO to data bus pin connections
	 * D0		->	B0
	 * D1		-> 	B1
	 * D2		->	B12
	 * D3		->	B3
	 * D4		->	B4
	 * D5		->	B5
	 * D6		->	B6
	 * D7		->	B7
	 * RST		->	B8
	 * CS		->	B9
	 * RS/CD	->	B10
	 * WR		->	B11
	 * RD		->	B13
	 */
	LCD_CS_LOW();
	LCD_CD_LOW();
    GPIOB -> ODR = (GPIOB -> ODR & 0xef00) | (data & 0xfd) | ( (data & 0x0004) << 10);
	LCD_WR_STROBE();
	LCD_CD_HIGH();
}
/**
 * \brief LCD Initialization
 *
 * \param
 *
 * \return void
*/
void LCD_Begin(void)
{
	LCD_GPIO_Init();
	delay(80);

	LCD_Reset();
	delay(250);

	LCD_WriteCMD(0x01);	//soft reset
	LCD_CS_HIGH();
	delay(80);

	LCD_WriteCMD(EXIT_SLEEP_MODE);
	LCD_CS_HIGH();
	delay(80);


	LCD_WriteCMD(TIMING_SETTING_PARTIAL);
	LCD_Write8(0x44U);
	LCD_CS_HIGH();

	LCD_WriteCMD(POWER_SETTING); //power setting
		LCD_Write8(0x07);
		LCD_Write8(0x42);
		LCD_Write8(0x18);
		LCD_CS_HIGH();

	LCD_WriteCMD(VCOM_CONTROL); //VCOM control
	    LCD_Write8(0x00);
	    LCD_Write8(0x07);
	    LCD_Write8(0x10);
		LCD_CS_HIGH();

	  LCD_WriteCMD(POWER_SETTING_NORMAL); //power setting for normal mode
	    LCD_Write8(0x01);
	    LCD_Write8(0x02); //Fosc setting
		LCD_CS_HIGH();

	  LCD_WriteCMD(PANEL_DRIVE_SETTING); //panel driving setting - 5 parameters
	    LCD_Write8(0x10); //REV=1, SM=0, GS=0 - grayscale inversion enabled : will invert colors
	    LCD_Write8(0x3B); //NL[5] - max lines
	    LCD_Write8(0x00); //SCN - scanning start position
	    LCD_Write8(0x02); //NDL (non-display area o/p level), PTS[3]
	    LCD_Write8(0x01); //PTG=0 (interval scan), ISC[3]=0001 (3 frames)
		LCD_CS_HIGH();

	  LCD_WriteCMD(FRAME_RATE_CONTROL); //frame rate and inversion control - 1 parameter
	    //LCD_Write8(0x03); //72FPS (default) - this had backlight flickering
	    LCD_Write8(0x00); //125 FPS (max) - this has no flickering
	    LCD_Write8(0x00);
	    LCD_Write8(0x00);
	    LCD_Write8(0x00);
		LCD_CS_HIGH();

	  LCD_WriteCMD(FRAME_MEMORY_ACCESS_SETTING); //frame memeory access and interface setting
	    LCD_Write8(0x00); //extra data is ignored
	    LCD_Write8(0x00); //all defaults
	    LCD_Write8(0x00);
	    LCD_Write8(0x00);
		LCD_CS_HIGH();

	  LCD_WriteCMD(SET_TEAR_OFF); //tearing compensation
		LCD_CS_HIGH();

	  LCD_WriteCMD(TIMING_SETTING_NORMAL); //display timing for normal mode
	    LCD_Write8(0x10); //BC=0 (frame inversion waveform)
	    LCD_Write8(0x10); //line period is 16 clocks
	    LCD_Write8(0x22); //front and back porch lines = 2
		LCD_CS_HIGH();

	  LCD_WriteCMD(GAMMA_SETTING); //gamma setting
	    LCD_Write8(0x00);
	    LCD_Write8(0x30);
	    LCD_Write8(0x36);
	    LCD_Write8(0x45);
	    LCD_Write8(0x04);
	    LCD_Write8(0x16);
	    LCD_Write8(0x37);
	    LCD_Write8(0x75);
	    LCD_Write8(0x77);
	    LCD_Write8(0x54);
	    LCD_Write8(0x0f);
	    LCD_Write8(0x00);
		LCD_CS_HIGH();

	  LCD_WriteCMD(SET_ADDRESS_MODE); //set address mode - 1 parameter
	    LCD_Write8(0x08); //BGR, horizontal flip
		LCD_CS_HIGH();

	  LCD_WriteCMD(0x20); //eneter normal mode - no param
		LCD_CS_HIGH();

	  LCD_WriteCMD(TIMING_SETTING_NORMAL);
		LCD_Write8(0x22); 	LCD_Write8(0x14); 	LCD_Write8(0x28);
		LCD_CS_HIGH();

	  LCD_WriteCMD(SET_PIXEL_FORMAT); //set pixel format - 1 parameter
	    LCD_Write8(0x55); //16-bit per pixel
		LCD_CS_HIGH();

		LCD_setAddrWindow(0, 0, Width-1, Height-1);
	    delay(120);

	  LCD_WriteCMD(SET_DISPLAY_ON); //set display on
	}


void LCD_Reset(void)
{
    //Initialize the device
    LCD_RST_HIGH();              // release from reset
    HAL_Delay(100);
    LCD_RST_LOW();               // reset
    HAL_Delay(100);
    LCD_RST_HIGH();              // release from reset
    HAL_Delay(100);
}

void LCD_setAddrWindow (int x0, int y0, int x1, int y1) {
  x1 = (x1 >= Width) ? Width -1 : x1;
  y1 = (y1 >= Height) ? Height -1 : y1;
  LCD_WriteCMD(SET_COL_ADDRESS); //set col address - 4 param
  LCD_Write16(x0);
  LCD_Write16(x1);
  LCD_CS_HIGH();

  LCD_WriteCMD(SET_PAGE_ADDRESS); //set page address - 4 param
  LCD_Write16(y0);
  LCD_Write16(y1);
  LCD_CS_HIGH();

  LCD_WriteCMD(WRITE_MEMORY_START); //write memory start
  LCD_CS_HIGH();

}

void LCD_fillScreen (uint32_t color)
{
  LCD_WriteCMD(0x2c); //write memory start
  for(int i=0; i<Height; i++)
  {
    for(int j=0; j<Width; j++)
    	{
    	LCD_Write8((color) & 0xff);
    	LCD_Write8(( color >> 8) & 0xff);
    	LCD_Write8(( color >> 16) & 0xff);
    	}
  }
  LCD_CS_HIGH();
}

void LCD_Invert(uint8_t value)
{
	(value !=0) ? LCD_WriteCMD(0x21) : LCD_WriteCMD(0x20);
	LCD_CS_HIGH();
}
