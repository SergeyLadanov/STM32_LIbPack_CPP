/*
 * ws2812b_matrix.h
 *
 *  Created on: 7 февр. 2021 г.
 *      Author: Acer
 */

#ifndef WS2812B_INC_WS2812B_MATRIX_HPP_
#define WS2812B_INC_WS2812B_MATRIX_HPP_


#include "ws2812b_types.hpp"
#include <ws2812b.hpp>

typedef enum {
	WS2812MTRX_PARALLEL,
	WS2812MTRX_ZIGZAG
}WS2812MTRX_Type;


class WS2812MTRX:public WS2812
{
private:
	uint16_t Height;
	uint16_t Width;
	uint8_t Type;
public:
	inline WS2812MTRX(void){};
	// Инициализация светодиодной матрицы
	inline WS2812MTRX(uint16_t height, uint16_t width, WS2812_RGB *rgb_buf)
		:WS2812(rgb_buf, (uint32_t ) (height * width)),
		 Height(height),
		 Width(width)
	{

	}
	// Передача аргумента
	inline WS2812MTRX(uint16_t height, uint16_t width, WS2812_RGB *rgb_buf, void *arg)
		:WS2812(rgb_buf, height * width, arg),
		 Height(height),
		 Width(width)
	{

	}
	void SetType(uint8_t type);
	void SetPixelXY(uint16_t x, uint16_t y, uint32_t color);
	void SetPixelXY(uint16_t x, uint16_t y, WS2812_RGB color);
	void SetPixelXY(uint16_t x, uint16_t y, WS2812_HSV color);
	constexpr uint16_t GetHeight() {return Height;}
	constexpr uint16_t GetWidth() {return Width;}
	WS2812_RGB GetColorXY_RGB(uint16_t x, uint16_t y);
private:
	uint32_t GetPosition(uint16_t x, uint16_t y);
};


#endif /* WS2812B_INC_WS2812B_MATRIX_HPP_ */
