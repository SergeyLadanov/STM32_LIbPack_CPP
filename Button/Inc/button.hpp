/*
 * button.h
 *
 *  Created on: Jan 7, 2021
 *      Author: Acer
 */

#ifndef BUTTON_INC_BUTTON_HPP_
#define BUTTON_INC_BUTTON_HPP_
#include "main.h"

#define BUTTON_HOLD_TIME 1000
#define BUTTON_INVERSE 0


#define BUTTON_MULTICLICK_CLICK_NUM 3
#define BUTTON_MULTICLICK_CLICK_DELAY 350
#define BUTTON_MULTICLICK_CLICK_HOLD_DIVIDER 15



// Работа с кнопкой в обычном режиме
typedef struct __ButtonDescriptor{
	GPIO_TypeDef *GPIO_Port;
	uint16_t Pin;
	void (*PressedCallBack)(void);
	void (*HoldCallBack)(void);
} ButtonDescriptor;


typedef struct __ButtonProperties{
	uint16_t HoldDelay;
} ButtonProperties;


// Работа с кнопкой в режиме мультикликов
typedef struct __ButtonMultiClickDescriptor{
	GPIO_TypeDef *GPIO_Port;
	uint16_t Pin;
	void (*X1ClickCallBack)(void);
	void (*X2ClickCallBack)(void);
	void (*X3ClickCallBack)(void);
	void (*HoldEventCallBack)(void);
	void (*HoldCallBack)(void);
} ButtonMultiClickDescriptor;


typedef struct __ButMultiClickProperties{
	uint16_t HoldDelay;
	uint8_t ClickNum;
	uint16_t ClickDelay;
	uint16_t HoldDivider;
} ButMultiClickProperties;

class Button{
private:
	const ButtonDescriptor *Desc;
	ButtonProperties *Prop;
	uint8_t Count;
public:
	inline Button(){}
	// Конструктор класса кнопки (обычный режим)
	inline Button(const ButtonDescriptor *desc, ButtonProperties *prop, uint8_t size)
	{
		Desc = desc;
		Prop = prop;
		Count = size;
	}
	void Handle(void);
	static Button& Instance();
};

class ButtonMultiClick{
private:
	const ButtonMultiClickDescriptor *Desc;
	ButMultiClickProperties *Prop;
	uint8_t Count;
public:
	inline ButtonMultiClick(){}
	// Конструктор класса кнопки (обычный мультиклик)
	inline ButtonMultiClick(const ButtonMultiClickDescriptor *desc, ButMultiClickProperties *prop, uint8_t size)
	{
		Desc = desc;
		Prop = prop;
		Count = size;
	}
	void Handle(void);
	static ButtonMultiClick& Instance();
};

#endif /* BUTTON_INC_BUTTON_HPP_ */
