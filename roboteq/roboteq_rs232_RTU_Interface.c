/*
 * roboteq_rs232_RTU_Interface.c
 *
 *  Created on: 18 Ağu 2021
 *      Author: nesat
 */

#include "roboteq_rs232_RTU_Interface.h"
#include "dwt_delay.h"

roboteq_Modbus_RTU roboteq_Modbus_RTU_create(  UART_HandleTypeDef *huart,
		uint16_t timeout, uint8_t slaveAddress){
	roboteq_Modbus_RTU tmp;
	tmp.huart=huart;
	tmp.slaveAddress=slaveAddress;
	tmp.timeout=timeout;

	return tmp;
}


uint16_t roboteq_rs232_RTU_calcCRC(const uint8_t *p, uint16_t length) {
uint8_t hi = 0xFF;
uint8_t lo = 0xFF;
while (length--) {
uint8_t i = lo ^ *p++;
lo = hi ^ modbus_crc_hi[i];
hi = modbus_crc_lo[i];
}
return (uint16_t) (( lo << 8) | hi);
}




/*write 0x10
 * 1				1			2					2												  1
 * Node address | Func code | Register address | Number of registers to write (must be 0x00 0x02) | Number of bytes to be written (must be 0x04)
 * 	4						   2
 *  | value (4 byte msb first) | crc
 * response
 * 1				1			2			     	4				2
 * Node address | Func code | Register address | responce bytes | crc-16-ibm
 * */
bool roboteq_Modbus_RTU_write(roboteq_Modbus_RTU *drv,uint8_t funcCode,uint16_t registerAdd,
		uint8_t channel,uint32_t value){
	 	HAL_Delay(1);

	 	uint8_t responseArr[8];
	uint16_t tmp;
	uint8_t data[]={drv->slaveAddress ,funcCode ,(uint8_t)((registerAdd|channel)>>8) ,
			(uint8_t)((registerAdd|channel)&0xFF) ,0x00 ,0x02 ,0x04 ,
			(uint8_t)(value>>24) ,(uint8_t)(value>>16) ,(uint8_t)(value>>8) ,(uint8_t)(value&0xFF) ,
			0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, responseArr, sizeof(responseArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==responseArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(responseArr,sizeof(responseArr)-2);
		 	if(tmp==(uint16_t)((responseArr[6]<<8) | responseArr[7])) return 1;
	  }
		  return 0;
}

bool roboteq_Modbus_RTU_Motor_Channel1_set_speed(roboteq_Modbus_RTU *drv,int32_t value){
	HAL_Delay(1);
	 	uint8_t responseArr[8];
	uint16_t tmp;
	uint8_t data[]={drv->slaveAddress ,WRITE_MULTIPLE_HOLDING_REGISTER ,(uint8_t)((COMMAND_CG|MOTOR_CHANNEL_1)>>8) ,
			(uint8_t)((COMMAND_CG|MOTOR_CHANNEL_1)&0xFF) ,0x00 ,0x02 ,0x04 ,
			(uint8_t)(value>>24) ,(uint8_t)(value>>16) ,(uint8_t)(value>>8) ,(uint8_t)(value&0xFF) ,
			0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, responseArr, sizeof(responseArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==responseArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(responseArr,sizeof(responseArr)-2);
		 	if(tmp==(uint16_t)((responseArr[6]<<8) | responseArr[7])) return 1;
	  }
		  return 0;
}

bool roboteq_Modbus_RTU_Motor_Channel2_set_speed(roboteq_Modbus_RTU *drv,int32_t value){
	HAL_Delay(1);
	 	uint8_t responseArr[8];
	uint16_t tmp;
	uint8_t data[]={drv->slaveAddress ,WRITE_MULTIPLE_HOLDING_REGISTER ,(uint8_t)((COMMAND_CG|MOTOR_CHANNEL_2)>>8) ,
			(uint8_t)((COMMAND_CG|MOTOR_CHANNEL_2)&0xFF) ,0x00 ,0x02 ,0x04 ,
			(uint8_t)(value>>24) ,(uint8_t)(value>>16) ,(uint8_t)(value>>8) ,(uint8_t)(value&0xFF) ,
			0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, responseArr, sizeof(responseArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==responseArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(responseArr,sizeof(responseArr)-2);
		 	if(tmp==(uint16_t)((responseArr[6]<<8) | responseArr[7])) return 1;
	  }
		  return 0;
}

bool roboteq_Modbus_RTU_Motor_Channel1_set_Encoder(roboteq_Modbus_RTU *drv,int32_t value){
	HAL_Delay(1);
	 	uint8_t responseArr[8];
	uint16_t tmp;
	uint8_t data[]={drv->slaveAddress ,WRITE_MULTIPLE_HOLDING_REGISTER ,(uint8_t)((COMMAND_C|MOTOR_CHANNEL_1)>>8) ,
			(uint8_t)((COMMAND_C|MOTOR_CHANNEL_1)&0xFF) ,0x00 ,0x02 ,0x04 ,
			(uint8_t)(value>>24) ,(uint8_t)(value>>16) ,(uint8_t)(value>>8) ,(uint8_t)(value&0xFF) ,
			0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, responseArr, sizeof(responseArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==responseArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(responseArr,sizeof(responseArr)-2);
		 	if(tmp==(uint16_t)((responseArr[6]<<8) | responseArr[7])) return 1;
	  }
		  return 0;
}

bool roboteq_Modbus_RTU_Motor_Channel2_set_Encoder(roboteq_Modbus_RTU *drv,int32_t value){
	HAL_Delay(1);
	 	uint8_t responseArr[8];
	uint16_t tmp;
	uint8_t data[]={drv->slaveAddress ,WRITE_MULTIPLE_HOLDING_REGISTER ,(uint8_t)((COMMAND_C|MOTOR_CHANNEL_2)>>8) ,
			(uint8_t)((COMMAND_C|MOTOR_CHANNEL_2)&0xFF) ,0x00 ,0x02 ,0x04 ,
			(uint8_t)(value>>24) ,(uint8_t)(value>>16) ,(uint8_t)(value>>8) ,(uint8_t)(value&0xFF) ,
			0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, responseArr, sizeof(responseArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==responseArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(responseArr,sizeof(responseArr)-2);
		 	if(tmp==(uint16_t)((responseArr[6]<<8) | responseArr[7])) return 1;
	  }
		  return 0;
}

/*read 0x04
 * 1				1			2						 2				  2
 * Node address | Func code |  register adress (2 byte) | 0x00 0x02 | crc-16-ibm
 *
 * response
 * 1				1			1									4				2
 * Node address | Func code | total bytes read (always 4 byte) | responce bytes | crc-16-ibm
 * */

uint32_t roboteq_Modbus_RTU_read(roboteq_Modbus_RTU *drv,uint8_t funcCode,uint16_t registerAdd,
		uint8_t channel){
 	HAL_Delay(1);
	uint16_t tmp;
	static uint8_t respArr[9];
	uint8_t data[]={drv->slaveAddress ,funcCode ,(uint8_t)((registerAdd|channel)>>8) ,
			(uint8_t)((registerAdd|channel)&0xFF) ,0x00 ,0x02 , 0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, respArr, sizeof(respArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==respArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(respArr,sizeof(respArr)-2);
		 	if(tmp==(uint16_t)((respArr[7]<<8) | respArr[8])){
		 		uint32_t tmp= (uint32_t)(respArr[3]<<24) | (respArr[4]<<16) | (respArr[5]<<8) | respArr[6];
		 		return tmp;
		 	}
	  }
		  return -1111;
}

int32_t roboteq_Modbus_RTU_Motor_Channel1_get_Encoder(roboteq_Modbus_RTU *drv){
 	HAL_Delay(1);
	uint16_t tmp;
	static uint8_t respArr[9];
	uint8_t data[]={drv->slaveAddress ,READ_INPUT_REGISTER ,(uint8_t)((QUERY_C|MOTOR_CHANNEL_1)>>8) ,
			(uint8_t)((QUERY_C|MOTOR_CHANNEL_1)&0xFF) ,0x00 ,0x02 , 0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, respArr, sizeof(respArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==respArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(respArr,sizeof(respArr)-2);
		 	if(tmp==(uint16_t)((respArr[7]<<8) | respArr[8])){
		 		uint32_t tmp= (uint32_t)(respArr[3]<<24) | (respArr[4]<<16) | (respArr[5]<<8) | respArr[6];
		 		return tmp;
		 	}
	  }
		  return -1111;
}

int32_t roboteq_Modbus_RTU_Motor_Channel2_get_Encoder(roboteq_Modbus_RTU *drv){
 	HAL_Delay(1);
	uint16_t tmp;
	static uint8_t respArr[9];
	uint8_t data[]={drv->slaveAddress ,READ_INPUT_REGISTER ,(uint8_t)((QUERY_C|MOTOR_CHANNEL_2)>>8) ,
			(uint8_t)((QUERY_C|MOTOR_CHANNEL_2)&0xFF) ,0x00 ,0x02 , 0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, respArr, sizeof(respArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==respArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(respArr,sizeof(respArr)-2);
		 	if(tmp==(uint16_t)((respArr[7]<<8) | respArr[8])){
		 		uint32_t tmp= (uint32_t)(respArr[3]<<24) | (respArr[4]<<16) | (respArr[5]<<8) | respArr[6];
		 		return tmp;
		 	}
	  }
		  return -1111;
}

//EX - Emergency Stop  address 0x0180
bool roboteq_Modbus_RTU_Motor_Channel2_EmergencyStop(roboteq_Modbus_RTU *drv){
	HAL_Delay(1);
	 	uint8_t responseArr[8];
	uint16_t tmp;
	uint8_t data[]={drv->slaveAddress ,WRITE_MULTIPLE_HOLDING_REGISTER ,(uint8_t)((COMMAND_EX)>>8) ,
			(uint8_t)((COMMAND_EX)&0xFF) ,0x00 ,0x02 ,0x04 ,
			0 ,0 ,0 ,0 ,
			0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, responseArr, sizeof(responseArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==responseArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(responseArr,sizeof(responseArr)-2);
		 	if(tmp==(uint16_t)((responseArr[6]<<8) | responseArr[7])) return 1;
	  }
		  return 0;
}
//MG - Emergency Stop Release  address 0x01A0
bool roboteq_Modbus_RTU_Motor_Channel2_EmergencyStopRelease(roboteq_Modbus_RTU *drv){
	HAL_Delay(1);
	 	uint8_t responseArr[8];
	uint16_t tmp;
	uint8_t data[]={drv->slaveAddress ,WRITE_MULTIPLE_HOLDING_REGISTER ,(uint8_t)((COMMAND_MG)>>8) ,
			(uint8_t)((COMMAND_MG)&0xFF) ,0x00 ,0x02 ,0x04 ,
			0 ,0 ,0 ,0 ,
			0x00 ,0x00};
			tmp=roboteq_rs232_RTU_calcCRC(data,sizeof(data)-2);
			data[sizeof(data)-2]=(uint8_t)(tmp>>8);
			data[sizeof(data)-1]=(uint8_t)(tmp&0xFF);
	  drv->huart->Lock=HAL_UNLOCKED;
	  HAL_UART_Transmit(drv->huart, data, sizeof(data),drv->timeout);
	  HAL_UART_Receive_DMA(drv->huart, responseArr, sizeof(responseArr));
	  drv->huart->Lock=HAL_LOCKED;
	  if(0x01==responseArr[0]){
		   tmp=roboteq_rs232_RTU_calcCRC(responseArr,sizeof(responseArr)-2);
		 	if(tmp==(uint16_t)((responseArr[6]<<8) | responseArr[7])) return 1;
	  }
		  return 0;
}
