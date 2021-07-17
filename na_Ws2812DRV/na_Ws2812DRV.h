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

#define MAX_LED_SIZE 32

typedef struct{
	uint32_t ledsArr[MAX_LED_SIZE][3];
	uint8_t led_arrSize;
	uint8_t bright;
	bool dataSendFlag;
	uint32_t htimChannel;
	uint16_t T0H;
	uint16_t T1H;

	TIM_HandleTypeDef *htim;
}na_Ws2812;

na_Ws2812  na_Ws2812_create(TIM_HandleTypeDef *htim,
		uint32_t Channel,
		uint8_t led_arrSize);
void na_Ws2812_putColorIndex(na_Ws2812  *drv,uint8_t index,
		uint8_t r,uint8_t g,uint8_t b);
void na_Ws2812_send(na_Ws2812 *drv);
void na_Ws2812_setFlag(na_Ws2812  *drv,bool flag);
void na_Ws2812_setBright(na_Ws2812  *drv,uint8_t bright);
bool na_Ws2812_getFlag(na_Ws2812  *drv);

#endif /* NA_WS2812DRV_H_ */
