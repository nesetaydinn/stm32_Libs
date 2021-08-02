/*
 * na_Ina226DRV.cpp
 *
 *  Created on: 30 Jul 2021
 *      Author: nesetaydin
 */


#include "na_Ina226DRV.h"

na_Ina2256 na_Ina226_drv_create( I2C_HandleTypeDef *hi2c,uint8_t address,uint8_t ratebits,
		uint8_t vshctbits,uint8_t vbusctbits,uint8_t modebits,
		uint16_t mask_enable_bits,uint16_t timeout,double rSunt){
	na_Ina2256 tmp;
	tmp.hi2c=hi2c;
	tmp.address=(uint8_t)(address<<1);
	tmp.timeout=timeout;
	tmp.Rshunt=rSunt;
	tmp.conf_reg.RST_BITS=0;
	tmp.conf_reg.AVG_RATE_BITS=ratebits;
	tmp.conf_reg.VSHCT_BITS=vshctbits;
	tmp.conf_reg.VBUSCT_BITS=vbusctbits;
	tmp.conf_reg.MODE_BITS=modebits;
	tmp.enable_reg.SOL_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_SOL_BIT);
	tmp.enable_reg.SUL_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_SUL_BIT);
	tmp.enable_reg.BOL_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_BOL_BIT);
	tmp.enable_reg.BUL_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_BUL_BIT);
	tmp.enable_reg.POL_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_POL_BIT);
	tmp.enable_reg.CNVR_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_CNVR_BIT);
	tmp.enable_reg.APOL_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_APOL_BIT);
	tmp.enable_reg.LEN_BIT=(mask_enable_bits & NA_INA226_MASK_ENABLE_REG_LEN_BIT);
	return tmp;
}

bool na_Ina226_drv_SetConfig(na_Ina2256 *drv){
	uint8_t data[2];
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
 	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
		uint16_t tmp = (drv->conf_reg.RST_BITS<<15)|(drv->conf_reg.AVG_RATE_BITS<<9)
				|(drv->conf_reg.VBUSCT_BITS<<6)|(drv->conf_reg.VSHCT_BITS<<3)|(drv->conf_reg.MODE_BITS);
	    data[0]=(tmp>>8);
	    data[1]=(tmp&0xFF);
		HAL_I2C_Mem_Write(drv->hi2c, drv->address, NA_INA226_CONFIG_REG, 1,data, 2, drv->timeout);
		return true;
	 }return false;
}
bool na_Ina226_drv_SetCalib(na_Ina2256 *drv){
	uint8_t data[2];
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
	    volatile double Imax =(81.92e-3)/drv->Rshunt;
	    volatile double current_LSB= Imax/32768;
	    volatile double power_LSB=current_LSB * 25;
	    uint16_t cal_value= (uint16_t)((0.00512)/(current_LSB*drv->Rshunt));
	    data[0]=(cal_value>>8);
	    data[1]=(cal_value&0xFF);
	    HAL_I2C_Mem_Write(drv->hi2c, drv->address, NA_INA226_CALIBRATION_REG, 1,data, 2, drv->timeout);
		return true;
	 }return false;
}

bool na_Ina226_drv_SetShuntVoltageLimit(na_Ina2256 *drv){
	uint8_t data[2];
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
	    volatile double Imax =81.92/drv->Rshunt;
	    volatile double Voltage =Imax*drv->Rshunt;
	    uint16_t cal_value= (uint16_t)(Voltage/2.5e-6);
	    data[0]=(cal_value>>8);
	    data[1]=(cal_value&0xFF);
	    HAL_I2C_Mem_Write(drv->hi2c, drv->address, NA_INA226_ALERT_LIMIT_REG, 1,data, 2, drv->timeout);
		return true;
	 }return false;
}


double na_Ina226_drv_GetShuntVoltage(na_Ina2256 *drv){
	uint8_t data[2];
	uint16_t tmp=0;
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){

	    HAL_I2C_Mem_Read(drv->hi2c, drv->address, NA_INA226_SHUNT_VOLTAGE_REG, 1,data, 2, drv->timeout);
	    tmp=(data[0]<<8)|data[1];
	    return (tmp*2.5e-6);
	 }return (-1.0);
}

double na_Ina226_drv_GetBusVoltage(na_Ina2256 *drv){
	uint8_t data[2];
	uint16_t tmp=0;
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
	    HAL_I2C_Mem_Read(drv->hi2c, drv->address, NA_INA226_BUS_VOLTAGE_REG, 1,data, 2, drv->timeout);
	    tmp=(data[0]<<8)|data[1];
	    return (tmp*1.25e-3);
	 }return (-1.0);
}

double na_Ina226_drv_GetTotalVoltage(na_Ina2256 *drv){
	return na_Ina226_drv_GetBusVoltage(drv)+na_Ina226_drv_GetShuntVoltage(drv);
}

double na_Ina226_drv_GetCurrent_mA(na_Ina2256 *drv){
	uint8_t data[2];
	int16_t tmp=0;
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
	    HAL_I2C_Mem_Read(drv->hi2c, drv->address, NA_INA226_CURRENT_REG, 1,data, 2, drv->timeout);
	    volatile double Imax =81.92/drv->Rshunt;
	    volatile double current_LSB=Imax/32768;
	    tmp=(data[0]<<8)|data[1];
	    return (tmp*current_LSB);
	 }return (-1.0);
}
double na_Ina226_drv_GetCurrent_A(na_Ina2256 *drv){
	return (na_Ina226_drv_GetCurrent_mA(drv)/1000);
}
double na_Ina226_drv_GetPower_mW(na_Ina2256 *drv){
	uint8_t data[2];
	int16_t tmp=0;
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
	    HAL_I2C_Mem_Read(drv->hi2c, drv->address, NA_INA226_POWER_REG, 1,data, 2, drv->timeout);
	    volatile double Imax =81.92/drv->Rshunt;
	    volatile double current_LSB=Imax/32768;
	    volatile double power_LSB=current_LSB*25;
	    tmp=(data[0]<<8)|data[1];
	    return (tmp*power_LSB);
	 }return (-1.0);
}
double na_Ina226_drv_GetPower_W(na_Ina2256 *drv){
	return (na_Ina226_drv_GetPower_mW(drv)/1000);
}
bool na_Ina226_drv_GetMask_Enable_RegVal(na_Ina2256 *drv){
	uint8_t data[2];
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
	    data[0]=0x00;
	    data[1]=0x00;
	    HAL_I2C_Mem_Read(drv->hi2c, drv->address, NA_INA226_MASK_ENABLE_REG, 1,data, 2, drv->timeout);
	    uint16_t tmp=(data[0]<<8)|data[1];
		drv->enable_reg.SOL_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_SOL_BIT);
		drv->enable_reg.SUL_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_SUL_BIT);
		drv->enable_reg.BOL_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_BOL_BIT);
		drv->enable_reg.BUL_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_BUL_BIT);
		drv->enable_reg.POL_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_POL_BIT);
		drv->enable_reg.CNVR_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_CNVR_BIT);
		drv->enable_reg.APOL_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_APOL_BIT);
		drv->enable_reg.LEN_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_LEN_BIT);
		drv->enable_reg.LEN_BIT=(tmp & NA_INA226_MASK_ENABLE_REG_LEN_BIT);
		drv->enable_reg.AFF_BIT=(tmp & 0x10);
		drv->enable_reg.CVRF_BIT=(tmp & 0x08);
		drv->enable_reg.OVF_BIT=(tmp & 0x04);
	    return true;
	 }return false;
}
bool na_Ina226_drv_GetDie_RegVal(na_Ina2256 *drv){
	uint8_t data[2];
	HAL_I2C_Mem_Read (drv->hi2c, drv->address,NA_INA226_MANUFACTURER_ID_REG,1, data, sizeof(data), drv->timeout);
	if(0x5449==(uint16_t)((data[0]<<8)|data[1])){
	    data[0]=0x00;
	    data[1]=0x00;
	    HAL_I2C_Mem_Read(drv->hi2c, drv->address, NA_INA226_MASK_ENABLE_REG, 1,data, 2, drv->timeout);
	    uint16_t tmp=(data[0]<<8)|data[1];
	    drv->die_Id_reg.RID_BITS = (tmp & 0x0F);
	    drv->die_Id_reg.RID_BITS = (tmp & 0xFFF0);
	    return true;
	 }return false;
}


