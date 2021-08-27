/*
 * button.c
 *
 *  Created on: Jan 7, 2021
 *      Author: Acer
 */

#include "button.hpp"


// Функция получения глобального объекта кнопок
__weak Button& Button::Instance()
{
	static Button hbut;
	return hbut;
}

// Функция получения глобального объекта кнопок
__weak ButtonMultiClick& ButtonMultiClick::Instance()
{
	static ButtonMultiClick hbut;
	return hbut;
}

// Функция обработки кнопок (обычный режим)
void Button::Handle(void)
{
	for (uint8_t i = 0; i < Count; i++)
	{
#if (BUTTON_INVERSE == 1)
		if ((Desc[i].GPIO_Port->IDR & Desc[i].Pin) == 0)
#else
		if ((Desc[i].GPIO_Port->IDR & Desc[i].Pin))
#endif
		{

			if ((Prop[i].HoldDelay) < (BUTTON_HOLD_TIME + 1))
			{
				Prop[i].HoldDelay++;
				if (Prop[i].HoldDelay == BUTTON_HOLD_TIME)
				{
#if BUTTON_USE_CALLBACKS != 0
					if (Desc[i].HoldCallBack != NULL)
					{
						Desc[i].HoldCallBack();
					}
#endif

#if BUTTON_USE_OBSERVER != 0
					if (Observer != nullptr)
					{
						Observer->HoldCallBack(this, i);
					}
#endif
				}
			}
		}
		else
		{
			if ((Prop[i].HoldDelay > 0) && (Prop[i].HoldDelay < BUTTON_HOLD_TIME))
			{
#if BUTTON_USE_CALLBACKS != 0

				if (Desc[i].PressedCallBack != NULL)
				{
					Desc[i].PressedCallBack();
				}
#endif

#if BUTTON_USE_OBSERVER != 0
				if (Observer != nullptr)
				{
					Observer->PressedCallBack(this, i);
				}
#endif
			}
			Prop[i].HoldDelay = 0;
		}
	}
}

// Функция обработки кнопок (обычный мультиклик)
void ButtonMultiClick::Handle(void)
{
	for (uint8_t i = 0; i < Count; i++)
	{
#if (BUTTON_INVERSE == 1)
		if (Desc[i].GPIO_Port->IDR & Desc[i].Pin) == 0)
#else
		if ((Desc[i].GPIO_Port->IDR & Desc[i].Pin))
#endif
		{

			if ((!Prop[i].HoldDelay) && (Prop[i].ClickNum < BUTTON_MULTICLICK_CLICK_NUM))
			{
				Prop[i].ClickNum++;
				Prop[i].ClickDelay = BUTTON_MULTICLICK_CLICK_DELAY;
			}

			if ((Prop[i].HoldDelay) < (BUTTON_HOLD_TIME + 1))
			{
				Prop[i].HoldDelay++;
				if (Prop[i].HoldDelay == BUTTON_HOLD_TIME)
				{
					Prop[i].ClickNum = 0;
					Prop[i].ClickDelay = 0;
#if BUTTON_USE_CALLBACKS != 0
					if (Desc[i].HoldEventCallBack != NULL)
					{
						Desc[i].HoldEventCallBack();
					}
#endif

#if BUTTON_USE_OBSERVER != 0
					if (Observer != nullptr)
					{
						Observer->HoldEventCallBack(this, i);
					}
#endif
				}
			}
			else
			{
				Prop[i].HoldDivider = (Prop[i].HoldDivider + 1) % BUTTON_MULTICLICK_CLICK_HOLD_DIVIDER;

				if (!Prop[i].HoldDivider)
				{
#if BUTTON_USE_CALLBACKS != 0
					if (Desc[i].HoldCallBack != NULL)
					{
						Desc[i].HoldCallBack();
					}
#endif

#if BUTTON_USE_OBSERVER != 0
					if (Observer != nullptr)
					{
						Observer->HoldCallBack(this, i);
					}
#endif
				}
			}
		}
		else
		{
			if ((Prop[i].HoldDelay < BUTTON_HOLD_TIME))
			{
				if (Prop[i].ClickDelay)
				{
					if (Prop[i].ClickDelay == 1)
					{
						switch(Prop[i].ClickNum)
						{
						case 1 :
#if BUTTON_USE_CALLBACKS != 0
							if (Desc[i].X1ClickCallBack != NULL)
							{
								Desc[i].X1ClickCallBack();
							}
#endif

#if BUTTON_USE_OBSERVER != 0
							if (Observer != nullptr)
							{
								Observer->X1ClickCallBack(this, i);
							}
#endif
							break;
						case 2 :
#if BUTTON_USE_CALLBACKS != 0
							if (Desc[i].X2ClickCallBack != NULL)
							{
								Desc[i].X2ClickCallBack();
							}
#endif

#if BUTTON_USE_OBSERVER != 0
							if (Observer != nullptr)
							{
								Observer->X2ClickCallBack(this, i);
							}
#endif
							break;

						case 3 :
#if BUTTON_USE_CALLBACKS != 0
							if (Desc[i].X3ClickCallBack != NULL)
							{
								Desc[i].X3ClickCallBack();
							}
#endif

#if BUTTON_USE_OBSERVER != 0
							if (Observer != nullptr)
							{
								Observer->X3ClickCallBack(this, i);
							}
#endif
							break;
						}

						Prop[i].ClickNum = 0;
						Prop[i].ClickDelay = 0;
					}
					Prop[i].ClickDelay--;
				}

			}
			Prop[i].HoldDelay = 0;
		}
	}
}


