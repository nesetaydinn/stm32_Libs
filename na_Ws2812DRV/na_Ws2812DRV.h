/*
 * na_Ws2812DRV.h
 *
 *  Created on: Jul 13, 2021
 *      Author: nesetaydin
 */

#ifndef NA_WS2812DRV_H_
#define NA_WS2812DRV_H_

#include "main.h"
#include "stdint.h"
#include "stdbool.h"

#define MAX_LED_SIZE 16

typedef struct{
	uint32_t ledsArr[MAX_LED_SIZE][3];
	uint8_t led_arrSize;
	uint8_t bright;
	bool dataSendFlag;
	uint32_t htimChannel;

	TIM_HandleTypeDef *htim;
	DMA_HandleTypeDef *hdma_tim;
}na_Ws2812;

na_Ws2812  na_Ws2812_create(TIM_HandleTypeDef *htim,
		DMA_HandleTypeDef *hdma_tim,
		uint32_t Channel,
		uint8_t led_arrSize);
na_Ws2812 na_Ws2812_putColorIndex(na_Ws2812  drv,uint8_t index,
		uint8_t r,uint8_t g,uint8_t b);
na_Ws2812 na_Ws2812_send(na_Ws2812  drv);
na_Ws2812 na_Ws2812_setFlag(na_Ws2812  drv,bool flag);
na_Ws2812 na_Ws2812_setBright(na_Ws2812  drv,uint8_t bright);
bool na_Ws2812_getFlag(na_Ws2812  drv);

#endif /* NA_WS2812DRV_H_ */
