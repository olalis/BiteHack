/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdint.h"

#include "comm.h"
#include "am2320.h"
#include "lm35dz.h"
#include "lps25hb.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t start_command[4] = {0xAA, 0xAA, 0xAA, 0xAA};
float humidity, pressure, temperature1, temperature2, temperature3;
volatile uint8_t help_button = 0;

volatile uint16_t adc_buf[200];
volatile uint16_t adc_idx = 0;

static uint32_t current_time = 0U;

volatile uint8_t uart_rx = 0U;
volatile uint32_t sft_cnt = 0;

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
  MX_USART3_UART_Init();
  MX_I2C4_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  Lps25hb_Init();
//  Lps25hb_SetCalib(114);
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_UART_Receive_IT(&huart3, &uart_rx, 1);

  current_time = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  while((HAL_GetTick() - current_time) < 2000U);
	  current_time = HAL_GetTick();

	  Am2320_Read(&hi2c4, &humidity, &temperature1);
	  temperature2 = Lm35dz_Read();
	  Lps25hb_Read(&pressure, &temperature3);

	  // Start command
	  Comm_SendBytes(start_command, 4);

	  // Send temperature
	  Comm_SendBytes((uint8_t*)&temperature1, 4);

	  Comm_SendBytes((uint8_t*)&temperature2, 4);

	  Comm_SendBytes((uint8_t*)&temperature3, 4);

	  // Send pressure
	  Comm_SendBytes((uint8_t*)&pressure, 4);

	  // Send humidity
	  Comm_SendBytes((uint8_t*)&humidity, 4);

	  // Send help_button
	  Comm_SendBytes((uint8_t*)&help_button, 1);
	  if(help_button == 1U)
	  {
		  help_button = 0U;
	  }

	  if(sft_cnt > 0U)
	  {
		sft_cnt--;
	  	if(sft_cnt % 2 == 1U)
	  	{
	  		HAL_GPIO_WritePin(LEDS_R_GPIO_Port, LEDS_R_Pin, GPIO_PIN_SET);
	  		HAL_GPIO_WritePin(LEDS_G_GPIO_Port, LEDS_G_Pin, GPIO_PIN_SET);
	  	}
	  	else
	  	{
	  		HAL_GPIO_WritePin(LEDS_R_GPIO_Port, LEDS_R_Pin, GPIO_PIN_SET);
	  		HAL_GPIO_WritePin(LEDS_G_GPIO_Port, LEDS_G_Pin, GPIO_PIN_RESET);

	  	}
	  }
	  else
	  {
		HAL_GPIO_WritePin(LEDS_R_GPIO_Port, LEDS_R_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LEDS_G_GPIO_Port, LEDS_G_Pin, GPIO_PIN_RESET);
		sft_cnt = 0U;
	  }

	  if(temperature1 < 30.0f)
	  {
		  HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, GPIO_PIN_SET);
	  }
	  else
	  {
		  HAL_GPIO_WritePin(HEATER_GPIO_Port, HEATER_Pin, GPIO_PIN_RESET);
	  }

	  // Send ADC
	  Comm_SendBytes((uint8_t*)&adc_buf, 400);

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == HELP_BUTTON_INT_Pin)
  {
	  HAL_GPIO_WritePin(LEDS_R_GPIO_Port, LEDS_R_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(LEDS_G_GPIO_Port, LEDS_G_Pin, GPIO_PIN_SET);
	  help_button = 1;
	  sft_cnt = 5;
  }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		HAL_ADC_Start_IT(&hadc2);
	}
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(adc_idx >= 200U)
	{
		adc_idx = 0U;
	}

	adc_buf[adc_idx] = HAL_ADC_GetValue(&hadc2);
	adc_idx++;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART3)
  {
	  HAL_UART_Receive_IT(&huart3, &uart_rx, 1);
  }
}

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
