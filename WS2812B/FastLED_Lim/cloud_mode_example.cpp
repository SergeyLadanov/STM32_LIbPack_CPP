/*
 * lava_mode.cpp
 *
 *  Created on: 16 февр. 2021 г.
 *      Author: serge
 */

#include "cloud_mode.hpp"

// настройки ленты
#define M_WIDTH 16      // ширина матрицы
#define M_HEIGHT 16      // высота матрицы
#define LED_BRIGHT 255  // яркость

// настройки пламени
#define HUE_GAP 50      // заброс по hue
#define FIRE_STEP 10    // шаг изменения "языков" пламени
#define HUE_START 140     // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
#define MIN_BRIGHT 40   // мин. яркость огня
#define MAX_BRIGHT 255  // макс. яркость огня
#define MIN_SAT 10     // мин. насыщенность
#define MAX_SAT 255     // макс. насыщенность

#define ORDER_GRB       // порядок цветов ORDER_GRB / ORDER_RGB / ORDER_BRG
#define COLOR_DEBTH 3   // цветовая глубина: 1, 2, 3 (в байтах)

#define FOR_i(from, to) for(int i = (from); i < (to); i++)
#define FOR_j(from, to) for(int j = (from); j < (to); j++)

static CRGBPalette16 pal;


// Получение цвета пикселя
static uint32_t getPixColor(CRGB thisPixel) {
  return (((uint32_t)thisPixel.r << 16) | (thisPixel.g << 8) | thisPixel.b);
}


// Генерация цвета
static CRGB getPalleteColor(int val) {
  // чем больше val, тем сильнее сдвигается цвет, падает насыщеность и растёт яркость
  return CHSV(
           HUE_START + map(val, 0, 255, 0, HUE_GAP),                    // H
           constrain(map(val, 0, 255, MAX_SAT, MIN_SAT), 0, 255),       // S
           constrain(map(val, 0, 255, MIN_BRIGHT, MAX_BRIGHT), 0, 255)  // V
         );
}


// Инициализация эффекта лавы
void Cloud_Init(void)
{
	pal = CRGBPalette16(
			getPalleteColor(0 * 16),
			getPalleteColor(1 * 16),
			getPalleteColor(2 * 16),
			getPalleteColor(3 * 16),
			getPalleteColor(4 * 16),
			getPalleteColor(5 * 16),
			getPalleteColor(6 * 16),
			getPalleteColor(7 * 16),
			getPalleteColor(8 * 16),
			getPalleteColor(9 * 16),
			getPalleteColor(10 * 16),
			getPalleteColor(11 * 16),
			getPalleteColor(12 * 16),
			getPalleteColor(13 * 16),
			getPalleteColor(14 * 16),
			getPalleteColor(15 * 16)
		);
}

// Генерация эффекта лавы
void Cloud_Handle(WS2812MTRX *hmtrx, uint8_t brightness)
{
	static int counter = 0;
    FOR_i(0, M_WIDTH) {
      FOR_j(0, M_HEIGHT) {
    	  hmtrx->SetPixelXY(i, j, getPixColor(ColorFromPalette(
																pal,
																(inoise8(i * FIRE_STEP, j * FIRE_STEP, counter)),
																brightness,
																LINEARBLEND
															)));
      }
    }
    counter += 1;
    hmtrx->Show();
    HAL_Delay(25);
}



