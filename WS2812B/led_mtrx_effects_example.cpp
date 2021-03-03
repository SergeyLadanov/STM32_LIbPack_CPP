/*
 * led_actions.c
 *
 *  Created on: 8 февр. 2021 г.
 *      Author: serge
 */
#include "main.h"
#include "led_actions.hpp"

//ДИНАМИЧЕСКИЕ
void WS2812_VerticalRainBow(WS2812MTRX *hws, uint16_t resolution, uint8_t brightness)
{
	static uint16_t step = 0;
	WS2812_HSV hsvTmp;
	for(uint16_t i=0; i < hws->GetHeight(); i++)
	{
		hsvTmp.h = (step + i * resolution) % 360;
		hsvTmp.s = 255;
		hsvTmp.v = brightness;

		for (uint16_t j = 0; j < hws->GetWidth(); j++)
		{
			hws->SetPixelXY(j, i, hsvTmp);
		}
	}

	step = (step + 1) % 360;

	hws->Show();
	hws->Delay_ms(33);
}
//------------------------------------------------------
void WS2812_HorizontalRainBow(WS2812MTRX *hws, uint16_t resolution, uint8_t brightness)
{
	static uint16_t step = 0;
	WS2812_HSV hsvTmp;
	for(uint16_t i=0; i < hws->GetHeight(); i++)
	{
		hsvTmp.h = (step + i * resolution) % 360;
		hsvTmp.s = 255;
		hsvTmp.v = brightness;

		for (uint16_t j = 0; j < hws->GetWidth(); j++)
		{
			hws->SetPixelXY(i, j, hsvTmp);
		}
	}

	step = (step + 1) % 360;

	hws->Show();
	hws->Delay_ms(33);
}
//-------------------------------------------------------
void WS2812_RunWhite(WS2812MTRX *hws, uint8_t brightness)
{
	static uint8_t pos = 0, movDir = 0, timeDelay = 0;
	WS2812_HSV color = {0};

	timeDelay = (timeDelay + 1) % 10;

	if (timeDelay == 0)
	{
		hws->Clear();

		if (!movDir)
		{
			if (pos < (hws->GetHeight() - 3))
				pos++;
			else
				movDir = ~movDir;
		}
		else
		{
			if (pos > 0)
				pos--;
			else
				movDir = ~movDir;
		}

	}

	color.v = 0.3*brightness;

	for (uint16_t i = 0; i < hws->GetWidth(); i++)
	{
		hws->SetPixelXY(i, pos, color);
	}

	color.v = brightness;

	for (uint16_t i = 0; i < hws->GetWidth(); i++)
	{
		hws->SetPixelXY(i, pos + 1, color);
	}

	color.v = 0.3*brightness;

	for (uint16_t i = 0; i < hws->GetWidth(); i++)
	{
		hws->SetPixelXY(i, pos + 2, color);
	}

	hws->Show();
	hws->Delay_ms(7);
}
//-----------------------------------------------------
void WS2812_StaticRainBow(WS2812MTRX *hws, uint16_t resolution, uint8_t brightness)
{
	static uint16_t step = 0;
	uint16_t i;
	WS2812_HSV hsvTmp;
	for(i=0; i < hws->GetSize(); i++)
	{
		hsvTmp.h = step;
		hsvTmp.s = 255;
		hsvTmp.v = brightness;
		hws->SetPixel(hsvTmp, i);
	}
	step = (step + resolution) % 360;
	hws->Show();

	hws->Delay_ms(33);
}
//---------------------------------------------
void WS2812_Confetti(WS2812MTRX *hws, uint8_t brightness)
{
	static uint8_t timeDelay = 0;
	WS2812_HSV outBuf[hws->GetSize()];

	timeDelay = (timeDelay + 1) % 5;
//Формирование задержки 200 мс
	if (timeDelay == 0)
	{
		for (uint16_t i = 0; i < 60; i++)
		{
			uint16_t ind = RND_GetRange(0, (hws->GetSize() - 1));
			outBuf[ind].v = brightness;
			outBuf[ind].s = 255;
			outBuf[ind].h = RND_GetRange(0, 359);
		}
	//-------------------------------------------
		for (uint16_t i = 0; i < 40; i++)
		{
			uint16_t ind = RND_GetRange(0, (hws->GetSize() - 1));
			outBuf[ind].v = 0;
			outBuf[ind].s = 255;
			outBuf[ind].h = 0;
		}

	}

	//-----------------------------------------
	for(uint16_t i=0; i < hws->GetSize(); i++)
	{
		//Устанавливается текущее значение яркости для активных пикселей (для плавного вкл./выкл.)
		if (outBuf[i].v != 0)
		{
			outBuf[i].v = brightness;
		}
		hws->SetPixel(outBuf[i], i);
	}

	hws->Show();
	hws->Delay_ms(20);

}

//-----------------------------------------------
void WS2812_GlowWorms(WS2812MTRX *hws, uint8_t brightness)
{
	const uint8_t quantity = 3;
	uint16_t colorBuf[quantity];

	static uint16_t pos[16][3];
	static uint16_t new_pos[16][3] = {0, 5, 10};
	static uint8_t delay[16][3] = {3, 3, 3};
	WS2812_HSV outBuf[hws->GetWidth()][hws->GetHeight()];

	memset(outBuf,0,sizeof(outBuf)); //Сброс буфера

	colorBuf[0] = 0;
	colorBuf[1] = 45;
	colorBuf[2] = 320;


	for (uint8_t cnt = 0; cnt < 16; cnt++)
	{
//--------------------------------------------
		for (uint16_t i = 0; i < quantity; i++)
		{
			outBuf[cnt][pos[cnt][i]].h = colorBuf[i];

			//Если координаты пересеклись
			if (outBuf[cnt][pos[cnt][i]].s == 0)
				outBuf[cnt][pos[cnt][i]].s = 255;
			else
				outBuf[cnt][pos[cnt][i]].s = 40;

			outBuf[cnt][pos[cnt][i]].v = brightness;
		}

	//-------------------------------------------
		for(uint16_t i=0; i < quantity; i++)
		{
			if (pos[cnt][i] < new_pos[cnt][i])
			{
				pos[cnt][i]++;
				delay[cnt][i] = 2;
			}
			else if (pos[cnt][i] > new_pos[cnt][i])
			{
				pos[cnt][i]--;
				delay[cnt][i] = 2;
			}
			else
			{
				if (delay[cnt][i] > 0)
				{
					delay[cnt][i]--;
				}
				else
				{
					new_pos[cnt][i] = RND_GetRange(0, hws->GetHeight() - 1);
				}
			}

		}
	}

	//Копирование значений на светодиоды
	for(uint16_t i=0; i < hws->GetHeight(); i++)
	{
		for (uint16_t j = 0; j < hws->GetWidth(); j++)
		{
			hws->SetPixelXY(i, j, outBuf[i][j]);
		}
	}

	hws->Show();
	hws->Delay_ms(50);
}
//----------------------------------------------

//СТАТИЧЕСКИЕ
void WS2812_StaticWhite(WS2812MTRX *hws, uint8_t brightness)
{
	WS2812_HSV hsvTmp;

	hsvTmp.h = 0;
	hsvTmp.s = 0;
	hsvTmp.v = brightness;

	hws->SetAll(hsvTmp);
	hws->Show();
	hws->Delay_ms(50);
}
//-----------------------------------------------
void WS2812_StaticOrange(WS2812MTRX *hws, uint8_t brightness)
{
	WS2812_HSV hsvTmp;

	hsvTmp.h = 40;
	hsvTmp.s = 255;
	hsvTmp.v = brightness;

	hws->SetAll(hsvTmp);
	hws->Show();
	hws->Delay_ms(50);

}
//---------------------------------------------
void WS2812_StaticGreen(WS2812MTRX *hws, uint8_t brightness)
{
	WS2812_HSV hsvTmp;

	hsvTmp.h = 100;
	hsvTmp.s = 255;
	hsvTmp.v = brightness;

	hws->SetAll(hsvTmp);
	hws->Show();
	hws->Delay_ms(50);

}
//-----------------------------------------------
void WS2812_StaticBlueLight(WS2812MTRX *hws, uint8_t brightness)
{
	WS2812_HSV hsvTmp;

	hsvTmp.h = 180;
	hsvTmp.s = 255;
	hsvTmp.v = brightness;

	hws->SetAll(hsvTmp);
	hws->Show();
	hws->Delay_ms(50);

}
//--------------------------------------
void WS2812_StaticBlue(WS2812MTRX *hws, uint8_t brightness)
{
	WS2812_HSV hsvTmp;

	hsvTmp.h = 240;
	hsvTmp.s = 255;
	hsvTmp.v = brightness;

	hws->SetAll(hsvTmp);
	hws->Show();
	hws->Delay_ms(50);

}
//--------------------------------------
void WS2812_StaticViolet(WS2812MTRX *hws, uint8_t brightness)
{
	WS2812_HSV hsvTmp;

	hsvTmp.h = 44;
	hsvTmp.s = 255;
	hsvTmp.v = brightness;

	hws->SetAll(hsvTmp);
	hws->Show();
	hws->Delay_ms(50);

}
