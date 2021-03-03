/*
 * serial.c
 *
 *  Created on: 29 авг. 2019 г.
 *      Author: Acer
 */
#include "serial.hpp"


#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

#ifndef __weak
#define __weak __attribute__((weak))
#endif

static uint8_t SerialReadBuf[SERIAL_READ_BUF_LEN];

// Обработчик таймаута
__weak void Serial::TimeOutCallBack(void)
{
	UNUSED(nullptr);
}


//Функция отправки данных
__weak uint8_t Serial::SendData(uint8_t *data, uint16_t len)
{
	UNUSED(data);
	UNUSED(len);

	return 0;
}


// Конструкторы класса
Serial::Serial(uint8_t *ringBuf,  uint32_t ringbufSize)
{
	RingBuffer = ringBuf;
	Size = ringbufSize;
	Arg = nullptr;
}
//---------------------------------
Serial::Serial(uint8_t *ringBuf,  uint32_t ringbufSize, void *arg)
{
	RingBuffer = ringBuf;
	Size = ringbufSize;
	Arg = arg;
}

//Функция системного таймера
void Serial::TickHandle(void)
{
	if (RecTimeOutCnt > 0)
	{
		RecTimeOutCnt--;

		if (RecTimeOutCnt == 0)
		{
			TimeOutCallBack();
		}
	}
}
//-------------------------------------------------------------
void Serial::ByteHandle(uint8_t data)
{
	//Установка счетчика таймаута
	RecTimeOutCnt = TimeOutValue;
	//Запись данных в кольцевой буфер
	RingBuffer[WriteIndex] = data;
	//Увеличение количества непрочитанных байт
	if (UnReadCnt < (Size - 1))
	{
		UnReadCnt++;
	}
	//Увеличение индекса записи
	WriteIndex = (WriteIndex + 1) % Size;
}

//--------------------------------------------------------------
//Функция чтения необработанного байта из кольцевого буфера
uint8_t Serial::GetReceivedByte(void)
{
	uint8_t byteResult = 0;

	if (UnReadCnt)
	{
		byteResult = RingBuffer[(WriteIndex + (Size - UnReadCnt)) % Size];

		if (UnReadCnt > 0)
		{
			UnReadCnt--;
		}

		return byteResult;
	}

	return byteResult;
}
//------------------------------------
uint16_t Serial::Available(void)
{
	while(RecTimeOutCnt);
	return UnReadCnt;
}
//-----------------------------------------
char* Serial::GetBytesUntil(char terminator)
{
	char *ptr = (char *) SerialReadBuf;
	uint32_t limit = 0;
	uint8_t recByte = 0;

	if (Available())
	{
		memset(SerialReadBuf, 0, SERIAL_READ_BUF_LEN);
		while (Available())
		{
			recByte = GetReceivedByte();

			limit++;

			if (recByte == terminator)
			{
				break;
			}

			if (limit >= SERIAL_READ_BUF_LEN)
			{
				return NULL;
			}

			*ptr++ = recByte;
		}

		return (char *) SerialReadBuf;
	}

	return NULL;
}
//--------------------------------------------------------------
uint16_t Serial::GetBytesUntil(uint8_t *rxBuf, char terminator, uint16_t maxLength)
{
	char *ptr = (char *)rxBuf;
	uint32_t limit = 0;
	uint8_t recByte = 0;

	if (Available())
	{
		memset(rxBuf, 0, maxLength);
		while (Available())
		{
			recByte = GetReceivedByte();

			limit++;

			if (recByte == terminator)
			{
				break;
			}

			if (limit >= maxLength)
			{
				break;
			}

			*ptr++ = recByte;
		}
		return limit;
	}

	return 0;
}
//--------------------------------------------------------------
uint16_t Serial::GetBytes(uint8_t *rxBuf, uint16_t maxLength)
{
	char *ptr = (char *)rxBuf;
	uint32_t limit = 0;
	if (Available())
	{
		memset(rxBuf, 0, maxLength);
		while (Available())
		{
			*ptr++ = GetReceivedByte();

			limit++;

			if (limit >= maxLength)
			{
				break;
			}
		}
		return limit;
	}

	return 0;
}
//---------------------------------------------------------------
char* Serial::GetBytes(void)
{
	char *ptr = (char *) SerialReadBuf;
	uint32_t limit = 0;
	if (Available())
	{
		memset(SerialReadBuf, 0, SERIAL_READ_BUF_LEN);
		while (Available())
		{
			*ptr++ = GetReceivedByte();
			limit++;
			if (limit >= SERIAL_READ_BUF_LEN)
			{
				break;
			}
		}
		return (char *) SerialReadBuf;
	}

	return NULL;
}
//---------------------------------------
void Serial::SetTimeOutVal(volatile uint16_t val)
{
	TimeOutValue = val;
}
//------------------------------------------------------
char Serial::GetLastChar(void)
{
	uint16_t lastCharIndex = 0;

	if (UnReadCnt > 0)
	{
		lastCharIndex = ((WriteIndex + (Size - 1))) % Size;
		return (char)RingBuffer[lastCharIndex];
	}
	else
	{
		return 0;
	}
}
//-----------------------------------------
uint8_t Serial::LastSymbolIs(char symbol)
{
	if (GetLastChar() == symbol)
	{
		return 1;
	}

	return 0;
}
