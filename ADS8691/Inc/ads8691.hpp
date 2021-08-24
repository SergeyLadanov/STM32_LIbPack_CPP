/*
 * ads8691.h
 *
 *  Created on: 19 апр. 2018 г.
 *      Author: Acer
 */

#ifndef ADS8691_DRIVER_INC_ADS8691_H_
#define ADS8691_DRIVER_INC_ADS8691_H_

#include <cstdint>
#include <cstring>

#define ADS8691_USE_VIRTUAL_INTERFACE 1

class ADS8691{

public:
	static constexpr bool CONV_SET = true;
	static constexpr bool CONV_RESET = false;

	/****************КОМАНДЫ*************************/
	static constexpr uint32_t CMD_NOP = (0b00000000000000000000000000000000);
	static constexpr uint32_t CMD_CLEAR_HWORD = (0b11000000000000000000000000000000);
	static constexpr uint32_t CMD_READ_HWORD = (0b11001000000000000000000000000000);
	static constexpr uint32_t CMD_READ_BYTE = (0b01001000000000000000000000000000);
	static constexpr uint32_t CMD_WRITE = (0b11010000000000000000000000000000);
	static constexpr uint32_t CMD_WRITE_MSB = (0b11010010000000000000000000000000);
	static constexpr uint32_t CMD_WRITE_LSB = (0b11010100000000000000000000000000);
	static constexpr uint32_t CMD_SET_HWORD = (0b11011000000000000000000000000000);
	/************************************************/
	//Регистры
	static constexpr uint8_t DEVICE_ID_REG  = 0x00;
	static constexpr uint8_t RST_PWRCTL_REG =	0x04;
	static constexpr uint8_t SDI_CTL_REG =	0x08;
	static constexpr uint8_t SDO_CTL_REG =	0x0C;
	static constexpr uint8_t DATAOUT_CTL_REG = 0x10;
	static constexpr uint8_t RANGE_SEL_REG = 0x14;
	static constexpr uint8_t ALARM_REG = 0x20;
	static constexpr uint8_t ALARM_H_TH_REG = 0x24;
	static constexpr uint8_t ALARM_L_TH_REG = 0x28;
	/************************************************/
	//Настройки
	static constexpr uint16_t INTREF_DISABLE = (0b0000000001000000);
	static constexpr uint16_t INTREF_ENABLE = (0b0000000000000000);
	//Настройки биполярного диапазона
	static constexpr uint8_t RANGE_PN_3_0xVREF = 0b0000;
	static constexpr uint8_t RANGE_PN_2_5xVREF = 0b0001;
	static constexpr uint8_t RANGE_PN_1_5xVREF = 0b0010;
	static constexpr uint8_t RANGE_PN_1_25xVREF = 0b0011;
	static constexpr uint8_t RANGE_PN_0_625xVREF = 0b0100;
	//Настройки однополярного диапазона
	static constexpr uint8_t RANGE_P_3_0xVREF = 0b1000;
	static constexpr uint8_t RANGE_P_2_5xVREF = 0b1001;
	static constexpr uint8_t RANGE_P_1_5xVREF = 0b1010;
	static constexpr uint8_t RANGE_P_1_25xVREF = 0b1011;


protected:
	void *Arg;
	uint8_t DeviceNumber;
	uint32_t *Buf;
	uint32_t *VoidBuf;


	//--------------------------------------------------------------------------------
public:

	typedef uint32_t ChannelBuffer[2];

	ADS8691(uint8_t device_number = 0, ChannelBuffer *buffer = nullptr, void *arg = nullptr)
	{
		uint32_t *Ptr = (uint32_t *) buffer;
		memset(Ptr, 0, device_number * sizeof(ChannelBuffer));
		Buf = Ptr;
		VoidBuf = &Ptr[device_number];
		Arg = arg;
		DeviceNumber = device_number;
	}

	void SetArg(void *arg);
	void SendCommands(void);
	void Read( uint8_t *rxData, uint16_t len);
	void Write(uint8_t *txData, uint16_t len);
	uint8_t CmdPrepare(uint8_t deviceNumver, uint32_t cmd, uint16_t adr, uint16_t data);

	void HandleResult(int32_t *output);
	void HandleResult(void);
	uint32_t *GetVoidBuf(void);
	uint32_t* GetBuf(void);

#if ADS8691_USE_VIRTUAL_INTERFACE != 0

	virtual void StartContinuousConv(void) = 0;
	virtual void StopContinuousConv(void) = 0;

private:
	virtual void ConversionComplete(int32_t *data) = 0;
	virtual void Delay(uint32_t value) = 0;
	virtual void WriteRawData(uint8_t *txData, uint16_t len) = 0;
	virtual void ReadRawData(uint8_t *rxData, uint16_t len) = 0;
	virtual uint16_t ReadRVS(void) = 0;
	virtual void WriteConv(bool state) = 0;

#else
	void StartContinuousConv(void);
	void StopContinuousConv(void);

private:
	void ConversionComplete(int32_t *data);
	void Delay(uint32_t value);
	void WriteRawData(uint8_t *txData, uint16_t len);
	void ReadRawData(uint8_t *rxData, uint16_t len);
	uint16_t ReadRVS(void);
	void WriteConv(bool state);

#endif

};


#endif /* ADS8691_DRIVER_INC_ADS8691_H_ */
