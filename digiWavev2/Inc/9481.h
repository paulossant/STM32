#include "fonts.h"

#define	BLACK   	0x0000
#define	BLUE    	0x001F
#define	DARKBLUE    0x0008
#define	RED     	0xF800
#define	DARKRED     0x4800
#define	GREEN   	0x07E0
#define	DARKGREEN  	0x01E0
#define CYAN    	0x07FF
#define MAGENTA 	0xF81F
#define YELLOW  	0xFFE0
#define WHITE   	0xFFFF
#define GRAY 		0x5555
#define DARKGRAY 	0x1111

static const int Width = 240; //constants
static const int Height = 320;



void LCD_GPIO_Init(void);
void LCD_Begin(void);
void LCD_Reset(void);
void delay(unsigned int t);

void LCD_Write8(uint8_t data);
void LCD_Write16(uint16_t data);
void LCD_WriteCMD(uint8_t data);

void LCD_Read8(uint8_t data, char Rxbuf[16], uint8_t nreads);

void LCD_setCursor(unsigned int x, unsigned int y);
void LCD_setAddrWindow (int x0, int y0, int x1, int y1);
void LCD_Invert(uint8_t value);
void LCD_fillScreen (uint16_t color);
void LCD_fillRectangle (int x0, int y0, int x1, int y1, uint16_t color);
void LCD_drawPixel (int x0, int y0, uint16_t color);
void LCD_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
void LCD_WriteString(uint16_t x, uint16_t y, char *str, FontDef font, uint16_t color, uint16_t bgcolor);

void LCD_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,  uint8_t size, uint32_t color);
void LCD_Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint8_t size, uint32_t color24);
void LCD_Triangle_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint32_t color24);
void LCD_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, uint32_t color24);
void LCD_Circle(uint16_t x, uint16_t y, uint8_t radius, uint8_t fill, uint8_t size, uint32_t color24);
void LCD_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t size, uint32_t color24);
void LCD_Rect_Round(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint8_t size, uint32_t color24);
void LCD_Circle_Fill_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint32_t color24);
void LCD_Rect_Round_Fill(uint16_t x, uint16_t y, uint16_t length, uint16_t width, uint16_t r, uint32_t color24);

