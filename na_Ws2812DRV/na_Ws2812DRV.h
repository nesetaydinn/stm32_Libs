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

#define NA_WS2812_MAX_LED_SIZE 32
#define NA_WS2812_LED_BRIGHT_COMPLEX 1
#define NA_WS2812_DELAY_TIME 40
typedef struct{
	#if NA_WS2812_LED_BRIGHT_COMPLEX
	uint32_t ledsArr[NA_WS2812_MAX_LED_SIZE][4];
	#else
	uint32_t ledsArr[NA_WS2812_MAX_LED_SIZE][3];
	#endif
	TIM_HandleTypeDef *htim;
	uint32_t htimChannel;
	uint16_t T0H;
	uint16_t T1H;
	uint8_t led_arrSize;
	uint8_t bright;
	bool dataSendFlag;
}na_Ws2812;

na_Ws2812  na_Ws2812_create(TIM_HandleTypeDef *htim,
		uint32_t Channel,
		uint8_t led_arrSize);

#if NA_WS2812_LED_BRIGHT_COMPLEX
void  na_Ws2812_putColorIndex(na_Ws2812* drv,uint8_t index,
		uint8_t r,uint8_t g,uint8_t b, uint8_t bright);
#else
void  na_Ws2812_putColorIndex(na_Ws2812* drv,uint8_t index,
		uint8_t r,uint8_t g,uint8_t b);

void na_Ws2812_setBright(na_Ws2812  *drv,uint8_t bright);
#endif
void na_Ws2812_send(na_Ws2812 *drv);
void na_Ws2812_clean_arr(na_Ws2812  *drv);
void na_Ws2812_setFlag(na_Ws2812  *drv,bool flag);
bool na_Ws2812_getFlag(na_Ws2812  *drv);
void na_Ws2812_stopDMA(na_Ws2812  *drv,TIM_HandleTypeDef *htim);



#endif /* NA_WS2812DRV_H_ */
