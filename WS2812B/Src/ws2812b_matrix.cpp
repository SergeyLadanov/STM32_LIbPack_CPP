/*
 * ws2812b_matrix.c
 *
 *  Created on: 7 февр. 2021 г.
 *      Author: Acer
 */
#include <ws2812b_matrix.hpp>

#define MAX_POSITION 0xFFFFFFFF

// Указание типа матрицы
void WS2812MTRX::SetType(uint8_t type)
{
	Type = type;
}

// Получение позиции пикселя исходя из заданных координат
uint32_t WS2812MTRX::GetPosition(uint16_t x, uint16_t y)
{
	uint32_t position = 0;

	if ((x >= Width) || (y >= Height))
	{
		return MAX_POSITION;
	}

	if (Type == WS2812MTRX_ZIGZAG)
	{
		// Проверка на четность
		if ((y % 2) != 0)
		{
			x = (Width - 1) - x;
		}
	}

	position = x + (y * Width);


	return position;
}

// Установка пикселя по коду цвета
void WS2812MTRX::SetPixelXY(uint16_t x, uint16_t y, uint32_t color)
{
	uint32_t pos = GetPosition(x, y);

	if (pos != MAX_POSITION)
	{
		SetPixel(color, pos);
	}
}

// Установка пикселя в RGB формате
void WS2812MTRX::SetPixelXY(uint16_t x, uint16_t y, WS2812_RGB color)
{
	uint32_t pos = GetPosition(x, y);

	if (pos != MAX_POSITION)
	{
		SetPixel(color, pos);
	}
}

// Установка пикселя в HSV формате
void WS2812MTRX::SetPixelXY(uint16_t x, uint16_t y, WS2812_HSV color)
{
	uint32_t pos = GetPosition(x, y);

	if (pos != MAX_POSITION)
	{
		SetPixel(color, pos);
	}
}

// Получение цвета пикселя по координатам в формате RGB
WS2812_RGB WS2812MTRX::GetColorXY_RGB(uint16_t x, uint16_t y)
{
	uint32_t pos = GetPosition(x, y);
	WS2812_RGB result = {0};

	if (pos != MAX_POSITION)
	{
		result = GetColor(pos);
	}

	return result;
}



