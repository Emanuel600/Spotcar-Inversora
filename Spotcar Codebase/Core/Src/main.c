/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HD44780.h"
#include "menu.h"
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
uint16_t ADC_Readouts[2];
pin_s data_pins[] = {{D1_Pin, D1_GPIO_Port}, {D2_Pin, D2_GPIO_Port}, {D3_Pin, D3_GPIO_Port}, {D4_Pin, D4_GPIO_Port}};
pin_s e_rs_pins[] = {{Enable_Pin, Enable_GPIO_Port}, {RS_Pin, RS_GPIO_Port}};

uint32_t triggered = 0;
static uint32_t s_current = 0;
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
  MX_DMA_Init();
  MX_ADC_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */
  ADC_Calibrate(&hadc);
  HAL_ADC_Start_DMA(&hadc, (uint32_t*)ADC_Readouts, 2);
  HD_Init(data_pins, e_rs_pins);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t last_updated = HAL_GetTick();
  uint32_t trigger_start = 0;
  uint32_t Pulse = 0;
  while (1)
  {
	  if ((HAL_GetTick() - last_updated) > 10){
		  Read_Button_State(ADC_Readouts[1]);
		  Menu_Logic_Handler();
		  // Trigger
		  if(Is_Trigger_Ready() & (triggered==0)){
		  		  ADC_Calibrate(&hadc);
		  		  trigger_start = HAL_GetTick();
		  		  Pulse = Current_Get_Compare();
		  		  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Pulse);
		  		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Pulse);
		  		  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
		  		  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
		  		  triggered = 1;
		  }
		  last_updated = HAL_GetTick();
	  }

	  // Checks if it needs to update at the start of the function
	  Menu_Update_Display();
	  // Stops trigger if gone over time
	  if(Test_Trigger_Time(HAL_GetTick() - trigger_start)){
	  		  		  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	  		  		  HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
	  		  		  triggered = 0;
	  		  }
	  // Adjusts PWM for more or less current
	   if(((HAL_GetTick() - last_updated) > 2) & (triggered==1)){
		   // Adjusts Current Proportionally to Current Error
		  int32_t adjustment = 3*((int32_t)Get_Target_Current() - (int32_t)s_current)>>12;
		  Pulse += adjustment;
		  // Clamp pulse width
		  if (Pulse > 300){
			  Pulse = 300;
		  } else if (Pulse < 9){
			  Pulse = 9;
		  }
		  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Pulse);
  		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Pulse);
	  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	 s_current = (ADC_Readouts[0]*3300*124)>>10;
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
#ifdef USE_FULL_ASSERT
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
