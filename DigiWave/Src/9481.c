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
 */

/*
 * LCD Orientation Vertical (see the writing at the bottom of the screen)
 * LCD Size (Hor=320; Height = 480)
 *
 */



#define LCD_CS_PIN  GPIO_PIN_11	// PB0 -> A3 // Chip Select goes to Analog 3
#define LCD_CD_PIN  GPIO_PIN_1	// PA4 -> A2 // Command/Data goes to Analog 2
#define LCD_WR_PIN  GPIO_PIN_0	// PA1 -> A1 // LCD Write goes to Analog 1
#define LCD_RST_PIN GPIO_PIN_2	// PC1 -> RESET
#define LCD_RD_PIN  GPIO_PIN_12

#define LCD_CS_HIGH()     GPIOB -> BSRR = LCD_CS_PIN
#define LCD_CS_LOW()      GPIOB-> BSRR = LCD_CS_PIN <<16

#define LCD_WR_HIGH()	  GPIOB->BSRR = LCD_WR_PIN
#define LCD_WR_LOW()      GPIOB->BSRR = LCD_WR_PIN 	<<16

#define LCD_CD_HIGH()     GPIOB->BSRR = LCD_CD_PIN
#define LCD_CD_LOW()      GPIOB->BSRR = LCD_CD_PIN 	<<16

#define LCD_RST_HIGH()    GPIOB->BSRR = LCD_RST_PIN
#define LCD_RST_LOW()     GPIOB->BSRR = LCD_RST_PIN <<16

#define LCD_RD_HIGH()	  GPIOB->BSRR = LCD_RD_PIN
#define LCD_RD_LOW()      GPIOB->BSRR = LCD_RD_PIN 	<<16

#define LCD_WR_STROBE() { LCD_WR_LOW();  LCD_WR_HIGH(); }	//If always stable remove
#define LCD_RD_STROBE() { LCD_RD_LOW();  LCD_RD_HIGH(); }	//If always stable remove

#define ABS(x) ((x) > 0 ? (x) : -(x))

void delay(unsigned int t)
{
	for (; t > 0; t-- )
	{
	__NOP();
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
	 */
	RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;	/* Enable clock for GPIOB */
	GPIOB -> CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7 );		/*Output Max speed 50 MHz */
	GPIOB -> CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1  | GPIO_CRL_CNF2  | GPIO_CRL_CNF3  | GPIO_CRL_CNF4  | GPIO_CRL_CNF5  | GPIO_CRL_CNF6  | GPIO_CRL_CNF7  );		/* Gen Purpose out push-pull */
	GPIOB -> CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10| GPIO_CRH_MODE11 | GPIO_CRH_MODE12);
	GPIOB -> CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9  | GPIO_CRH_CNF10 | GPIO_CRH_CNF11 | GPIO_CRH_CNF12);

	GPIOB -> BSRR |= (GPIO_BSRR_BS0 | GPIO_BSRR_BS1  | GPIO_BSRR_BS2  |  GPIO_BSRR_BS11 |  GPIO_BSRR_BS12);

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
	 */
	//It is included in the ODR the WR low bit 11
	LCD_CS_LOW();
    GPIOB -> ODR = (GPIOB -> ODR & 0x1807) | ((data & 0x00ff)<<3);
    LCD_WR_STROBE();
    LCD_CS_HIGH();
}


void LCD_Write16(uint16_t data)
{
	LCD_Write8(data >> 8);
	LCD_Write8(data);
}
void LCD_WriteCMD(uint8_t data)
{
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
	 */
	//chip select, Command data and write low included in the ODR,
	LCD_CD_LOW();
	LCD_CS_LOW();
	GPIOB -> ODR = (GPIOB -> ODR & 0x1807) | ((data & 0xff)<<3);
    LCD_WR_STROBE();
    LCD_CD_HIGH();
    LCD_CS_HIGH();
}

void LCD_Read8(uint8_t data, char Rxbuf[16], uint8_t nreads)
{
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
	 */
	//It is included in the ODR the WR low bit 11
	LCD_CS_LOW();
	LCD_CD_LOW();
	GPIOB -> ODR = (GPIOB -> ODR & 0x1807) | ((data & 0xff)<<3);
    LCD_WR_STROBE();
	LCD_CD_HIGH();

	delay(10);

	GPIOB -> CRL &= ~( GPIO_CRL_MODE3 | GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7 );
	GPIOB -> CRL |= (GPIO_CRL_CNF3_0  | GPIO_CRL_CNF4_0  | GPIO_CRL_CNF5_0  | GPIO_CRL_CNF6_0  | GPIO_CRL_CNF7_0  );
	GPIOB -> CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10);
	GPIOB -> CRH |= (GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0  | GPIO_CRH_CNF10_0 );

	delay(1000);
for (int i = 0; i< nreads; i++)
	{
	LCD_RD_LOW();
	delay(10);
    Rxbuf[i]= ((GPIOB -> IDR & 0x07f8 )>>3);
    LCD_RD_HIGH();
    delay(100);
	}

    LCD_CS_HIGH();
    LCD_GPIO_Init();
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
	HAL_Delay(1);

	LCD_Reset();
	HAL_Delay(100);

	LCD_WriteCMD(0x11);		// Exit sleep mode

	HAL_Delay(100);

	LCD_WriteCMD(0xD0);		// Power settings
	LCD_Write8(0x07);
	LCD_Write8(0x42);
	LCD_Write8(0x18);


	LCD_WriteCMD(0xD1);		// VCOM control
	LCD_Write8(0x00);
	LCD_Write8(0x07);
	LCD_Write8(0x10);


	LCD_WriteCMD(0xD2);		// Power setting for normal mode
	LCD_Write8(0x01);
	LCD_Write8(0x02);
;

	LCD_WriteCMD(0xc0);		// panel driving setting - 5 parameters
	LCD_Write8(0x10);
	LCD_Write8(0x3b);
	LCD_Write8(0x00);
	LCD_Write8(0x00);
	LCD_Write8(0x02);
	LCD_Write8(0x01);


	LCD_WriteCMD(0xc5);		// frame rate and inversion control - 1 parameter
	LCD_Write8(0x00);


	LCD_WriteCMD(0xb3);		//frame memory access and interface setting
	LCD_Write8(0x00);
	LCD_Write8(0x00);
	LCD_Write8(0x00);
	LCD_Write8(0x00);


	LCD_WriteCMD(0x34);		// Tear off


	LCD_WriteCMD(0xc1);		//display timing for normal mode
	LCD_Write8(0x10);
	LCD_Write8(0x10);
	LCD_Write8(0x22);


	LCD_WriteCMD(0xc8);     // GAMMA CURVE CORRECTION POSITIVE
	LCD_Write8(0x00);LCD_Write8(0x32);LCD_Write8(0x36);LCD_Write8(0x45);
	LCD_Write8(0x06);LCD_Write8(0x16);LCD_Write8(0x37);LCD_Write8(0x75);
	LCD_Write8(0x77);LCD_Write8(0x54);LCD_Write8(0x0c);LCD_Write8(0x00);


	LCD_WriteCMD(0x36);		// set address mode - 1 parameter
	LCD_Write8(0x3b);		// ROTATION


	LCD_WriteCMD(0x13);		// enter normal mode - no param


	LCD_WriteCMD(0x3a);		// PIXEL FORMAT
	LCD_Write8(0x05);


	LCD_WriteCMD(0x29);		// TURN ON DISPLAY
	HAL_Delay(1000);

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


  LCD_WriteCMD(0x2BU); //set page address - 4 param
  LCD_Write16(y0);
  LCD_Write16(y1);

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

}

void LCD_Invert(uint8_t value)
{
	(value !=0) ? LCD_WriteCMD(0x21) : LCD_WriteCMD(0x20);

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

}

void LCD_drawPixel (int x0, int y0, uint16_t color)
{
  LCD_setAddrWindow (x0,y0,x0,y0);
  LCD_WriteCMD(0x2c); 			//write memory start
  LCD_Write8(color >> 8);
  LCD_Write8(color);

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


}

void LCD_ONOFF(uint8_t state){
	if (state == 0){
		LCD_WriteCMD(0x28);
	}
	else if (state == 1){
		LCD_WriteCMD(0x29);
	}
}

void LCD_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,  uint8_t size, uint32_t color)
{
	int deltaX = ABS(x2 - x1);
	int deltaY = ABS(y2 - y1);
	int signX = x1 < x2 ? 1 : -1;
	int signY = y1 < y2 ? 1 : -1;
	int error = deltaX - deltaY;
	int error2 = 0;
	for (;;)
	{
		LCD_fillRectangle(x1, y1, x1+size, y1+size, color);
		if (x1 == x2 && y1 == y2)
		break;
		error2 = error * 2;
		if (error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}
		if (error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

void LCD_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint32_t color24)
{
	LCD_Line(x1, y1, x2, y2, size, color24);
	LCD_Line(x2, y2, x3, y3, size, color24);
	LCD_Line(x3, y3, x1, y1, size, color24);
}

void LCD_Triangle_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color24)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
	curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1)
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1)
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)
	{
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	}
	else
	{
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		LCD_Line(x, y, x3, y3, 1, color24);

		num += numadd;
		if (num >= den)
		{
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

void LCD_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, uint32_t color24)
{
	LCD_Line(x, y, x + w, y, size, color24);
	LCD_Line(x, y + h, x + w, y + h, size, color24);
	LCD_Line(x, y, x, y + h, size, color24);
	LCD_Line(x + w, y, x + w, y + h, size, color24);
}

void LCD_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint32_t color24)
{
	int a_, b_, P;
	a_ = 0;
	b_ = radius;
	P = 1 - radius;
	while (a_ <= b_)
	{
		if (fill == 1)
		{
			LCD_fillRectangle(x - a_, y - b_, x - a_ + 2 * a_ + 1,  y - b_ +2 * b_ + 1, color24);
			LCD_fillRectangle(x - b_, y - a_, x - b_ + 2 * b_ + 1, y - a_ + 2 * a_ + 1, color24);
		}
		else
		{
			LCD_fillRectangle(a_ + x, b_ + y, a_ + x + size, b_ + y + size, color24);
			LCD_fillRectangle(b_ + x, a_ + y, b_ + x + size, a_ + y + size, color24);
			LCD_fillRectangle(x - a_, b_ + y, x - a_ + size, b_ + y + size, color24);
			LCD_fillRectangle(x - b_, a_ + y, x - b_ + size, a_ + y + size, color24);
			LCD_fillRectangle(b_ + x, y - a_, b_ + x + size, y - a_ + size, color24);
			LCD_fillRectangle(a_ + x, y - b_, a_ + x + size, y - b_ + size, color24);
			LCD_fillRectangle(x - a_, y - b_, x - a_ + size, y - b_ + size, color24);
			LCD_fillRectangle(x - b_, y - a_, x - b_ + size, y - a_ + size, color24);
		}
		if (P < 0)
		{
			P = (P + 3) + (2 * a_);
			a_++;
		}
		else
		{
			P = (P + 5) + (2 * (a_ - b_));
			a_++;
			b_--;
		}
	}
}

void LCD_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint32_t color24)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			LCD_fillRectangle(x0 + x, y0 + y, x0 + x + size, y0 + y + size, color24);
			LCD_fillRectangle(x0 + y, y0 + x, x0 + y + size, y0 + x + size, color24);
		}
		if (cornername & 0x2) {
			LCD_fillRectangle(x0 + x, y0 - y, x0 + x + size, y0 - y + size, color24);
			LCD_fillRectangle(x0 + y, y0 - x, x0 + y + size, y0 - x + size, color24);
		}
		if (cornername & 0x8) {
			LCD_fillRectangle(x0 - y, y0 + x, x0 - y + size, y0 + x + size, color24);
			LCD_fillRectangle(x0 - x, y0 + y, x0 - x + size, y0 + y + size, color24);
		}
		if (cornername & 0x1) {
			LCD_fillRectangle(x0 - y, y0 - x, x0 - y + size, y0 - x + size, color24);
			LCD_fillRectangle(x0 - x, y0 - y, x0 - x + size, y0 - y + size, color24);
		}
	}
}

void LCD_Rect_Round(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint32_t color24)
{
	LCD_Line(x + (r + 2), y, x + length + size - (r + 2), y, size, color24);
	LCD_Line(x + (r + 2), y + width - 1, x + length + size - (r + 2), y + width - 1, size, color24);
	LCD_Line(x, y + (r + 2), x, y + width - size - (r + 2), size, color24);
	LCD_Line(x + length - 1, y + (r + 2), x + length - 1, y + width - size - (r + 2), size, color24);

	LCD_Circle_Helper(x + (r + 2), y + (r + 2), (r + 2), 1, size, color24);
	LCD_Circle_Helper(x + length - (r + 2) - 1, y + (r + 2), (r + 2), 2, size, color24);
	LCD_Circle_Helper(x + length - (r + 2) - 1, y + width - (r + 2) - 1, (r + 2), 4, size, color24);
	LCD_Circle_Helper(x + (r + 2), y + width - (r + 2) - 1, (r + 2), 8, size, color24);
}

void LCD_Circle_Fill_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color24)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			LCD_Line(x0 + x, y0 - y, x0 + x, y0 - y + 2 * y + delta, 1, color24);
			LCD_Line(x0 + y, y0 - x, x0 + y, y0 - x + 2 * x + delta, 1, color24);
		}
		if (cornername & 0x2) {
			LCD_Line(x0 - x, y0 - y, x0 - x, y0 - y + 2 * y + delta, 1, color24);
			LCD_Line(x0 - y, y0 - x, x0 - y, y0 - x + 2 * x + delta, 1, color24);
		}
	}
}

void LCD_Rect_Round_Fill(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint32_t color24)
{
	LCD_fillRectangle(x + r, y, x + r + length - 2 * r, y + width, color24);
	LCD_Circle_Fill_Helper(x + length - r - 1, y + r, r, 1, width - 2 * r - 1, color24);
	LCD_Circle_Fill_Helper(x + r, y + r, r, 2, width - 2 * r - 1, color24);
}

//void LCD_WriteBMP()
