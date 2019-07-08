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
#include "ili9481.h"

#define TFTWIDTH   240
#define TFTHEIGHT  320

#define TFTLCD_DELAY 0xFF

// GPIO to data bus pin connections
// ---- PORT Pin ---     --- Data ----
// GPIOA, GPIO_PIN_9  -> BIT 0
// GPIOC, GPIO_PIN_7  -> BIT 1
// GPIOA, GPIO_PIN_10 -> BIT 2
// GPIOB, GPIO_PIN_3  -> BIT 3
// GPIOB, GPIO_PIN_5  -> BIT 4
// GPIOB, GPIO_PIN_4  -> BIT 5
// GPIOB, GPIO_PIN_10 -> BIT 6
// GPIOA, GPIO_PIN_8  -> BIT 7


#define LCD_CS_PIN  GPIO_PIN_0	// PB0 -> A3 // Chip Select goes to Analog 3
#define LCD_CD_PIN  GPIO_PIN_4	// PA4 -> A2 // Command/Data goes to Analog 2
#define LCD_WR_PIN  GPIO_PIN_1	// PA1 -> A1 // LCD Write goes to Analog 1
#define LCD_RD_PIN  GPIO_PIN_0	// PA0 -> A0 // LCD Read goes to Analog 0
#define LCD_RST_PIN GPIO_PIN_1	// PC1 -> RESET

#define LCD_CS_GPIO_PORT  GPIOB

#define LCD_CS_HIGH()     LCD_CS_GPIO_PORT -> BSRR = LCD_CS_PIN
#define LCD_CS_LOW()      LCD_CS_GPIO_PORT-> BSRR = LCD_CS_PIN  <<16

#define LCD_RD_GPIO_PORT  GPIOA
#define LCD_RD_HIGH()     LCD_RD_GPIO_PORT->BSRR = LCD_RD_PIN
#define LCD_RD_LOW()      LCD_RD_GPIO_PORT->BSRR = LCD_RD_PIN<<16

#define LCD_WR_GPIO_PORT  GPIOA
#define LCD_WR_HIGH()	  LCD_WR_GPIO_PORT->BSRR = LCD_WR_PIN
#define LCD_WR_LOW()      LCD_WR_GPIO_PORT->BSRR = LCD_WR_PIN <<16

#define LCD_CD_GPIO_PORT  GPIOA
#define LCD_CD_HIGH()     LCD_CD_GPIO_PORT->BSRR = LCD_CD_PIN
#define LCD_CD_LOW()      LCD_CD_GPIO_PORT->BSRR = LCD_CD_PIN <<16

#define LCD_RST_GPIO_PORT GPIOC
#define LCD_RST_HIGH()    LCD_RST_GPIO_PORT->BSRR = LCD_RST_PIN
#define LCD_RST_LOW()     LCD_RST_GPIO_PORT->BSRR = LCD_RST_PIN <<16

#define LCD_WR_STROBE() { LCD_WR_LOW(); delay(0); LCD_WR_HIGH();  delay(0);}	//If always stable remove delay
#define LCD_RD_STROBE() { LCD_RD_LOW(); delay(0); LCD_RD_HIGH();  delay(0);}	//If always stable remove delay

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
  // GPIO to data bus pin connections
  // ---- PORT Pin ---     --- Data ----
  //GPIOA, GPIO_PIN_9  	-> 	BIT 0
  //GPIOC, GPIO_PIN_7  	->	BIT 1
  //GPIOA, GPIO_PIN_10 	-> 	BIT 2
  //GPIOB, GPIO_PIN_3  	-> 	BIT 3
  //GPIOB, GPIO_PIN_5  	-> 	BIT 4
  //GPIOB, GPIO_PIN_4  	-> 	BIT 5
  //GPIOB, GPIO_PIN_10 	-> 	BIT 6
  //GPIOA, GPIO_PIN_8  	-> 	BIT 7
  //GPIOB, GPIO_PIN_0	-> 	CS
  //GPIOA, GPIO_PIN_4	-> 	CD
  //GPIOA, GPIO_PIN_1	-> 	WR
  //GPIOA, GPIO_PIN_0	-> 	RD
  //GPIOC, GPIO_PIN_1	-> 	RST

  	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA -> MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0);
	GPIOA -> MODER &= ~(GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1 | GPIO_MODER_MODE4_1 | GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1);
	GPIOA -> OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9 | GPIO_OTYPER_OT10);
	GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10 );
	GPIOA -> ODR &= ~(GPIO_ODR_OD0 | GPIO_ODR_OD1 | GPIO_ODR_OD4 | GPIO_ODR_OD8 | GPIO_ODR_OD9 | GPIO_ODR_OD10 );

  	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB -> MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE10_0);
	GPIOB -> MODER &= ~(GPIO_MODER_MODE0_1 | GPIO_MODER_MODE3_1 | GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1 | GPIO_MODER_MODE10_1);
	GPIOB -> OTYPER &= ~(GPIO_OTYPER_OT0 |GPIO_OTYPER_OT3 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT10);
	GPIOB -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR10 );
	GPIOB -> ODR &= ~(GPIO_ODR_OD0 | GPIO_ODR_OD3 | GPIO_ODR_OD4 | GPIO_ODR_OD5 | GPIO_ODR_OD10 );

  	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC -> MODER |= (GPIO_MODER_MODE1_0 | GPIO_MODER_MODE7_0);
	GPIOC -> MODER &= ~(GPIO_MODER_MODE1_1 | GPIO_MODER_MODE7_1);
	GPIOC -> OTYPER &= ~(GPIO_OTYPER_OT1 | GPIO_OTYPER_OT7);
	GPIOC -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR7);
	GPIOC -> ODR &= ~(GPIO_ODR_OD1 | GPIO_ODR_OD7);
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
	// ------ PORT -----     --- Data ----
	// GPIOA, GPIO_PIN_9  -> BIT 0 -> 0x01
	// GPIOC, GPIO_PIN_7  -> BIT 1 -> 0x02
	// GPIOA, GPIO_PIN_10 -> BIT 2 -> 0x04
	// GPIOB, GPIO_PIN_3  -> BIT 3 -> 0x08
	// GPIOB, GPIO_PIN_5  -> BIT 4 -> 0x10
	// GPIOB, GPIO_PIN_4  -> BIT 5 -> 0x20
	// GPIOB, GPIO_PIN_10 -> BIT 6 -> 0x40
	// GPIOA, GPIO_PIN_8  -> BIT 7 -> 0x80

	GPIOA->ODR = (GPIOA->ODR & 0xF8FF) |
			((data & 0x01) << 9) | ((data & 0x04) << 8) | ((data & 0x80) << 1);
	GPIOB->ODR = (GPIOB->ODR & 0xFBC7) |
			(data & 0x08) | ((data & 0x10) << 1) | ((data & 0x20) >> 1) | ((data & 0x40) << 4);
	GPIOC->ODR = (GPIOC->ODR & 0xFF7F) | ((data & 0x02) << 6);

	LCD_WR_STROBE();
}

uint8_t LCD_Read8(void)
{
	uint8_t data = 0;

	// ------ PORT -----     --- Data ----
	// GPIOA, GPIO_PIN_9  -> BIT 0 -> 0x01
	// GPIOC, GPIO_PIN_7  -> BIT 1 -> 0x02
	// GPIOA, GPIO_PIN_10 -> BIT 2 -> 0x04
	// GPIOB, GPIO_PIN_3  -> BIT 3 -> 0x08
	// GPIOB, GPIO_PIN_5  -> BIT 4 -> 0x10
	// GPIOB, GPIO_PIN_4  -> BIT 5 -> 0x20
	// GPIOB, GPIO_PIN_10 -> BIT 6 -> 0x40
	// GPIOA, GPIO_PIN_8  -> BIT 7 -> 0x80

	//GPIOA (Bit 0,2, 7)
	data |= ( ((GPIOA -> IDR & 0x0200) >> 9U ) | ((GPIOA -> IDR & 0x400) >> 8U )  | ((GPIOA -> IDR & 0x100) >> 1U ));
	//GPIOB (Bit 3, 4, 5, 6)
	data |= ( (GPIOB -> IDR & 0x0008 ) | ((GPIOB -> IDR & 0x0020) >> 1U )  | ((GPIOB -> IDR & 0x0010) << 1U)
			| ((GPIOB -> IDR & 0x0400) >> 4U ) );
	//GPIOC (Bit 1)
	data |=  ((GPIOC -> IDR & 0x0080) >> 6U) ;
	LCD_RD_STROBE();

	return data;
}

/**
 * \brief Writes 8-Bit register
 *
 * \param data	8-Bit Data
 * d = 0 is command
 * d = 1 is parameter
 * \return void
 */
void LCD_Write(uint8_t c, uint8_t d[], uint8_t i)
{
	uint8_t j=0;

	LCD_CS_LOW();

	LCD_CD_LOW();
	LCD_Write8(c);
	LCD_CD_HIGH();

	for (j=0; j<i; j++)
		{
		LCD_Write8(d[j]);
		}

	LCD_CS_HIGH();
}

/**
 * \brief Writes 8-Bit data
 *
 * \param data	8-Bit Data
 *
 * \return void
 */

void LCD_Read(uint8_t c, unsigned char d[], uint8_t i)
{
	uint8_t j=0;
	// send command
	LCD_CS_LOW();

	LCD_CD_LOW();
	LCD_Write8(c);
	LCD_CD_HIGH();

	//Setup datalines as inputs
	GPIOA -> MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10 | GPIO_MODER_MODER8);
	GPIOA -> PUPDR &= ~(GPIO_PUPDR_PUPDR9 | GPIO_PUPDR_PUPDR10 | GPIO_PUPDR_PUPDR8);
	GPIOB -> MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER5 | GPIO_MODER_MODER4 | GPIO_MODER_MODER10);
	GPIOB -> PUPDR &= ~(GPIO_PUPDR_PUPDR3 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR10);
	GPIOC -> MODER &= ~(GPIO_MODER_MODER7);
	GPIOC -> PUPDR &= ~(GPIO_PUPDR_PUPDR7);

	//read (first is garbage)
	LCD_Read8();

	for (j=0; j<i; j++)
		{
		d[j] = LCD_Read8();
		}

	//Set datalines as outputs
	GPIOA -> MODER |= ( GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0);
	GPIOA -> MODER &= ~(GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1);
	GPIOA -> OTYPER &= ~(GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9 | GPIO_OTYPER_OT10);
	GPIOA -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10 );

	GPIOB -> MODER |= (GPIO_MODER_MODE3_0 | GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE10_0);
	GPIOB -> MODER &= ~(GPIO_MODER_MODE3_1 | GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1 | GPIO_MODER_MODE10_1);
	GPIOB -> OTYPER &= ~(GPIO_OTYPER_OT3 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT10);
	GPIOB -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR10 );

	GPIOC -> MODER |= (GPIO_MODER_MODE7_0);
	GPIOC -> MODER &= ~(GPIO_MODER_MODE7_1);
	GPIOC -> OTYPER &= ~(GPIO_OTYPER_OT7);
	GPIOC -> OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR7);

	LCD_CS_HIGH();
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
	uint8_t dat[]={0x00, 0x00, 0x00, 0x00, 0x00};
	LCD_Reset();
//	LCD_Write(0x11, 0x00, 0);
	dat={0x01, 0x02, 0x03, 0x04, 0x05};
	LCD_Write(0xD0, dat, 5);
    HAL_Delay(25);
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


