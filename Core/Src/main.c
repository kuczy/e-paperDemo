/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//EPD
#include "Display_EPD_W21.h"
#include "Ap_29demo.h"
//GUI
#include "GUI_Paint.h"
#include "fonts.h"
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

/* USER CODE BEGIN PV */
unsigned char BlackImage[5000];//Define canvas space
unsigned char fen_L,fen_H,miao_L,miao_H;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
	//Clear screen
	EPD_HW_Init(); //Electronic paper initialization
	EPD_WhiteScreen_White(); //Show all white
	EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!

	EPD_HW_Init(); //EPD init
	EPD_WhiteScreen_ALL(gImage_1);//EPD_picture1
	EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
	HAL_Delay(5000);

	EPD_HW_Init(); //EPD init
	EPD_WhiteScreen_ALL(gImage_4);//EPD_picture1
	EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
	HAL_Delay(5000);

	//////////////////////Partial refresh digital presentation//////////////////////////////////////
	EPD_HW_Init(); //Electronic paper initialization
	EPD_SetRAMValue_BaseMap(gImage_logo);  //Partial refresh background color
	EPD_Dis_Part(0,32,gImage_num1,32,32); //x,y,DATA,Resolution 32*32
	EPD_Dis_Part(0,32,gImage_num2,32,32); //x,y,DATA,Resolution 32*32
	EPD_Dis_Part(0,32,gImage_num3,32,32); //x,y,DATA,Resolution 32*32
	EPD_Dis_Part(0,32,gImage_num4,32,32); //x,y,DATA,Resolution 32*32
	HAL_Delay(2000);

	//////////////////////Partial refresh time demo/////////////////////////////////////
	EPD_HW_Init(); //Electronic paper initialization
	EPD_SetRAMValue_BaseMap(gImage_basemap); //Partial refresh background color
	for(fen_H=0;fen_H<6;fen_H++)
	{
		for(fen_L=0;fen_L<10;fen_L++)
		{
			for(miao_H=0;miao_H<6;miao_H++)
			{
				for(miao_L=0;miao_L<10;miao_L++)
				{
					EPD_Dis_Part_myself(64,40,Num[miao_L],         //x-A,y-A,DATA-A
							64,72,Num[miao_H],         //x-B,y-B,DATA-B
							64,112,gImage_numdot,       //x-C,y-C,DATA-C
							64,154,Num[fen_L],         //x-D,y-D,DATA-D
							64,186,Num[fen_H],32,64);	 //x-E,y-E,DATA-E,Resolution 32*64

					if((fen_L==0)&&(miao_H==0)&&(miao_L==5))
						goto Clear;
				}
			}

		}
	}

	////////////////////////////////////////////////////////////////////////
	//Clear screen
	Clear:
	EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!

	///////////////////////////GUI///////////////////////////////////////////////////////////////////////////////////
	//Data initialization settings
	Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE); //Set screen size and display orientation
	Paint_SelectImage(BlackImage);//Set the virtual canvas data storage location

	/**************Drawing**********************/
	EPD_HW_Init_GUI(); //EPD init GUI
	Paint_Clear(WHITE);
	//Point
	Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
	Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);
	//Line
	Paint_DrawLine(20, 10, 70, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
	Paint_DrawLine(70, 10, 20, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
	//Rectangle
	Paint_DrawRectangle(20, 10, 70, 60, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
	Paint_DrawRectangle(85, 10, 130, 60, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
	//Circle
	Paint_DrawCircle(30, 120, 25, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
	Paint_DrawCircle(100, 120, 25, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
	EPD_Display(BlackImage);//display image
	EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
	HAL_Delay(2000); //2s

	/***********String***************************/
	EPD_HW_Init_GUI(); //EPD init GUI
	Paint_Clear(WHITE);
	Paint_DrawString_EN(0, 0, "Good Display", &Font8, WHITE, BLACK);  //5*8
	Paint_DrawString_EN(0, 10, "Good Display", &Font12, BLACK, WHITE); //7*12
	Paint_DrawString_EN(0, 25, "Good Display", &Font16, WHITE, BLACK); //11*16
	Paint_DrawString_EN(0, 45, "Good Display", &Font20, BLACK, WHITE); //14*20
	Paint_DrawString_EN(0, 80, "Good Display", &Font24, WHITE, BLACK); //17*24
	EPD_Display(BlackImage); //display image
	EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
	HAL_Delay(2000); //2s
	/*************Number************************/
	EPD_HW_Init_GUI(); //EPD init GUI
	Paint_Clear(WHITE);
	Paint_DrawNum(0, 0, 123456789, &Font8, BLACK, WHITE);  //5*8
	Paint_DrawNum(0, 10, 123456789, &Font12, WHITE, BLACK); //7*12
	Paint_DrawNum(0, 25, 123456789, &Font16, BLACK, WHITE); //11*16
	Paint_DrawNum(0, 45, 123456789, &Font20, WHITE, BLACK); //14*20
	Paint_DrawNum(0, 70, 123456789, &Font24, BLACK, WHITE); //17*24
	EPD_Display(BlackImage); //display image
	EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
	HAL_Delay(2000); //2s

	//Clear screen
	EPD_HW_Init(); //Electronic paper initialization
	EPD_WhiteScreen_White(); //Show all white
	EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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
	__disable_irq();
	while (1)
	{
	}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
