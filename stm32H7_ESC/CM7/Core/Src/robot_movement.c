///*
//* robot_movement.c
//*
//*  Created on: Nov 12, 2024
//*      Author: jesus
//*/
//
//#include "robot_movement.h"
//#include "motor_control.h"  // Se asume que tienes una biblioteca para controlar los motores
//
//// Función para inicializar el sistema de control de motores
//void Robot_Init(void)
//{
//   // Inicializar los motores o cualquier hardware necesario
//   Motor_Init();  // Suposición: la función Motor_Init inicializa los motores
//}
//
//// Función para mover el robot en la dirección especificada
//void Robot_Move(Direction direction)
//{
//   switch(direction)
//   {
//       case FORWARD:
//           // Activar los motores para avanzar
//           Motor_SetSpeed(100, 100);  // Suposición: Motor_SetSpeed controla la velocidad de los motores (valores de 0 a 100)
//           Motor_Move(FORWARD);       // Mueve el robot hacia adelante
//           break;
//
//       case BACKWARD:
//           // Activar los motores para retroceder
//           Motor_SetSpeed(100, 100);
//           Motor_Move(BACKWARD);      // Mueve el robot hacia atrás
//           break;
//
//       case LEFT:
//           // Activar los motores para girar a la izquierda
//           Motor_SetSpeed(50, 100);   // Ejemplo de velocidad desigual para hacer un giro
//           Motor_Move(LEFT);          // Mueve el robot hacia la izquierda
//           break;
//
//       case RIGHT:
//           // Activar los motores para girar a la derecha
//           Motor_SetSpeed(100, 50);   // Ejemplo de velocidad desigual para hacer un giro
//           Motor_Move(RIGHT);         // Mueve el robot hacia la derecha
//           break;
//
//       default:
//           // Si no se especifica una dirección válida, detener el robot
//           Robot_Stop();
//           break;
//   }
//}
//
//// Función para detener el robot
//void Robot_Stop(void)
//{
//   Motor_Stop();  // Detiene ambos motores
//}
//
