/*
 * serial.h
 *
 *  Created on: 29 авг. 2019 г.
 *      Author: Acer
 */

#ifndef SERIAL_INC_SERIAL_HPP_
#define SERIAL_INC_SERIAL_HPP_


#include <cstdint>
#include <cstring>
#include <cstdlib>

//Настройки библиотеки
#define SERIAL_READ_BUF_LEN 1024
#define SERIAL_USE_VIRTUAL_INTERFACE 0

//--------------------------------------
class Serial{
protected:
	void *Arg;
	uint8_t *RingBuffer; //Приемный кольцевой буфер
	uint32_t Size; // Размер приемного кольцевого буфера
	uint16_t WriteIndex; //Индекс записи в кольцевой буфер
	uint16_t UnReadCnt; //Количество непрочитанных байт
	uint16_t RecTimeOutCnt; //Переменная счета времени таймаута приема
	uint16_t TimeOutValue; //Значение таймаута
public:
	inline Serial(void){};
	Serial(uint8_t *ringBuf,  uint32_t ringbufSize);
	Serial(uint8_t *ringBuf,  uint32_t ringbufSize, void *arg);
	void TickHandle(void);
	void ByteHandle(uint8_t data);
	uint8_t GetReceivedByte(void);
	uint16_t Available(void);
	char* GetBytes(void);
	uint16_t GetBytes(uint8_t *rxBuf, uint16_t maxLength);
	void SetTimeOutVal(volatile uint16_t val);
	char* GetBytesUntil(char terminator);
	uint16_t GetBytesUntil(uint8_t *rxBuf, char terminator, uint16_t maxLength);
	char GetLastChar(void);
	uint8_t LastSymbolIs(char symbol);
	// Переопределяемые методы
	// Передача данных
#if SERIAL_USE_VIRTUAL_INTERFACE != 0
	virtual uint8_t SendData(uint8_t *data, uint16_t len) = 0;
#else
	uint8_t SendData(uint8_t *data, uint16_t len);
#endif
private:
	// Обработчик таймаута
#if SERIAL_USE_VIRTUAL_INTERFACE != 0
	virtual void TimeOutCallBack() = 0;
#else
	void TimeOutCallBack();
#endif
};

#endif /* SERIAL_INC_SERIAL_HPP_ */
