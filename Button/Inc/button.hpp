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

#define BUTTON_USE_CALLBACKS 0
#define BUTTON_USE_OBSERVER 1



class Button{

public:
	// Работа с кнопкой в обычном режиме
	typedef struct __Descriptor{
		GPIO_TypeDef *GPIO_Port;
		uint16_t Pin;
	#if BUTTON_USE_CALLBACKS == 1

		void (*PressedCallBack)(void);
		void (*HoldCallBack)(void);

	#endif
	} Descriptor;


typedef struct __Properties{
	uint16_t HoldDelay;
} Properties;

private:
	const Descriptor *Desc;
	Properties *Prop;
	uint8_t Count;

#if BUTTON_USE_OBSERVER == 1
public:
	class IListener
	{
	public:
		virtual void PressedCallBack(Button *obj, uint8_t index) = 0;
		virtual void HoldCallBack(Button *obj, uint8_t index) = 0;
	};

protected:
	IListener *Observer = nullptr;


#endif


public:
#if BUTTON_USE_OBSERVER == 1

	void Bind(IListener * obj)
	{
		Observer = obj;
	}

#endif
	inline Button(){}
	// Конструктор класса кнопки (обычный режим)
	inline Button(const Descriptor *desc, Properties *prop, uint8_t size)
	{
		Desc = desc;
		Prop = prop;
		Count = size;
	}
	void Handle(void);
	static Button& Instance();
};

class ButtonMultiClick{

public:
	// Работа с кнопкой в режиме мультикликов
	typedef struct __Descriptor{
		GPIO_TypeDef *GPIO_Port;
		uint16_t Pin;

	#if BUTTON_USE_CALLBACKS == 1

		void (*X1ClickCallBack)(void);
		void (*X2ClickCallBack)(void);
		void (*X3ClickCallBack)(void);
		void (*HoldEventCallBack)(void);
		void (*HoldCallBack)(void);

	#endif
	} Descriptor;


	typedef struct __Properties{
		uint16_t HoldDelay;
		uint8_t ClickNum;
		uint16_t ClickDelay;
		uint16_t HoldDivider;
	} Properties;
private:
	const Descriptor *Desc;
	Properties *Prop;
	uint8_t Count;

#if BUTTON_USE_OBSERVER == 1
public:
	class IListener
	{
	public:
		virtual void X1ClickCallBack(ButtonMultiClick *obj, uint8_t index) = 0;
		virtual void X2ClickCallBack(ButtonMultiClick *obj, uint8_t index) = 0;
		virtual void X3ClickCallBack(ButtonMultiClick *obj, uint8_t index) = 0;
		virtual void HoldEventCallBack(ButtonMultiClick *obj, uint8_t index) = 0;
		virtual void HoldCallBack(ButtonMultiClick *obj, uint8_t index) = 0;
	};

protected:
	IListener *Observer = nullptr;

#endif

public:
#if BUTTON_USE_OBSERVER == 1

	void Bind(IListener * obj)
	{
		Observer = obj;
	}

#endif

	inline ButtonMultiClick(){}
	// Конструктор класса кнопки (обычный мультиклик)
	inline ButtonMultiClick(const Descriptor *desc, Properties *prop, uint8_t size)
	{
		Desc = desc;
		Prop = prop;
		Count = size;
	}
	void Handle(void);
	static ButtonMultiClick& Instance();
};

#endif /* BUTTON_INC_BUTTON_HPP_ */
