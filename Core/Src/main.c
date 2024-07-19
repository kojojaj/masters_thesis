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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define button_portType GPIO_TypeDef*
#define button_pinType uint16_t
typedef struct {
	button_portType button_port;
	button_pinType button_pin;
} button_HandleTypeDef;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BLINK_TIME 125
#define CLICK_TIME 50
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void buttons();
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
  /* USER CODE BEGIN 2 */
  Lcd_PortType ports[] = { LCD_D4_GPIO_Port, LCD_D5_GPIO_Port, LCD_D6_GPIO_Port, LCD_D7_GPIO_Port };
  Lcd_PinType pins[] = {LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin};
  Lcd_HandleTypeDef lcd;
  lcd = Lcd_create(ports, pins, LCD_RS_GPIO_Port, LCD_RS_Pin, LCD_E_GPIO_Port, LCD_E_Pin, LCD_4_BIT_MODE);

  bool isEnterPressed = 0;
  enum pos {UNITIES = 7, TENTHS = 9, HUNDREDTHS = 10};
  struct sMass {
	  uint8_t u;
	  uint8_t t;
	  uint8_t h;
  };
  struct sMass mass = {1, 5, 0};
  int cursor_pos = 0;
  char lcd_infoText[] = "masa = ";
  Lcd_cursor(&lcd, 0, cursor_pos);
  Lcd_string(&lcd, lcd_infoText);
  cursor_pos = strlen(lcd_infoText);
  Lcd_cursor(&lcd, 0, cursor_pos);
  Lcd_int(&lcd, mass.u);
  cursor_pos++;
  Lcd_string(&lcd, ".");
  cursor_pos = TENTHS;
  Lcd_int(&lcd, mass.t);
  cursor_pos = HUNDREDTHS;
  Lcd_int(&lcd, mass.h);

  while (isEnterPressed == false)
  {
	  if(HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port, BUTTON_UP_Pin) == GPIO_PIN_SET){
		  switch (cursor_pos){
		  case (UNITIES):
			mass.u++;
		  	HAL_Delay(CLICK_TIME);
		  break;
		  case (TENTHS):
			mass.t++;
		  HAL_Delay(CLICK_TIME);
		  break;
		  case (HUNDREDTHS):
			mass.h++;
		  HAL_Delay(CLICK_TIME);
		  break;
		  default:
			  break;
		  }
	  } else if (HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin) == GPIO_PIN_SET) {
		  switch (cursor_pos){
		  case (UNITIES):
			mass.u--;
		  HAL_Delay(CLICK_TIME);
		  break;
		  case (TENTHS):
			mass.t--;
		  HAL_Delay(CLICK_TIME);
		  break;
		  case (HUNDREDTHS):
			mass.h--;
		  HAL_Delay(CLICK_TIME);
		  break;
		  default:
		  break;
		  }
	  } else if (HAL_GPIO_ReadPin(BUTTON_LEFT_GPIO_Port, BUTTON_LEFT_Pin) == GPIO_PIN_SET){
		  switch (cursor_pos){
		  case (UNITIES):
				  cursor_pos = HUNDREDTHS;
		  HAL_Delay(CLICK_TIME);
		  break;
		  case (TENTHS):
				  cursor_pos = UNITIES;
		  HAL_Delay(CLICK_TIME);
		  break;
		  case (HUNDREDTHS):
				  cursor_pos = TENTHS;
		  HAL_Delay(CLICK_TIME);
		  default:
			  break;
		  }
	  } else if (HAL_GPIO_ReadPin(BUTTON_RIGHT_GPIO_Port, BUTTON_RIGHT_Pin) == GPIO_PIN_SET){
		  switch (cursor_pos){
		  case (UNITIES):
		  cursor_pos = TENTHS;
		  HAL_Delay(CLICK_TIME);
		  break;
		  case (TENTHS):
		  cursor_pos = HUNDREDTHS;
		  HAL_Delay(CLICK_TIME);
		  break;
		  case (HUNDREDTHS):
		  cursor_pos = UNITIES;
		  HAL_Delay(CLICK_TIME);
		  default:
		  break;
		  }
	  }
	  switch (cursor_pos){
	  case (UNITIES):
		  Lcd_cursor(&lcd, 0, cursor_pos);
	  	  Lcd_string(&lcd, " ");
	  	  HAL_Delay(BLINK_TIME);
	  	  Lcd_cursor(&lcd, 0, cursor_pos);
	  	  Lcd_int(&lcd, mass.u);
	  	  HAL_Delay(BLINK_TIME);
	  	  break;
	  case (TENTHS):
		  Lcd_cursor(&lcd, 0, cursor_pos);
	  	  Lcd_string(&lcd, " ");
	  	  HAL_Delay(BLINK_TIME);
	  	  Lcd_cursor(&lcd, 0, cursor_pos);
	  	  Lcd_int(&lcd, mass.t);
	  	  HAL_Delay(BLINK_TIME);
	  	  break;
	  case (HUNDREDTHS):
		  Lcd_cursor(&lcd, 0, cursor_pos);
	  	  Lcd_string(&lcd, " ");
	  	  HAL_Delay(BLINK_TIME);
	  	  Lcd_cursor(&lcd, 0, cursor_pos);
	  	  Lcd_int(&lcd, mass.h);
	  	  HAL_Delay(BLINK_TIME);
	      break;
	  default:
		  break;
	  }

  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*if(HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port, BUTTON_UP_Pin) == GPIO_PIN_SET){
		  m++;
		  cursor_pos = strlen(lcd_infoText);
		  Lcd_cursor(&lcd, 0, cursor_pos);
		  Lcd_int(&lcd, m/100);
		  cursor_pos++;
		  Lcd_string(&lcd, ".");
		  cursor_pos++;
		  Lcd_int(&lcd, m % 100);
	  }*/
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
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
