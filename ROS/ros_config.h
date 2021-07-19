/*
 * ros_config.h
 *
 *  Created on: Jul 18, 2021
 *      Author: nesetaydin
 */

#ifndef ROS_CONFIG_H_
#define ROS_CONFIG_H_



#define STM32F4xx  // Change for your device
#ifdef STM32F3xx
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_uart.h"
#endif /* STM32F3xx */
#ifdef STM32F4xx
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#endif /* STM32F4xx */
#ifdef STM32F7xx
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_uart.h"
#endif /* STM32F7xx */


#define ROS_UART_CHANNEL huart2
extern UART_HandleTypeDef ROS_UART_CHANNEL;


#endif /* ROS_CONFIG_H_ */
