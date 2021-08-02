/*
 * na_Ina226DRV.h
 *
 *  Created on: 30 Jul 2021
 *      Author: nesetaydin
 */

#ifndef NA_INA226DRV_H_
#define NA_INA226DRV_H_

#include "main.h"
#include "stdint.h"
#include "stdbool.h"


//package Slave adress byte (7 bit one bit shifting left) - Register Pointer Byte - Data MSB byte - Data LSB byte
#define NA_INA226_CONFIG_REG             0x00 //r/w
#define NA_INA226_SHUNT_VOLTAGE_REG      0x01 //r
#define NA_INA226_BUS_VOLTAGE_REG        0x02 //r
#define NA_INA226_POWER_REG              0x03 //r
#define NA_INA226_CURRENT_REG            0x04 //r
#define NA_INA226_CALIBRATION_REG        0x05 //r/w
#define NA_INA226_MASK_ENABLE_REG        0x06 //r/w
#define NA_INA226_ALERT_LIMIT_REG        0x07 //r/w
#define NA_INA226_MANUFACTURER_ID_REG    0xFE //r
#define NA_INA226_DIE_ID_REG   			 0xFF //r



/*
 * DESC: This register controls the
 * conversion time settings for both the shunt and bus voltage measurements as well as the averaging mode used.
 * The operating mode that controls what signals are selected to be measured is also programmed in the
 * Configuration Register.
 * D15 D14 D13 D12  D11  D10  D9     D8      D7      D6     D5     D4     D3    D2    D1    D0
 * rst  -   -   -  AVG2 AVG1 AVG0 VBUSCT2 VBUSCT1 VBUSCT0 VSHCT2 VSHCT1 VSHCT0 MODE3 MODE2 MODE1
 *
 * D15: rst bit clear all
 * D11-D10-D9: Averaging mode (AVGx) using for software filter: determine numbers of samples and get averaged.
 *  0 0 0 -> 1 (default)
 *  0 0 1 -> 4
 *  0 1 0 -> 16
 *  0 1 1 -> 64
 *  1 0 0 -> 128
 *  1 0 1 -> 256
 *  1 1 0 -> 512
 *  1 1 1 -> 1024
 *
 *  D8-D7-D6: Bus voltage conversion time(VBUSCTx) using for sets the conversion time for the bus voltage measurement.
 *  0 0 0 -> 140us
 *  0 0 1 -> 204us
 *  0 1 0 -> 332us
 *  0 1 1 -> 588us
 *  1 0 0 -> 1.1ms (default)
 *  1 0 1 -> 2.116ms
 *  1 1 0 -> 4.156ms
 *  1 1 1 -> 8.244ms
 *  D5-D4-D3: Shunt Voltage Conversion Time(VSHCTx) using for sets the conversion time for the shunt voltage measurement.
 *  0 0 0 -> 140us
 *  0 0 1 -> 204us
 *  0 1 0 -> 332us
 *  0 1 1 -> 588us
 *  1 0 0 -> 1.1ms (default)
 *  1 0 1 -> 2.116ms
 *  1 1 0 -> 4.156ms
 *  1 1 1 -> 8.244ms
 *  D2-D1-D0: Operating Mode(MODEx) using for  Selects continuous,
 *  triggered, or power-down mode of operation. These bits default to continuous shunt and bus measurement mode.
 *  0 0 0 -> Shutdown
 *  0 0 1 -> Shunt Voltage(Triggered)
 *  0 1 0 -> Bus Voltage(Triggered)
 *  0 1 1 -> Shunt and Bus(Triggered)
 *  1 0 0 -> Shutdown
 *  1 0 1 -> Shunt Voltage(Continuous)
 *  1 1 0 -> Bus Voltage(Continuous)
 *  1 1 1 -> Shunt and Bus(Continuous) (default)
 * */
/* DESC: The Shunt Voltage Register stores the current shunt voltage reading, VSHUNT.
 * Negative numbers are represented in two's complement format. Generate the two's complement of
 * a negative number by complementing the absolute value binary number and adding 1. An MSB = '1'
 * denotes a negative number.
 * Ex: : For a value of VSHUNT = –80 mV:
 * Take the absolute value: 80 mV
 * Translate this number to a whole decimal number (80 mV ÷ 2.5 µV) = 32000
 * Convert this number to binary = 0111 1101 0000 0000
 * Complement the binary result = 1000 0010 1111 1111
 * Add '1' to the complement to create the two's complement result = 1000 0011 0000 0000 = 8300h
 * Note: If averaging is enabled, this register displays the averaged value.
 * Full-scale range = 81.92 mV (decimal = 7FFF); LSB: 2.5 μV.
 * D15   D14 D13 D12  D11  D10  D9     D8      D7      D6     D5     D4     D3    D2    D1  D0
 * SIGN (MSB                                                                                LSB)
 * */
/* DESC: The Bus Voltage Register stores the most recent bus voltage reading, VBUS.
 * Note: If averaging is enabled, this register displays the averaged value.
 * Full-scale range = 40.96 V (decimal = 7FFF); LSB = 1.25 mV.
 * D15   D14 D13 D12  D11  D10  D9     D8      D7      D6     D5     D4     D3    D2    D1  D0
 *  -    (MSB                                                                              LSB)
 * */
/* DESC: The Power Register records power in Watts by multiplying the decimal values of the Current Register with the
 * decimal value of the Bus Voltage Register according to Equation 4. (Power=(Current * Bus Voltage)/20000)
 * Note: If averaging is enabled, this register displays the averaged value.
 * The Power Register LSB is internally programmed to equal 25 times the programmed value of the Current_LSB
 * PowerLsb=25*CurrentLsb
 * D15   D14 D13 D12  D11  D10  D9     D8      D7      D6     D5     D4     D3    D2    D1  D0
 * (MSB                                                                                    LSB)
 * */
/* DESC: The value of the Current Register is calculated by multiplying the decimal value
 * in the Shunt Voltage Register with the decimal value of the Calibration Register,
 * according to Equation 3. (Current=(Shunt Voltage * Calibration Register)/2048)
 * Note: If averaging is enabled, this register displays the averaged value.
 * D15   D14 D13 D12  D11  D10  D9     D8      D7      D6     D5     D4     D3    D2    D1  D0
 * SIGN  (MSB                                                                               LSB)
 * */
/* DESC: This register provides the device with the value of the shunt resistor that
 * was present to create the measured differential voltage.
 * It also sets the resolution of the Current Register.
 * Programming this register sets the Current_LSB and the Power_LSB.
 * This register is also suitable for use in overall system calibration.
 *
 * Current_LSB=(Maximum Expected Current/2^15)
 * CAL=0.00512/(Current_LSB*Rshunt) //Write calibration register return must be uint
 * D15   D14 D13 D12  D11  D10  D9     D8      D7      D6     D5     D4     D3    D2    D1  D0
 *  -    (MSB                                                                               LSB)
 * */
/* DESC: The Mask/Enable Register selects the function that is enabled
 * to control the Alert pin as well as how that pin
 * functions. If multiple functions are enabled, the highest significant
 * bit position Alert Function (D15-D11) takes
 * priority and responds to the Alert Limit Register.
 *
 * Current_LSB=(Maximum Expected Current/2^15)
 * CAL=0.00512/(Current_LSB*Rshunt) //Write calibration register return must be uint
 * D15   D14 D13 D12  D11  D10  D9 D8 D7 D6 D5  D4  D3   D2  D1   D0
 * SOL   SUL BOL BUL  POL  CNVR  — —  —  —  —   AFF CVRF OVF APOL LEN
 * SOL -> Shunt Voltage Over-Voltage
 * SUL -> Shunt Voltage Under-Voltage(than 2.5 mV)
 * BOL -> Bus Voltage Over-Voltage
 * BUL -> Bus Voltage Under-Voltage
 * POL -> Power Over-Limit
 * CNVR -> Power Over-Limit
 * AFF -> Alert Function Flag
 * CVRF -> Conversion Ready Flag
 * OVF -> Math Overflow Flag
 * APOL -> Alert Polarity bit; sets the Alert pin polarity
 * LEN -> Alert Latch Enable; configures the latching feature of the Alert pin and Alert Flag bits.
 * */


enum {
	NA_INA226_ADDRESS_DEFAULT=0x40,
	NA_INA226_ADDRESS_A1_GND_A0_GND=NA_INA226_ADDRESS_DEFAULT,
	NA_INA226_ADDRESS_A1_GND_A0_VS,
	NA_INA226_ADDRESS_A1_GND_A0_SDA,
	NA_INA226_ADDRESS_A1_GND_A0_SCL,
	NA_INA226_ADDRESS_A1_VS_A0_GND,
	NA_INA226_ADDRESS_A1_VS_A0_VS,
	NA_INA226_ADDRESS_A1_VS_A0_SDA,
	NA_INA226_ADDRESS_A1_VS_A0_SCL,
	NA_INA226_ADDRESS_A1_SDA_A0_GND,
	NA_INA226_ADDRESS_A1_SDA_A0_VS,
	NA_INA226_ADDRESS_A1_SDA_A0_SDA,
	NA_INA226_ADDRESS_A1_SDA_A0_SCL,
	NA_INA226_ADDRESS_A1_SCL_A0_GND,
	NA_INA226_ADDRESS_A1_SCL_A0_VS,
	NA_INA226_ADDRESS_A1_SCL_A0_SDA,
	NA_INA226_ADDRESS_A1_SCL_A0_SCL

};

enum {
	NA_INA226_AVG_1_RATE=0,
	NA_INA226_AVG_4_RATE,
	NA_INA226_AVG_16_RATE,
	NA_INA226_AVG_64_RATE,
	NA_INA226_AVG_128_RATE,
	NA_INA226_AVG_256_RATE,
	NA_INA226_AVG_512_RATE,
	NA_INA226_AVG_1024_RATE
};
enum {
	NA_INA226_CONV_TIME_140USEC=0,
	NA_INA226_CONV_TIME_204USEC,
	NA_INA226_CONV_TIME_332USEC,
	NA_INA226_CONV_TIME_588USEC,
	NA_INA226_CONV_TIME_1_1MSEC,
	NA_INA226_CONV_TIME_2_116MSEC,
	NA_INA226_CONV_TIME_4_156MSEC,
	NA_INA226_CONV_TIME_8_244MSEC
};
enum {
	NA_INA226_MODE_SHUTDOWN=0,
	NA_INA226_MODE_TRIGGERED_ONLY_SHUNT,
	NA_INA226_MODE_TRIGGERED_ONLY_BUS,
	NA_INA226_MODE_TRIGGERED_SHUNT_AND_BUS,
	NA_INA226_MODE_CONTINUOUS_ONLY_SHUNT=5,
	NA_INA226_MODE_CONTINUOUS_ONLY_BUS,
	NA_INA226_MODE_CONTINUOUS_SHUNT_AND_BUS
};

enum {
	NA_INA226_MASK_ENABLE_USE_DEFAULT=0x0,
	NA_INA226_MASK_ENABLE_REG_LEN_BIT=0x1,
	NA_INA226_MASK_ENABLE_REG_APOL_BIT=0x2,
	NA_INA226_MASK_ENABLE_REG_CNVR_BIT=0x400,
	NA_INA226_MASK_ENABLE_REG_POL_BIT=0x800,
	NA_INA226_MASK_ENABLE_REG_BUL_BIT=0x1000,
	NA_INA226_MASK_ENABLE_REG_BOL_BIT=0x2000,
	NA_INA226_MASK_ENABLE_REG_SUL_BIT=0x4000,
	NA_INA226_MASK_ENABLE_REG_SOL_BIT=0x8000
};

/* r/w
 * D15 D14 D13 D12  D11  D10   D9 D8 D7 D6 D5 D4  D3   D2  D1   D0
 * SOL SUL BOL BUL  POL  CNVR  -  -  -  -  -  AFF CVRF OVF APOL LEN
 * SOL: Shunt voltage Over-Voltage
 * SUL: Shunt Voltage Under-Voltage
 * BOL: Bus Voltage Over-Voltage
 * BUL: Bus Voltage Under-Voltage
 * POL: Power Over-Limit
 * CNVR: Conversion Ready
 * AFF: Alert Function Flag
 * CVRF: Conversion Ready Flag
 * OVF: Math Overflow Flag
 * APOL: Alert Polarity bit; sets the Alert pin polarity.
 * 1 = Inverted (active-high open collector)
 * 0 = Normal (active-low open collector) (default)
 * LEN: Alert Latch Enable; configures the latching feature of the Alert pin and Alert Flag bits.
 * 1 = Latch enabled
 * 0 = Transparent (default)
 * */

typedef struct{
 bool SOL_BIT:1;	//enable/disable
 bool SUL_BIT:1;	//enable/disable
 bool BOL_BIT:1;	//enable/disable
 bool BUL_BIT:1;	//enable/disable
 bool POL_BIT:1;	//enable/disable
 bool CNVR_BIT:1;	//enable/disable
 bool AFF_BIT:1;	//flag
 bool CVRF_BIT:1;	//flag
 bool OVF_BIT:1;	//flag
 bool APOL_BIT:1;	//enable/disable
 bool LEN_BIT:1;	//enable/disable
}na_Ina2256_mask_enable_reg;

/* r/w
 * D15 D14 D13 D12  D11  D10  D9     D8      D7      D6     D5     D4     D3    D2    D1    D0
 * rst  -   -   -  AVG2 AVG1 AVG0 VBUSCT2 VBUSCT1 VBUSCT0 VSHCT2 VSHCT1 VSHCT0 MODE3 MODE2 MODE1
 * */

typedef struct{
 bool RST_BITS:1;
 uint8_t AVG_RATE_BITS:3;
 uint8_t VBUSCT_BITS:3;
 uint8_t VSHCT_BITS:3;
 uint8_t MODE_BITS:3;
}na_Ina2256_conf_reg;

/* r
 * D15   D14   D13  D12  D11  D10  D9   D8   D7   D6   D5   D4   D3   D2   D1   D0
 * DID11 DID10 DID9 DID8 DID7 DID6 DID5 DID4 DID3 DID2 DID1 DID0 RID3 RID2 RID1 RID0
 *
 * DIDx: Device ID Bits, desc:Stores the device identification bits
 * RIDx: Die Revision ID Bits, desc: Stores the device revision identification bits
 * */

typedef struct{
 uint16_t DID_BITS:12;
 uint8_t  RID_BITS:4;
 }na_Ina2256_die_id_reg;

typedef struct{
 uint8_t address;
 uint16_t timeout;
 double Rshunt;
 na_Ina2256_mask_enable_reg enable_reg;
 na_Ina2256_conf_reg conf_reg;
 na_Ina2256_die_id_reg die_Id_reg;
 I2C_HandleTypeDef *hi2c;

}na_Ina2256;


na_Ina2256 na_Ina226_drv_create( I2C_HandleTypeDef *hi2c,uint8_t address,uint8_t ratebits,
		uint8_t vshctbits,uint8_t vbusctbits,uint8_t modebits,
		uint16_t mask_enable_bits,uint16_t timeout,double rSunt);
bool na_Ina226_drv_SetConfig(na_Ina2256 *drv);
bool na_Ina226_drv_SetCalib(na_Ina2256 *drv);
bool na_Ina226_drv_SetShuntVoltageLimit(na_Ina2256 *drv);
double na_Ina226_drv_GetShuntVoltage(na_Ina2256 *drv);
double na_Ina226_drv_GetBusVoltage(na_Ina2256 *drv);
double na_Ina226_drv_GetTotalVoltage(na_Ina2256 *drv);
double na_Ina226_drv_GetCurrent_mA(na_Ina2256 *drv);
double na_Ina226_drv_GetCurrent_A(na_Ina2256 *drv);
double na_Ina226_drv_GetPower_mW(na_Ina2256 *drv);
double na_Ina226_drv_GetPower_W(na_Ina2256 *drv);
bool na_Ina226_drv_GetMask_Enable_RegVal(na_Ina2256 *drv);
bool na_Ina226_drv_GetDie_RegVal(na_Ina2256 *drv);
#endif /* NA_INA226DRV_H_ */
