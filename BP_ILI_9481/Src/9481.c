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

/*	GPIO to data bus pin connections
 * D0		->	B0
 * D1		-> 	B1
 * D2		->	B2
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

/*
 * LCD Orientation Vertical (see the writing at the bottom of the screen)
 * LCD Size (Hor=320; Height = 480)
 *
 */

#define LCD_CS_PIN  GPIO_PIN_9	// PB0 -> A3 // Chip Select goes to Analog 3
#define LCD_CD_PIN  GPIO_PIN_10	// PA4 -> A2 // Command/Data goes to Analog 2
#define LCD_WR_PIN  GPIO_PIN_11	// PA1 -> A1 // LCD Write goes to Analog 1
#define LCD_RD_PIN  GPIO_PIN_13	// PA0 -> A0 // LCD Read goes to Analog 0
#define LCD_RST_PIN GPIO_PIN_8	// PC1 -> RESET

#define LCD_CS_HIGH()     GPIOB -> BSRR = LCD_CS_PIN
#define LCD_CS_LOW()      GPIOB-> BSRR = LCD_CS_PIN <<16

#define LCD_RD_HIGH()     GPIOB->BSRR = LCD_RD_PIN
#define LCD_RD_LOW()      GPIOB->BSRR = LCD_RD_PIN	<<16

#define LCD_WR_HIGH()	  GPIOB->BSRR = LCD_WR_PIN
#define LCD_WR_LOW()      GPIOB->BSRR = LCD_WR_PIN 	<<16

#define LCD_CD_HIGH()     GPIOB->BSRR = LCD_CD_PIN
#define LCD_CD_LOW()      GPIOB->BSRR = LCD_CD_PIN 	<<16

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
	 * D2		->	B2
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
	GPIOB -> CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7 );		/*Output Max speed 50 MHz */
	GPIOB -> CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);		/* Gen Purpose out push-pull */
	GPIOB -> CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_MODE11  | GPIO_CRH_MODE13);
	GPIOB -> CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF13 );

	GPIOB -> BSRR |= ( GPIO_BSRR_BS8 | GPIO_BSRR_BS9 | GPIO_BSRR_BS10 | GPIO_BSRR_BS11 | GPIO_BSRR_BS13);
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
	 * D2		->	B2
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
	//It is included in the ODR the WR low bit 11
    GPIOB -> ODR = (GPIOB -> ODR & 0xf700) | (data & 0xff);
    GPIOB->BSRR = LCD_WR_PIN;
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
	 * D2		->	B2
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

	//chip select, Command data and write low included in the ODR,
    GPIOB -> ODR = (GPIOB -> ODR & 0xf100) | (data & 0x00ff);
    GPIOB -> BSRR = LCD_WR_PIN;
    GPIOB -> BSRR = LCD_CD_PIN;
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
	delay(120);

	LCD_Reset();
	delay(120);

	LCD_WriteCMD(0x11);		// Exit sleep mode
	LCD_CS_HIGH();
	delay(80);

	LCD_WriteCMD(0xD0);		// Power settings
	LCD_Write8(0x07);
	LCD_Write8(0x42);
	LCD_Write8(0x18);
	LCD_CS_HIGH();

	LCD_WriteCMD(0xD1);		// VCOM control
	LCD_Write8(0x00);
	LCD_Write8(0x07);
	LCD_Write8(0x10);
	LCD_CS_HIGH();

	LCD_WriteCMD(0xD2);		// Power setting for normal mode
	LCD_Write8(0x01);
	LCD_Write8(0x02);
	LCD_CS_HIGH();

	LCD_WriteCMD(0xc0);		// panel driving setting - 5 parameters
	LCD_Write8(0x10);
	LCD_Write8(0x3b);
	LCD_Write8(0x00);
	LCD_Write8(0x00);
	LCD_Write8(0x02);
	LCD_Write8(0x01);
	LCD_CS_HIGH();

	LCD_WriteCMD(0xc5);		// frame rate and inversion control - 1 parameter
	LCD_Write8(0x00);
	LCD_CS_HIGH();

	LCD_WriteCMD(0xb3);		//frame memeory access and interface setting
	LCD_Write8(0x00);
	LCD_Write8(0x00);
	LCD_Write8(0x00);
	LCD_Write8(0x00);
	LCD_CS_HIGH();

	LCD_WriteCMD(0x34);		// Tear off
	LCD_CS_HIGH();

	LCD_WriteCMD(0xc1);		//display timing for normal mode
	LCD_Write8(0x10);
	LCD_Write8(0x10);
	LCD_Write8(0x22);
	LCD_CS_HIGH();

	LCD_WriteCMD(0xc8);     // GAMMA CURVE CORRECTION POSITIVE
	LCD_Write8(0x00);LCD_Write8(0x32);LCD_Write8(0x36);LCD_Write8(0x45);
	LCD_Write8(0x06);LCD_Write8(0x16);LCD_Write8(0x37);LCD_Write8(0x75);
	LCD_Write8(0x77);LCD_Write8(0x54);LCD_Write8(0x0c);LCD_Write8(0x00);
	LCD_CS_HIGH();

	LCD_WriteCMD(0x36);		// set address mode - 1 parameter
	LCD_Write8(0xea);
	LCD_CS_HIGH();

	LCD_WriteCMD(0x13);		// enter normal mode - no param
	LCD_CS_HIGH();

	LCD_WriteCMD(0x3a);		// PIXEL FORMAT
	LCD_Write8(0x55);
	LCD_CS_HIGH();

	LCD_WriteCMD(0x29);		// TURN ON DISPLAY
	LCD_CS_HIGH();

	LCD_Invert(1);
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

  LCD_WriteCMD(0x2A); //set col address - 4 param
  LCD_Write16(x0);
  LCD_Write16(x1);
  LCD_CS_HIGH();

  LCD_WriteCMD(0x2BU); //set page address - 4 param
  LCD_Write16(y0);
  LCD_Write16(y1);
  LCD_CS_HIGH();
}

void LCD_fillScreen (uint16_t color)
{
  LCD_setAddrWindow (0,0,Height-1, Width-1);
  LCD_WriteCMD(0x2c); //write memory start
  for(int i=0; i<Height; i++)
  {
    for(int j=0; j<Width; j++)
    	{
    	LCD_Write8(color >> 8);
    	LCD_Write8(color);
    	}
  }
  LCD_CS_HIGH();
}

void LCD_Invert(uint8_t value)
{
	(value !=0) ? LCD_WriteCMD(0x21) : LCD_WriteCMD(0x20);
	LCD_CS_HIGH();
}

void LCD_fillRectangle (int x0, int y0, int x1, int y1, uint16_t color)
{
  LCD_setAddrWindow (x0,y0,x1-1,y1);
  LCD_WriteCMD(0x2c); 			//write memory start
  for(int i=y0; i<y1; i++)		//Height
  {
    for(int j=x0; j<x1; j++)	//Width
    	{
    	LCD_Write8(color >> 8);
    	LCD_Write8(color);
    	}
  }
  LCD_CS_HIGH();
}

void LCD_drawPixel (int x0, int y0, uint16_t color)
{
  LCD_setAddrWindow (x0,y0,x0,y0);
  LCD_WriteCMD(0x2c); 			//write memory start
  LCD_Write8(color >> 8);
  LCD_Write8(color);
  LCD_CS_HIGH();
}

void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor) {
    uint32_t i, b, j;
    LCD_setAddrWindow(x, y, x+font.width-1, y+font.height-1);

    for(i = 0; i < font.height; i++)
    {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++)
			{
				if((b << j) & 0x8000)  {LCD_drawPixel(x+j, y+i, color);}
				else {LCD_drawPixel(x+j, y+i, bgcolor);}
			}
    }
}

void LCD_WriteString(uint16_t x, uint16_t y,  char *str, FontDef font, uint16_t color, uint16_t bgcolor) {
    while(*str)
    {
        LCD_WriteChar(x, y, str[0], font, color, bgcolor);
        x += font.width;
        str++;
    }
}

void LCD_rotateScreen(uint8_t rotation){

	LCD_WriteCMD(0x36);

	if (rotation == 0){
		LCD_WriteCMD(0x40);
	}
	else if (rotation == 1){
		LCD_WriteCMD(0xc0);
	}
	else if (rotation == 2){
		LCD_WriteCMD(0x60);
	}
	else if (rotation == 3){
		LCD_WriteCMD(0xa0);
	}

	LCD_CS_HIGH();
}

void LCD_ONOFF(uint8_t state){
	if (state == 0){
		LCD_WriteCMD(0x28);
	}
	else if (state == 1){
		LCD_WriteCMD(0x29);
	}
	LCD_CS_HIGH();
}
