#define	BLACK   0xffffff
#define	BLUE    0xffff00
#define	RED     0x00ffff
#define	GREEN   0xff00ff
#define MAGENTA 0x00ff00
#define YELLOW  0x0000ff
#define CYAN	0xff0000
#define WHITE   0x000000
#define GRAY 	0x777777

#define   SET_COL_ADDRESS                 0x2AU
#define   SET_PAGE_ADDRESS                0x2BU
#define   WRITE_MEMORY_START              0x2CU
#define   WRITE_MEMORY_CONTINUE           0x3CU
#define   SET_DISPLAY_OFF                 0x28U
#define   SET_DISPLAY_ON                  0x29U
#define   SET_ADDRESS_MODE                0x36U
#define   EXIT_INVERT_MODE                0x20U
#define   ENTER_INVERT_MODE               0x21U
#define   ENTER_NORMAL_MODE               0x13U //0 param
#define   EXIT_SLEEP_MODE                 0x11U
#define   SET_TEAR_OFF                    0x34U //0 param
#define   SET_TEAR_ON                     0x35U //1 param
#define   SET_PIXEL_FORMAT                0x3AU
#define   SET_TEAR_SCANLINE               0x44U //2 param
#define   FRAME_MEMORY_ACCESS_SETTING     0xB3U //4 param
#define   SET_DISPLAY_MODE                0xB4U //1 param
#define   PANEL_DRIVE_SETTING             0xC0U //6 param
#define   TIMING_SETTING_NORMAL           0xC1U //3 param
#define   TIMING_SETTING_PARTIAL          0xC2U //3 param
#define   FRAME_RATE_CONTROL              0xC5U //1 param
#define   INTERFACE_CONTROL               0xC6U //1 param
#define   POWER_SETTING                   0xD0U //3 param
#define   VCOM_CONTROL                    0xD1U //3 param
#define   POWER_SETTING_NORMAL            0xD2U //2 param
#define   POWER_SETTING_PARTIAL           0xD3U //2 param
#define GAMMA_SETTING 0xC8U //12 param

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
void LCD_fillScreen (uint32_t color);
void LCD_Invert(uint8_t value);
//void LCD_Read(uint8_t c, unsigned char d[], uint8_t i);
//uint8_t LCD_Read8(void);
