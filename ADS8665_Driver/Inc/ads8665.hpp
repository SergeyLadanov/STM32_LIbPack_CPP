/*
 * ads8691.h
 *
 *  Created on: 19 апр. 2018 г.
 *      Author: Acer
 */

#ifndef ADS8665_DRIVER_INC_ADS8665_HPP_
#define ADS8665_DRIVER_INC_ADS8665_HPP_




#include "main.h"

//Количество устройств в цепочке
#define MAX_DEVICES_NUMBER 6

// Номера АЦП
#define DEVICE_1  0
#define DEVICE_2  1
#define DEVICE_3  2
#define DEVICE_4  3
#define DEVICE_5  4
#define DEVICE_6  5
#define DEVICE_7  6
#define DEVICE_8  7
#define DEVICE_9  8
#define DEVICE_10 9
#define DEVICE_11 10
#define DEVICE_12 11
/****************КОМАНДЫ*************************/
#define CMD_NOP 		(uint32_t)(0b00000000000000000000000000000000)
#define CMD_CLEAR_HWORD (uint32_t)(0b11000000000000000000000000000000)
#define CMD_READ_HWORD 	(uint32_t)(0b11001000000000000000000000000000)
#define CMD_READ_BYTE 	(uint32_t)(0b01001000000000000000000000000000)
#define CMD_WRITE	 	(uint32_t)(0b11010000000000000000000000000000)
#define CMD_WRITE_MSB	(uint32_t)(0b11010010000000000000000000000000)
#define CMD_WRITE_LSB	(uint32_t)(0b11010100000000000000000000000000)
#define CMD_SET_HWORD	(uint32_t)(0b11011000000000000000000000000000)
/************************************************/
//Регистры
#define DEVICE_ID_REG  	0x00
#define RST_PWRCTL_REG 	0x04
#define SDI_CTL_REG 	0x08
#define SDO_CTL_REG 	0x0C
#define DATAOUT_CTL_REG 0x10
#define RANGE_SEL_REG 	0x14
#define ALARM_REG 		0x20
#define ALARM_H_TH_REG 	0x24
#define ALARM_L_TH_REG 	0x28
/************************************************/
//Настройки
#define INTREF_DISABLE 	 (uint16_t)(0b0000000001000000)
#define INTREF_ENABLE	 (uint16_t)(0b0000000000000000)
//Настройки биполярного диапазона
#define RANGE_PN_3_0xVREF 		0b0000
#define RANGE_PN_2_5xVREF 		0b0001
#define RANGE_PN_1_5xVREF 		0b0010
#define RANGE_PN_1_25xVREF 		0b0011
#define RANGE_PN_0_625xVREF 	0b0100
//Настройки однополярного диапазона
#define RANGE_P_3_0xVREF 		0b1000
#define RANGE_P_2_5xVREF 		0b1001
#define RANGE_P_1_5xVREF 		0b1010
#define RANGE_P_1_25xVREF 		0b1011

#define ADS8691_CONV_SET true
#define ADS8691_CONV_RESET false
/************************************************/
class ADS8665{
private:
	void *Arg;
	uint8_t DeviceNumber;
	uint32_t *CmdBuf;
	uint32_t *VoidBuf;
	int32_t *Result;
public:
	// Конструкторы класса
	inline ADS8665(){}
	ADS8665(uint8_t deviceNumber, int32_t *adc_buf);
	ADS8665(uint8_t deviceNumber, int32_t *adc_buf, void *arg);
	void SendCommands(void);
	void Read(uint8_t *rxData, uint16_t len);
	void Write(uint8_t *txData, uint16_t len);
	uint8_t CmdPrepare(uint8_t deviceNumver, uint32_t cmd, uint16_t adr, uint16_t data);
	void HandleResult(int32_t *output);
	inline int32_t *GetAdcBuf(){return Result;}
	inline uint32_t GetBufSize(){return DeviceNumber * sizeof(int32_t);}

	static void StartContinuousConv(ADS8665 *hadc);
	static void StopContinuousConv(ADS8665 *hadc);
	static uint32_t* GetVoidBuf(void);

private:
	void WriteRawData(uint8_t *txData, uint16_t len);
	void ReadRawData(uint8_t *rxData, uint16_t len);
	uint16_t ReadRVS();
	void ConvComplete(float *dataf);
	void WriteConv(bool state);
};

#endif /* ADS8665_DRIVER_INC_ADS8665_HPP_ */
