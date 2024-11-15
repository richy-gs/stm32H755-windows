/*
* motor_control.c
*
*  Created on: Nov 12, 2024
*      Author: jesus
*/
#include "motor_control.h"
#include "stm32h7xx_hal.h" // Suponiendo que uses STM32 o una plataforma similar


// Definir los pines de los motores
//#define MOTOR_LEFT_PIN_FORWARD   GPIO_PIN_0
//#define MOTOR_LEFT_PIN_BACKWARD  GPIO_PIN_1
//#define MOTOR_RIGHT_PIN_FORWARD  GPIO_PIN_2
//#define MOTOR_RIGHT_PIN_BACKWARD GPIO_PIN_3

//void Motor_Init(void)
//{
//	  if (HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1) != HAL_OK)
//	  {
//	  	  Error_Handler();
//	  }
//	  __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 1500);
//}
//
//void Motor_SetSpeed(int speed) // Range from -100 to 100
//{
//	uint32_t change = 10;
//	float delay = 10;
//	uint32_t x = TIM14->CRR1;
//
//	if(speed > 100) speed = 100;
//	else if (speed < -100) speed = -100;
//
//	int newSpeed = 1500 + (speed*4);
//
//	if (newSpeed - x > 0)
//	{
//		while(x < newSpeed)
//		{
//			x += change;
//			TIM14->CCR1 = (uint32_t) x;
//			HAL_Delay(delay);
//		}
//	} else if (newSpeed - x < 0)
//	{
//		while(x > newSpeed)
//			{
//				x -= change;
//				TIM14->CCR1 = (uint32_t) x;
//				HAL_Delay(delay);
//			}
//	} else {
//		TIM14->CCR1 = (uint32_t) x;
//	}
//}
//
//void Motor_Move(MotorDirection direction)
//{
//   switch(direction)
//   {
//       case FORWARD:
//           Motor_SetSpeed(100); // Ajusta la velocidad
//           break;
//       case BACKWARD:
//           Motor_SetSpeed(-100);
//           break;
//       case LEFT:
//           Motor_SetSpeed(50);
//           break;
//       case RIGHT:
//           Motor_SetSpeed(-50);
//           break;
//   }
//}
//
//void Motor_Stop(void)
//{
//   Motor_SetSpeed(0);
//}

