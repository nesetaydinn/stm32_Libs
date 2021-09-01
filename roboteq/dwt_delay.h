/*
 * dwt_delay.h
 *
 *  Created on: 20 Ağu 2021
 *      Author: nesat
 */

#ifndef DWT_DELAY_H_
#define DWT_DELAY_H_

#include <stdint.h>



#include "stm32f4xx_hal.h"

#define DWT_DELAY_NEWBIE 0

void DWT_Init(void);
void DWT_Delay(uint32_t us);

#endif /* DWT_DELAY_H_ */
