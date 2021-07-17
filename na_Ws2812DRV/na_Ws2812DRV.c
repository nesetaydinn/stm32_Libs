/*
 * na_Ws2812DRV.c
 *
 *  Created on: Jul 13, 2021
 *      Author: nesetaydin
 */
#include "na_Ws2812DRV.h"


uint16_t na_Ws2812_valuesMap(uint16_t inValue, uint16_t inMin, uint16_t inMax, uint16_t outMin, uint16_t outMax);

na_Ws2812 na_Ws2812_create(TIM_HandleTypeDef *htim,
		uint32_t Channel,
		uint8_t led_arrSize){
	na_Ws2812 tmp;
	htim->Instance->CCR1=0;
	htim->Instance->CCR2=0;
	htim->Instance->CCR3=0;
	htim->Instance->CCR4=0;
	tmp.htim =htim;
	tmp.htimChannel =Channel;
	#if NA_WS2812_LED_BRIGHT_COMPLEX
	for(uint8_t c1=0;c1<NA_WS2812_MAX_LED_SIZE;c1++)
		for(uint8_t c2=0;c2<4;c2++)
			tmp.ledsArr[c1][c2]=0;
	#else
	tmp.bright=100;
	for(uint8_t c1=0;c1<NA_WS2812_MAX_LED_SIZE;c1++)
		for(uint8_t c2=0;c2<3;c2++)
			tmp.ledsArr[c1][c2]=0;
	#endif
	tmp.led_arrSize=led_arrSize;
	tmp.T1H=(uint16_t)((htim->Instance->ARR+1)*2/3);
	tmp.T0H=(uint16_t)((htim->Instance->ARR+1)*1/3);


	return tmp;
}

#if NA_WS2812_LED_BRIGHT_COMPLEX
void  na_Ws2812_putColorIndex(na_Ws2812* drv,uint8_t index,
		uint8_t r,uint8_t g,uint8_t b, uint8_t bright){
	//if(index<drv.led_arrSize)return drv;
	if(bright>100 || r>255 || g>255 || b>255)return;
	drv->ledsArr[index][0]=bright;
	drv->ledsArr[index][1]=g;
	drv->ledsArr[index][2]=r;
	drv->ledsArr[index][3]=b;

}
#else

void  na_Ws2812_putColorIndex(na_Ws2812* drv,uint8_t index,
		uint8_t r,uint8_t g,uint8_t b){
	//if(index<drv.led_arrSize)return drv;
	drv->ledsArr[index][0]=g;
	drv->ledsArr[index][1]=r;
	drv->ledsArr[index][2]=b;

}
void na_Ws2812_setBright(na_Ws2812 * drv,uint8_t bright){
	drv->bright=bright;
}
#endif

void na_Ws2812_clean_arr(na_Ws2812  *drv){
#if NA_WS2812_LED_BRIGHT_COMPLEX
	for(uint8_t c1=0;c1<drv->led_arrSize;c1++)
		for(uint8_t c2=0;c2<4;c2++)
			drv->ledsArr[c1][c2]=0;
	#else
	tmp.bright=100;
	for(uint8_t c1=0;c1<drv->led_arrSize;c1++)
		for(uint8_t c2=0;c2<3;c2++)
			drv->ledsArr[c1][c2]=0;
	#endif
}

uint32_t pwmdata_X[(24*NA_WS2812_MAX_LED_SIZE)+NA_WS2812_DELAY_TIME];


void na_Ws2812_send(na_Ws2812  *drv){
	uint32_t index=0,color;
		for(uint8_t c=0;c<drv->led_arrSize;c++){

		#if NA_WS2812_LED_BRIGHT_COMPLEX
			color=na_Ws2812_valuesMap(drv->ledsArr[c][0],0,100,0,drv->ledsArr[c][1])<<16
					|na_Ws2812_valuesMap(drv->ledsArr[c][0],0,100,0,drv->ledsArr[c][2])<<8
					|na_Ws2812_valuesMap(drv->ledsArr[c][0],0,100,0,drv->ledsArr[c][3]);
		#else
			color=na_Ws2812_valuesMap(drv->bright,0,100,0,drv->ledsArr[c][0])<<16
					|na_Ws2812_valuesMap(drv->bright,0,100,0,drv->ledsArr[c][1])<<8
					|na_Ws2812_valuesMap(drv->bright,0,100,0,drv->ledsArr[c][2]);
		#endif


			for(int8_t c=23;c>=0;c--){
				if(color&(1<<c))pwmdata_X[index]=drv->T1H;
				else pwmdata_X[index]=drv->T0H;
				index++;
			}
		}
		for(uint8_t c=0;c<NA_WS2812_DELAY_TIME;c++){
			pwmdata_X[index]=0;
			index++;
		}
		HAL_TIM_PWM_Start_DMA(drv->htim,drv->htimChannel, (uint32_t *)pwmdata_X, index);
		while(!drv->dataSendFlag);
		drv->dataSendFlag=false;
}
void na_Ws2812_setFlag(na_Ws2812 * drv,bool flag){
	drv->dataSendFlag=flag;
}
bool na_Ws2812_getFlag(na_Ws2812  *drv){
	return drv->dataSendFlag;
}
void na_Ws2812_stopDMA(na_Ws2812  *drv,TIM_HandleTypeDef *htim){
	if(htim->Instance == drv->htim->Instance){
		HAL_TIM_PWM_Stop_DMA(drv->htim, drv->htimChannel);
		drv->dataSendFlag=true;
	}

}
uint16_t na_Ws2812_valuesMap(uint16_t inValue, uint16_t inMin, uint16_t inMax, uint16_t outMin, uint16_t outMax) {
	return (inValue - inMin)*(outMax - outMin) / (inMax - inMin) + outMin;
}
