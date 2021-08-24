/*
 * ads8691_events.cpp
 *
 *  Created on: 23 авг. 2021 г.
 *      Author: serge
 */
#include "ads8691_spi.hpp"
#include "main.h"


extern ADS8691_SPI adc;
extern SPI_HandleTypeDef hspi1;
extern ADS8691_SPI ADC_Instance;



//Функция старта чтения АЦП через ДМА
static void ADS8691_ReceiveDMA(ADS8691 *hadc)
{
	//Отключение потока RX
	DMA2_Stream0->CR &= ~0x01;
	//Отключение потока TX
	DMA2_Stream5->CR &= ~0x01;
	//Очистка флагов
	//DMA2->LIFCR |= DMA_LIFCR_CHTIF0 | DMA_LIFCR_CHTIF3;
	//DMA2->LIFCR |= DMA_LIFCR_CTCIF0 | DMA_LIFCR_CTCIF3;

	DMA2->LIFCR |= (DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTCIF0);
	DMA2->HIFCR |= (DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTCIF5);

	//Настройка адресов переферии и памяти (поток 0)
	DMA2_Stream0->PAR = (uint32_t) &(SPI1->DR);
	DMA2_Stream0->M0AR = (uint32_t) hadc->GetBuf();
	DMA2_Stream0->NDTR = 36; //Количество передаваемых байт

	//Настройка ДМА со стороны переферии
	SPI1->CR2 |= SPI_CR2_RXDMAEN;
	SPI1->CR2 |= SPI_CR2_TXDMAEN;
	SPI1->CR2 |= SPI_CR2_ERRIE;
    //

	//Включение прерываний по завершению
	DMA2_Stream0->CR |= (DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);
	DMA2_Stream0->FCR |= DMA_IT_FE;
	//DMA2_Stream0->CR |= 0x01;

	//Настройка адресов переферии и памяти (поток 5)
	DMA2_Stream5->PAR = (uint32_t) &(SPI1->DR);
	DMA2_Stream5->M0AR = (uint32_t) hadc->GetVoidBuf();
	DMA2_Stream5->NDTR = 36;

	//Включение прерываний по завершению
	DMA2_Stream5->CR |= (DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);
	DMA2_Stream5->FCR |= DMA_IT_FE;

	DMA2_Stream0->CR |= 0x01;
	DMA2_Stream5->CR |= 0x01;

}


// Обрабочтик прерывания готовности данных
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == ADC_RVS_Pin)
	{
		ADS8691_ReceiveDMA(&ADC_Instance);
	}
}



#if ADS8691_TEST_SIGNAL != 0
// Производная
static float derivative(float *Xn_1, float Xn)
{
	float tSample = 0.00015625f;
	float Yn;

	Yn = (Xn - (*Xn_1))/tSample;
	*Xn_1 = Xn;
	return Yn;
}
#endif

int32_t result[9];
// Обработчик прерывания по приему данных с АЦП
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi1)
	{

		ADC_CONV_GPIO_Port->ODR |= ADC_CONV_Pin;
		ADC_Instance.HandleResult();

		/*****************ТЕСТОВЫЕ СИГНАЛЫ**********************/
#if ADS8691_TEST_SIGNAL != 0
		float coefU = 1 * sqrtf(2.0f);
		float coefI = 1 * sqrtf(2.0f);
		float f = 50;
		static uint32_t sampleCounter;
		static float hdivIA = 0.0f, hdivIB = 0.0f, hdivIC = 0.0f;



		//Выборки фазы А
		result[3] = coefI * 300.0 * arm_sin_f32(2*PI*f*0.00015625*sampleCounter);
		result[6] = coefU * 15000.0*arm_sin_f32(2*PI*f*0.00015625*sampleCounter);

		//Выборки фазы B
		result[4] = coefI * 300.0 * arm_sin_f32(2*PI*f*0.00015625*sampleCounter + 2*PI/3);
		result[7] = coefU * 15000.0*arm_sin_f32(2*PI*f*0.00015625*sampleCounter + 2*PI/3);
		//Выборки фазы C
		result[5] = coefI * 300.0 * arm_sin_f32(2*PI*f*0.00015625*sampleCounter - 2*PI/3);
		result[8] = coefU * 15000.0*arm_sin_f32(2*PI*f*0.00015625*sampleCounter - 2*PI/3);

		result[0] = derivative(&hdivIA, result[3]);
		result[1] = derivative(&hdivIB, result[4]);
		result[2] = derivative(&hdivIC, result[5]);

		sampleCounter = (sampleCounter + 1) % 25600;
#endif
		/*********************************************************************/

	}
}




// Обработчик получения выборки
void ADS8691_SPI::ConversionComplete(int32_t *data)
{
	memcpy((void *) result, data, sizeof(result));
//	printf("Test\r\n");
}






