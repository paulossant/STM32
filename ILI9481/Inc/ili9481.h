
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define LIGHTGRAY 0xCDB6


void LCD_Begin(void);
void LCD_GPIO_Init(void);
void LCD_Reset(void);


void LCD_Write8(uint8_t data);
void LCD_Write(uint8_t c, uint8_t d[], uint8_t i);
void LCD_Read(uint8_t c, unsigned char d[], uint8_t i);
uint8_t LCD_Read8(void);
