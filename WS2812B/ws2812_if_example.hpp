/*
 * ws2812_if.h
 *
 *  Created on: 8 февр. 2021 г.
 *      Author: serge
 */

#ifndef INC_WS2812_IF_H_
#define INC_WS2812_IF_H_


#include "main.h"
#include <ws2812b.hpp>
#include "string.h"


// Получить размер буфера в байтах на основе количество обрабатываемых светодиодов
constexpr uint32_t WS2812_GET_FRAME_BUFFER_SIZE(uint32_t X)
{
	return (X * 24);
}


// Структура управления пакетом данных
class WS2812_FrameControl
{
public:
	uint8_t FrameOffset;
	uint8_t Busy;
	uint8_t *Buffer;
	uint32_t Size;

	inline WS2812_FrameControl(uint8_t *buf, uint32_t size)
	{
		Buffer = buf;
		Size = size;
		FrameOffset = 0;
		Busy = FRAME_STOP;
	}
	void RGB_To_DMA(WS2812 *hws, uint32_t offset);
	void Handle(WS2812 *hws, uint32_t offset);

	// Статусы передачи пакета
	typedef enum
	{
		FRAME_STOP = 0,
		FRAME_STOP_REQUEST,
		FRAME_START
	} Status;

private:
	// Значения шим для передачи логических 0 и 1
	typedef enum
	{
		HIGH = 65,
		LOW = 26
	} LogicValue;

	// Проверка установки бита
	constexpr bool BitIsSet(uint8_t reg, uint8_t bit)
	{
		return ((reg & (1<<bit)) != 0);
	}
	// Получить количество обрабатываемых светодиодов исходя из размера буфера
	constexpr uint32_t GetLedCount(uint32_t X)
	{
		return (X / 24);
	}

};



#endif /* INC_WS2812_IF_H_ */
