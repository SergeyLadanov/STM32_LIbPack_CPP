/*
 * ws2812b.c
 *
 *  Created on: 7 февр. 2021 г.
 *      Author: Acer
 */

#include <ws2812b.hpp>



// Отправка данных на светодиоды
__weak void WS2812::Show(void)
{
	UNUSED(nullptr);
}

// Функция задержки
__weak void WS2812::Delay_ms(volatile uint32_t value)
{
	UNUSED(value);
}



// Установка цвета пикселя по цветовому коду
void WS2812::SetPixel(uint32_t color, uint32_t pos)
{
	RGB_Buf[pos].r = (uint8_t)(color >> 16);
	RGB_Buf[pos].g = (uint8_t)(color >> 8);
	RGB_Buf[pos].b = (uint8_t)(color);
}

// Установка цвета пикселя в формате RGB
void WS2812::SetPixel(WS2812_RGB rgb, uint32_t pos)
{
	RGB_Buf[pos] = rgb;
}
// Установка цвета всех пикселей в формате HSV
void WS2812::SetPixel(WS2812_HSV hsv, uint32_t pos)
{
	RGB_Buf[pos] = HsvToRgb(hsv);
}

// Установка всех пикселей по коду
void WS2812::SetAll(uint32_t color)
{
	for (uint16_t i = 0; i < Size; i++)
	{
		SetPixel(color, i);
	}
}



// Установка цвета всех пикселей в формате RGB
void WS2812::SetAll(WS2812_RGB rgb)
{
	for (uint16_t i = 0; i < Size; i++)
	{
		SetPixel(rgb, i);
	}
}


// Установка цвета всех пикселей в формате HSV
void WS2812::SetAll(WS2812_HSV hsv)
{
	for (uint16_t i = 0; i < Size; i++)
	{
		SetPixel(hsv, i);
	}
}

// Погасить все светодиоды
void WS2812::Clear(void)
{
	SetAll(0x00000000);
}

// Выключение всех светодиодов
void WS2812::Off(void)
{
	Clear();
	Show();
	Delay_ms(5);
}

// Преобразование формата HSV в RGB
WS2812_RGB WS2812::HsvToRgb(WS2812_HSV hsv)
{
	uint8_t color[3] = {0};
	uint32_t base_V;
	WS2812_RGB rgb;

	if (hsv.s == 0)
	{
		color[0] = hsv.v;
		color[1] = hsv.v;
		color[2] = hsv.v;
	}
	else
	{
		base_V = ((255 - hsv.s) * hsv.v) >> 8;
		switch (hsv.h / 60)
		{
			case 0:
				color[0] = hsv.v;
				color[1] = (((hsv.v - base_V) * hsv.h) / 60) + base_V;
				color[2] = base_V;
			break;
			case 1:
				color[0] = (((hsv.v - base_V) * (60 - (hsv.h % 60))) / 60) + base_V;
				color[1] = hsv.v;
				color[2] = base_V;
			break;
			case 2:
				color[0] = base_V;
				color[1] = hsv.v;
				color[2] = (((hsv.v - base_V) * (hsv.h % 60)) / 60) + base_V;
			break;
			case 3:
				color[0] = base_V;
				color[1] = (((hsv.v - base_V) * (60 - (hsv.h % 60))) / 60) + base_V;
				color[2] = hsv.v;
			break;
			case 4:
				color[0] = (((hsv.v - base_V) * (hsv.h % 60)) / 60) + base_V;
				color[1] = base_V;
				color[2] = hsv.v;
			break;
			case 5:
				color[0] = hsv.v;
				color[1] = base_V;
				color[2] = (((hsv.v - base_V) * (60 - (hsv.h % 60))) / 60) + base_V;
			break;
		}
	}
	rgb.r = color[0];
	rgb.g = color[1];
	rgb.b = color[2];

	return rgb;
}


