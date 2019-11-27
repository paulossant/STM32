/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "9481.h"
#include "fonts.h"
#include "touch.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc2;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
u_int64_t counter = 0;
char TxBuf[100];
uint8_t i = 0 ;
volatile uint16_t valores[4];
uint32_t ADval[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  LCD_Begin();
  HAL_Delay(100);
  LCD_fillScreen(RED);
  LCD_fillScreen(GREEN);
  LCD_fillScreen(BLUE);
  LCD_fillScreen(MAGENTA);
  LCD_fillScreen(YELLOW);
  LCD_fillScreen(CYAN);
  LCD_fillScreen(WHITE);
  LCD_fillScreen(GRAY);
  LCD_fillScreen(BLACK);
  HAL_ADC_Start(&hadc2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  LCD_fillScreen(BLACK);
//	  LCD_fillRectangle(10, 10, 230, 70, BLUE);
//	  LCD_fillRectangle(10, 90, 230, 150, BLUE);
//	  LCD_fillRectangle(10, 170, 230, 230, BLUE);
//	  LCD_fillRectangle(10, 250, 230, 310, BLUE);
//	  LCD_fillRectangle(250, 10, 470, 70, RED);
//	  LCD_fillRectangle(250, 90, 470, 150, RED);
//	  LCD_fillRectangle(250, 170, 470, 230, RED);
//	  LCD_fillRectangle(250, 250, 470, 310, RED);
//	  LCD_WriteString(50, 20, "Option 1", Font_7x10, WHITE, BLUE);
//	  LCD_WriteString(50, 120, "Option 2", Font_11x18, WHITE, BLUE);
//	  LCD_Triangle(100, 100, 120,120,150,100, 4, CYAN);
//	  LCD_Triangle_Fill(0, 0, 50,50,400,10, MAGENTA);
//	  LCD_Rect(100,100, 230, 150, 5, BLUE);
//	  LCD_Circle(200,200, 50, 0, 1, YELLOW);
//	  LCD_Rect_Round(20,200, 100,100, 10, 2, WHITE);
//	  LCD_Rect_Round_Fill(200,10, 100,100, 15,GRAY);
//	  LCD_Line(100, 100, 200, 200, 2, GREEN);  //descida
//	  LCD_Line(100, 100, 200, 100, 2, GREEN); //H
//	  HAL_Delay(50);
//	  LCD_Line(400, 100, 400, 300, 5, GREEN); //V
//	  HAL_Delay(50);
//	  LCD_Line(300, 100, 400, 50, 10, GREEN);  //subida
//	  HAL_Delay(1000);
	  LCD_fillScreen(BLACK);
	  touch_read(valores);
	  sprintf(TxBuf,"%i", valores[0]);
	  LCD_WriteString(50, 20, TxBuf, Font_7x10, WHITE, BLUE);
	  sprintf(TxBuf,"%i", valores[1]);
	  LCD_WriteString(50, 120, TxBuf, Font_7x10, WHITE, BLUE);
	  sprintf(TxBuf,"%i", valores[2]);
	  LCD_WriteString(200, 20, TxBuf, Font_7x10, WHITE, BLUE);
	  sprintf(TxBuf,"%i", valores[3]);
	  LCD_WriteString(200, 120, TxBuf, Font_7x10, WHITE, BLUE);
	  HAL_Delay(1000);
	  LCD_fillScreen(BLACK);
	  for(;;){
		  touch_read(valores);
		  sprintf(TxBuf,"%i", valores[0]);
		  LCD_WriteString(50, 20, TxBuf, Font_7x10, WHITE, BLUE);
		  sprintf(TxBuf,"%i", valores[1]);
		  LCD_WriteString(50, 120, TxBuf, Font_7x10, WHITE, BLUE);
		  sprintf(TxBuf,"%i", valores[2]);
		  LCD_WriteString(50, 320, TxBuf, Font_7x10, WHITE, BLUE);
		  if (valores[2] < 3800){
		  LCD_drawPixel (valores[1]/7-50, valores[0]/11-25, GREEN);
		  }
		  HAL_Delay(0);
	  }

//	    if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) == HAL_OK)
//	    {
//	    ADval[0] = HAL_ADC_GetValue(&hadc2);
//	    }
//	    if (HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY) == HAL_OK)
//	    {
//	    ADval[1] = HAL_ADC_GetValue(&hadc2);
//	    }
//	    __NOP();
//	    ADval[0] = 0;
//	    ADval[1] = 0;
//	  	HAL_Delay(250);
//	  	GPIOB -> CRL |= GPIO_CRL_MODE1;
//	  	GPIOB -> CRL &= ~GPIO_CRL_CNF1;
//	  	GPIOB -> BSRR |= GPIO_BSRR_BS1;
//	  	HAL_Delay(2000);
//	  	GPIOB -> BSRR |= GPIO_BSRR_BR1;
//	  	HAL_Delay(2000);
//	  	GPIOB -> CRL &= ~GPIO_CRL_MODE1;
//	  	GPIOB -> CRL &= ~GPIO_CRL_CNF1;


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */
  /** Common config 
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 2;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
