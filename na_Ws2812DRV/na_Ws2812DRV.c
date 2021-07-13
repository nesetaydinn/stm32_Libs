/*
 * na_Ws2812DRV.c
 *
 *  Created on: Jul 13, 2021
 *      Author: nesetaydin
 */
#include "na_Ws2812DRV.h"
uint16_t na_Ws2812_valuesMap(uint16_t inValue, uint16_t inMin, uint16_t inMax, uint16_t outMin, uint16_t outMax);

na_Ws2812 na_Ws2812_create(TIM_HandleTypeDef *htim,
		DMA_HandleTypeDef *hdma_tim,
		uint32_t Channel,
		uint8_t led_arrSize){
	na_Ws2812 tmp;
	tmp.htim =htim;
	tmp.hdma_tim=hdma_tim;
	tmp.htimChannel =Channel;
	tmp.bright=100;
	tmp.led_arrSize=led_arrSize;
	for(uint8_t c1=0;c1<MAX_LED_SIZE;c1++)
		for(uint8_t c2=0;c2<3;c2++)
			tmp.ledsArr[c1][c2]=0;

	return tmp;
}


na_Ws2812  na_Ws2812_putColorIndex(na_Ws2812 drv,uint8_t index,
		uint8_t r,uint8_t g,uint8_t b){
	//if(index<drv.led_arrSize)return drv;
	drv.ledsArr[index][0]=g;
	drv.ledsArr[index][1]=r;
	drv.ledsArr[index][2]=b;
	return drv;
}

na_Ws2812 na_Ws2812_send(na_Ws2812  drv){
	uint32_t pwmdata_X[(24*MAX_LED_SIZE)+50];
	uint32_t index=0,color;
		for(uint8_t c=0;c<drv.led_arrSize;c++){

			color=valuesMap(drv.bright,0,100,0,drv.ledsArr[c][0])<<16
					|valuesMap(drv.bright,0,100,0,drv.ledsArr[c][1])<<8
					|valuesMap(drv.bright,0,100,0,drv.ledsArr[c][2]);

			for(int8_t c=23;c>=0;c--){
				if(color&(1<<c))pwmdata_X[index]=70;
				else pwmdata_X[index]=35;
				index++;
			}
		}
		for(uint8_t c=0;c<50;c++){
			pwmdata_X[index]=0;
			index++;
		}
		HAL_TIM_PWM_Start_DMA(drv.htim,drv.htimChannel, (uint32_t *)pwmdata_X, index);
		return drv;
}
na_Ws2812 na_Ws2812_setFlag(na_Ws2812  drv,bool flag){
	drv.dataSendFlag=flag;
	return drv;
}
bool na_Ws2812_getFlag(na_Ws2812  drv){
	return drv.dataSendFlag;
}
na_Ws2812 na_Ws2812_setBright(na_Ws2812  drv,uint8_t bright){
	drv.bright=bright;
	return drv;
}
uint16_t na_Ws2812_valuesMap(uint16_t inValue, uint16_t inMin, uint16_t inMax, uint16_t outMin, uint16_t outMax) {
	return (inValue - inMin)*(outMax - outMin) / (inMax - inMin) + outMin;
}

