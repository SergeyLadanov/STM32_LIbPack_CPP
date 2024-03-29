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
#define SERIAL_USE_VIRTUAL_INTERFACE 1
#define SERIAL_USE_RX_LOCKS 1
#define SERIAL_USE_WAITTIMEOUT_HANDLER 1
#define SERIAL_USE_WAITTFORRECEIVEDATA 1

//--------------------------------------
class Serial{
protected:
	void *Arg;
	uint8_t *RingBuffer = nullptr; //Приемный кольцевой буфер
	uint32_t Size = 0; // Размер приемного кольцевого буфера
	uint16_t WriteIndex = 0; //Индекс записи в кольцевой буфер
	uint16_t UnReadCnt = 0; //Количество непрочитанных байт
	uint16_t RecTimeOutCnt = 0; //Переменная счета времени таймаута приема
	uint16_t TimeOutValue = 100; //Значение таймаута
public:
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
	void UpdateBufferIndex(uint32_t new_index);


	inline Serial(uint8_t *ringBuf = nullptr,  uint32_t ringbufSize = 0, void *arg = nullptr)
	{
		RingBuffer = ringBuf;
		Size = ringbufSize;
		Arg = arg;
	}

#if SERIAL_USE_WAITTFORRECEIVEDATA != 0
#if SERIAL_USE_VIRTUAL_INTERFACE != 0
	virtual bool WaitForReceive(uint32_t timeout) = 0;
#else
	bool WaitForReceive(uint32_t timeout);
#endif
#endif

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


#if SERIAL_USE_VIRTUAL_INTERFACE != 0
#if SERIAL_USE_RX_LOCKS != 0
	virtual void RxLock(void) = 0;
	virtual void RxUnLock(void) = 0;
#endif

#if SERIAL_USE_WAITTIMEOUT_HANDLER != 0
	virtual void TimeOutHandle(uint16_t value) = 0;
#endif
#else
#if SERIAL_USE_RX_LOCKS != 0
	void RxLock(void);
	void RxUnLock(void);
#endif

#if SERIAL_USE_WAITTIMEOUT_HANDLER != 0
	void TimeOutHandle(uint16_t value);
#endif
#endif

};

#endif /* SERIAL_INC_SERIAL_HPP_ */
