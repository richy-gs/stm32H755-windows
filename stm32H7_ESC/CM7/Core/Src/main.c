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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim13;
TIM_HandleTypeDef htim14;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
typedef enum {
   mov_front,
   mov_back,
   mov_left,
   mov_right
} MovementType;

typedef struct {
  MovementType movement;
  int time;
  int angle;
  int speed;
} MovementRoutine;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM13_Init(void);
/* USER CODE BEGIN PFP */
void Motor_Init(void);
void Motor_SetSpeed2(int speed, int delayTime);
void Motor_Move(MovementType direction, int delayTime, int angle);
void Motor_Stop(void);
void ExecuteMovement(MovementRoutine *movement);
void Servo_Init(void);
void Servo_SetAngle(int angle);
void routine1(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Motor_Init(void)
{
	  if (HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1) != HAL_OK)
	  {
	  	  Error_Handler();
	  }
	  __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1500);
}

void Motor_SetSpeed2(int speed, int delayTime) // Range from -100 to 100
{
	const uint32_t change = 10;
	uint32_t delay = 10;
	uint32_t x = TIM14->CCR1;

	speed *= -1;
	if(speed > 100) speed = 100;
	else if (speed < -100) speed = -100;

	int newSpeed = 1500 + (speed*5);

	while(abs(newSpeed - x) > 10)
	{
		if (newSpeed > x) x += change;
		else if (newSpeed < x) x -= change;

		TIM14->CCR1 = (uint32_t) x;
		HAL_Delay(delay);
	}
	HAL_Delay(delayTime);
}

void Motor_Move(MovementType direction, int delayTime, int angle)
{
   switch(direction)
   {
       case mov_front:
           Motor_SetSpeed2(-100,delayTime); // Ajusta la velocidad
           break;
       case mov_back:
           Motor_SetSpeed2(100,delayTime);
           break;
       case mov_left:
           Servo_SetAngle(angle);
           Motor_SetSpeed2(100,delayTime);
           break;      
       case mov_right:
           Servo_SetAngle(angle);
           Motor_SetSpeed2(100,delayTime);
           break;
       default:
    	   Motor_SetSpeed2(0,delayTime);
    	   break;
   }
}

void Motor_Stop(void)
{
   Motor_SetSpeed2(0,1000);
}

void ExecuteMovement(MovementRoutine *movement)
{
  switch (movement -> movement)
  {
  case mov_front:
	  Servo_SetAngle(movement->angle);
	  Motor_SetSpeed2(movement->speed, movement->time);
	  break;
  case mov_back:
	  Servo_SetAngle(movement->angle);
	  Motor_SetSpeed2(movement->speed, movement->time);
	  break;
  case mov_left:
	  Servo_SetAngle(movement->angle);
	  Motor_SetSpeed2(movement->speed, movement->time);
	  break;
  case mov_right:
	  Servo_SetAngle(movement->angle);
	  Motor_SetSpeed2(movement->speed, movement->time);
	  break;
  default:
    break;
  }
}

void Servo_Init(void)
{
	if (HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1) != HAL_OK)
	  {
		  Error_Handler();
	  }
	__HAL_TIM_SET_COMPARE(&htim13, TIM_CHANNEL_1, 1500);
}

// Function to set the servo position (angle)
void Servo_SetAngle(int angle)		// Angle betweeen
{
	const uint32_t change = 10;
	uint32_t delay = 10;
	uint32_t x = TIM13->CCR1;


	if(angle > 60) angle = 60;
	else if (angle < -50) angle = -50;

	int newAngle = 1500 + (angle*9);

	while(abs(newAngle - x) > 10)
	{
		if (newAngle > x) x = x + change;
		else if (newAngle < x) x = x - change;

		TIM13->CCR1 = (uint32_t) x;
		HAL_Delay(delay);
	}
	HAL_Delay(2000);
}

void routine1(void)
{
	MovementRoutine moveFrontInstance = {mov_front, 800, 10, 100};
	MovementRoutine moveRightInstance = {mov_right, 0, 50, 100};
	MovementRoutine noMoveInstance = {mov_right, 500, 5, 0};
	MovementRoutine *moveFront = &moveFrontInstance;
	MovementRoutine *moveRight = &moveRightInstance;
	MovementRoutine *noMove = &noMoveInstance;

	while(1)
	{
		ExecuteMovement(moveFront);
		ExecuteMovement(moveRight);
		HAL_Delay(1300);
		ExecuteMovement(moveFront);
		ExecuteMovement(moveRight);
		HAL_Delay(1300);
		ExecuteMovement(noMove);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_TIM14_Init();
  MX_TIM13_Init();
  /* USER CODE BEGIN 2 */
  Servo_Init();
  Motor_Init();
  HAL_Delay(100);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  routine1();
/*
	  Servo_SetAngle(60);
	  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	  Servo_SetAngle(-50);
	  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
*/
//	  Servo_SetAngle(-40);
//	  HAL_Delay(2000);


//	  Servo_SetAngle(-30);
//	  HAL_Delay(2000);
//	  Servo_SetAngle(0);
//	  HAL_Delay(2000);
//	  Motor_SetSpeed2(100,1000);
//	  Motor_SetSpeed2(-100,1000);
//	  Motor_SetSpeed2(0,1000);

/* 			PRUEBA  1			*/
//	  Turning_SetAngle(0.0);
//	  HAL_Delay(1000);

/* 			PRUEBA  2			*/
//	  Turning_SetAngle(90.0);
//	  HAL_Delay(1000);
//	  Turning_SetAngle(0.0);
//	  HAL_Delay(1000);
//	  Turning_SetAngle(-90.0);
//	  HAL_Delay(1000);
//	float setpoint = 0.5;
//	while(setpoint*100 < 101){
//		setpoint += 0.01f;
//		TIM14->CCR1 = (uint32_t)((63999*0.05)+(63999*0.05*setpoint));
//		HAL_Delay(100);
//	}
//	setpoint = 0.5;
//	HAL_Delay(2000);
//	while(setpoint*100 > -1){
//			setpoint -= 0.01f;
//			TIM14->CCR1 = (uint32_t)((63999*0.05)+(63999*0.05*setpoint));
//			HAL_Delay(100);
//	}
//
//	setpoint = 0.5f;
//	HAL_Delay(2000);
//
//	setpoint = 0.5;
//	while(setpoint*100 < 101){
//		setpoint += 0.01f;
//		TIM14->CCR1 = (uint32_t)((63999*0.05)+(63999*0.05*setpoint));
//		HAL_Delay(100);
//	}
//	while(1){}

/* 			PRUEBA  3			*/
//	  float setpoint = 0.5;
//	  while(setpoint*100 < 101){
//	  		setpoint += 0.01f;
//	  		TIM14->CCR1 = (uint32_t)((63999*0.05)+(63999*0.05*setpoint));
//	  		HAL_Delay(100);
//
//
//	  	}
//	  	setpoint = 0.5f;
//	  	HAL_Delay(2000);
//	  	while(setpoint*100 > -1){
//	  			setpoint -= 0.01f;
//	  			TIM14->CCR1 = (uint32_t)((63999*0.05)+(63999*0.05*setpoint));
//	  			HAL_Delay(100);
//
//
//	  		}
//	  	setpoint = 0.5f;
//	  	HAL_Delay(2000);



//	  float dutyCycle = 2000;
//	  float aux = dutyCycle/10;
//	  float total = 1500;
//	  while(total < dutyCycle)
//	  {
//		  TIM14->CCR1 = (uint32_t) total;
//		  HAL_Delay(100);
//		  total += aux;
//	  }
//
//	  while(1){}



//	  float change = 10;
//	  float delay = 10;
//	  float x = 1500;
//
//	  while(x >= 1100)
//	  {
//		  x -= change;
//		  TIM13->CCR1 = (uint32_t) x;
//		  TIM14->CCR1 = (uint32_t) x;
//		  HAL_Delay(delay);
//	  }
//
//	  HAL_Delay(4000);
//
//	  while(x <= 1900)
//	  {
//		  x += change;
//		  TIM13->CCR1 = (uint32_t) x;
//		  TIM14->CCR1 = (uint32_t) x;
//		  HAL_Delay(delay);
//	  }
//
//	  HAL_Delay(4000);
//
//	  while(x >= 1500)
//	  {
//		x -= change;
//		TIM13->CCR1 = (uint32_t) x;
//		TIM14->CCR1 = (uint32_t) x;
//		HAL_Delay(delay);
//	  }
//
//	  HAL_Delay(4000);



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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM13 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM13_Init(void)
{

  /* USER CODE BEGIN TIM13_Init 0 */

  /* USER CODE END TIM13_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM13_Init 1 */

  /* USER CODE END TIM13_Init 1 */
  htim13.Instance = TIM13;
  htim13.Init.Prescaler = 239;
  htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim13.Init.Period = 19999;
  htim13.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim13) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim13, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM13_Init 2 */

  /* USER CODE END TIM13_Init 2 */
  HAL_TIM_MspPostInit(&htim13);

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 239;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 19999;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */
  HAL_TIM_MspPostInit(&htim14);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC1 PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD1_Pin */
  GPIO_InitStruct.Pin = LD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PG11 PG13 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
