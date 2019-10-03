#include "fonts.h"

#define	BLACK   0x0000
#define	BLUE     0xf800
#define	GREEN   0x07e0
#define	RED    0x001f
#define MAGENTA 0xf81f
#define CYAN   0xFFE0
#define YELLOW	0x07FF
#define WHITE   0xffff
#define GRAY 	0x5555

static const int Width = 320; //constants
static const int Height = 480;



void LCD_GPIO_Init(void);
void LCD_Begin(void);
void LCD_Reset(void);
void delay(unsigned int t);

void LCD_Write8(uint8_t data);
void LCD_Write16(uint16_t data);
void LCD_WriteCMD(uint8_t data);

void LCD_setAddrWindow (int x0, int y0, int x1, int y1);
void LCD_Invert(uint8_t value);
void LCD_fillScreen (uint16_t color);
void LCD_fillRectangle (int x0, int y0, int x1, int y1, uint16_t color);
void LCD_drawPixel (int x0, int y0, uint16_t color);
void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void LCD_WriteString(uint16_t x, uint16_t y, char *str, FontDef font, uint16_t color, uint16_t bgcolor);

//void LCD_Read(uint8_t c, unsigned char d[], uint8_t i);
//uint8_t LCD_Read8(void);
