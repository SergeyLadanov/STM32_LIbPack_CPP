/*
 * ws2812b.h
 *
 *  Created on: 7 февр. 2021 г.
 *      Author: Acer
 */

#ifndef WS2812B_INC_WS2812B_HPP_
#define WS2812B_INC_WS2812B_HPP_



#include "ws2812b_types.hpp"

#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

#ifndef __weak
#define __weak __attribute__((weak))
#endif



class WS2812 {
private:
	void *Arg;
	WS2812_RGB *RGB_Buf;
	uint32_t Size;
public:
	//Конструктор по умолчанию
	inline WS2812(void){};
	//  Конструктор с аргументом
	inline WS2812(WS2812_RGB *rgb_buf, uint32_t size, void *arg)
	{
		RGB_Buf = rgb_buf;
		Size = size;
		Arg = arg;
	}
	// Конструктор без аргумента
	inline WS2812(WS2812_RGB *rgb_buf, uint32_t size)
	{
		RGB_Buf = rgb_buf;
		Size = size;
		Arg = nullptr;
	}
	void SetPixel(uint32_t color, uint32_t pos);
	void SetPixel(WS2812_RGB rgb, uint32_t pos);
	void SetPixel(WS2812_HSV hsv, uint32_t pos);
	void SetAll(uint32_t color);
	void SetAll(WS2812_HSV hsv);
	void SetAll(WS2812_RGB rgb);
	void Show(void);
	void Delay_ms(volatile uint32_t value);
	void Clear(void);
	void Off(void);
	constexpr void *GetArg(void){return Arg;}
	constexpr WS2812_RGB *GetBuf(void){return RGB_Buf;}
	constexpr uint32_t GetSize(void){return Size;}
	constexpr WS2812_RGB GetColor(uint32_t pos){return RGB_Buf[pos];}
	// Преобразование RGB в код
	static constexpr uint32_t RGB_To_Code(WS2812_RGB color)
	{
		return (((uint32_t)color.raw[0] << 16) | ((uint32_t)color.raw[1] << 8 ) | (uint32_t)color.raw[2]);
	}

	static WS2812_RGB HsvToRgb(WS2812_HSV hsv);
};







#endif /* WS2812B_INC_WS2812B_HPP_ */
