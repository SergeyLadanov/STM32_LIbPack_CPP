/*
 * HardWareSerial.hpp
 *
 *  Created on: 24 сент. 2021 г.
 *      Author: serge
 */

#ifndef INC_HARDWARESERIAL_HPP_
#define INC_HARDWARESERIAL_HPP_


#include "main.h"
#include "serial.hpp"

class HardwareSerial : public Serial
{
public:
	inline HardwareSerial(uint8_t *ringBuf = nullptr,  uint32_t ringbufSize = 0, void *arg = nullptr)
		: Serial(ringBuf, ringbufSize, arg)
	{

	}

	// Функция отправки данных
	virtual uint8_t SendData(uint8_t *data, uint16_t len)
	{
		UART_HandleTypeDef *huart = (UART_HandleTypeDef *) Arg;
		uint8_t status = HAL_OK;
		while(HAL_UART_Transmit_IT(huart, data, len) != HAL_OK);
		while(huart->TxXferCount);

		return status;
	}


	// Обработчик таймаута
	virtual void TimeOutCallBack(void)
	{

	}
};




#endif /* INC_HARDWARESERIAL_HPP_ */
