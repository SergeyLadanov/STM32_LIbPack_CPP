/*
 * ws2812_if.c
 *
 *  Created on: 8 февр. 2021 г.
 *      Author: serge
 */

#include <lamp.hpp>
#include <ws2812_if.hpp>

extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch2_ch4;

// Обработчик пакетов передачи данных на светодиоды
void WS2812_FrameControl::Handle(WS2812 *hws, uint32_t offset)
{
	// Загрузка очередных данных
	if (Busy == FRAME_START)
	{
		if (FrameOffset < (hws->GetSize() - (GetLedCount(Size)/2)))
		{
			FrameOffset += (GetLedCount(Size)/2);
			RGB_To_DMA(hws, offset);
		}
		else
		{
			if (!offset)
			{
				memset(Buffer, 0, Size/2);
				Busy = FRAME_STOP_REQUEST;
			}
		}
	}
	// Остановка кадра
	if (offset)
	{
		if (Busy == FRAME_STOP_REQUEST)
		{
			Busy = FRAME_STOP;
			HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_2);
		}
	}
}

// Функция подготовки буфера DMA
void WS2812_FrameControl::RGB_To_DMA(WS2812 *hws, uint32_t offset)
{
	volatile uint16_t j;
	uint8_t i;
	WS2812_RGB *RGB_Buf = hws->GetBuf();

	for(j=0; j < GetLedCount(Size)/2; j++)
	{
		for(i=0; i<8; i++)
		{
			if (BitIsSet(RGB_Buf[j + FrameOffset].r,(7-i)) == 1)
			{
				Buffer[offset + j*24+i+8] = HIGH;
			}
			else
			{
				Buffer[offset + j*24+i+8] = LOW;
			}
			if (BitIsSet(RGB_Buf[j + FrameOffset].g,(7-i)) == 1)
			{
				Buffer[offset + j*24+i+0] = HIGH;
			}
			else
			{
				Buffer[offset + j*24+i+0] = LOW;
			}
			if (BitIsSet(RGB_Buf[j + FrameOffset].b,(7-i)) == 1)
			{
				Buffer[offset + j*24+i+16] = HIGH;
			}
			else
			{
				Buffer[offset + j*24+i+16] = LOW;
			}
		}
	}
}

// Передача данных на светодиоды
void WS2812::Show(void)
{
	WS2812_FrameControl *hframe = (WS2812_FrameControl *) this->Arg;
	hframe->Busy = hframe->FRAME_START;
	hframe->FrameOffset = 0;
	//Заполнение второй половины буфера данными
	hframe->RGB_To_DMA(this, hframe->Size/2);
	//Заполнение первой половины буфера нулями
	memset(hframe->Buffer, 0, hframe->Size/2);
	//Включение прерывания по готовности половины передачи
	DMA1_Channel7->CCR |= DMA_CCR_HTIE;
	//Запуск DMA
	HAL_TIM_PWM_Start_DMA(&htim2,TIM_CHANNEL_2, (uint32_t*)hframe->Buffer, hframe->Size);
}

// Реализаци функции задержки
void WS2812::Delay_ms(volatile uint32_t value)
{
	HAL_Delay(value);
}

// Обработчик половины переданных данных на светодиоды
void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim2)
	{
		WS2812_FrameControl *hframe = (WS2812_FrameControl *) Lamp_Instance.GetMtrx()->GetArg();
		hframe->Handle(Lamp_Instance.GetMtrx(), 0);
	}
}

// Обработчик завершения передачи данных на светодиоды
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim2)
	{
		WS2812_FrameControl *hframe = (WS2812_FrameControl *) Lamp_Instance.GetMtrx()->GetArg();
		hframe->Handle(Lamp_Instance.GetMtrx(), hframe->Size/2);
	}
}










