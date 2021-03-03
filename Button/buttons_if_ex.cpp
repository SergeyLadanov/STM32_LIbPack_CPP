/*
 * buttons_if.c
 *
 *  Created on: Jan 7, 2021
 *      Author: Acer
 */
#include "app.hpp"
#include "button.hpp"

#define BUTTONS_NUMBER 5

static void Button1_PressedCallBack(void);
static void Button2_PressedCallBack(void);
static void Button3_PressedCallBack(void);
static void Button4_PressedCallBack(void);
static void Button5_PressedCallBack(void);


Button& Button::Instance()
{
	// Дескрипторы кнопок
	static const ButtonDescriptor Buttons[BUTTONS_NUMBER] = {
			{BUT1_GPIO_Port, BUT1_Pin, Button1_PressedCallBack},
			{BUT2_GPIO_Port, BUT2_Pin, Button2_PressedCallBack},
			{BUT3_GPIO_Port, BUT3_Pin, Button3_PressedCallBack},
			{BUT4_GPIO_Port, BUT4_Pin, Button4_PressedCallBack},
			{BUT5_GPIO_Port, BUT5_Pin, Button5_PressedCallBack},
	};
	// Массив переменных для кнопок
	static ButtonProperties ButtonsProp[sizeof(Buttons)/sizeof(ButtonDescriptor)];

	// Объект набора кнопок
	static Button hbut(Buttons, ButtonsProp, sizeof(Buttons)/sizeof(ButtonDescriptor));

	return hbut;
}

uint8_t test1;
// Обработчик нажатия кнопки 1
static void Button1_PressedCallBack(void)
{
	App_Instance.StartRecord();
}
// Обработчик нажатия кнопки 2
static void Button2_PressedCallBack(void)
{
	App_Instance.StopRecord();
}
// Обработчик нажатия кнопки 3
static void Button3_PressedCallBack(void)
{
	test1++;
}
// Обработчик нажатия кнопки 4
static void Button4_PressedCallBack(void)
{
	test1++;
}
// Обработчик нажатия кнопки 5
static void Button5_PressedCallBack(void)
{
	test1++;
}





