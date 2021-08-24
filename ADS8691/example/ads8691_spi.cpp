/*
 * ads8691_spi.cpp
 *
 *  Created on: 23 авг. 2021 г.
 *      Author: serge
 */

#include "ads8691_spi.hpp"
#include "main.h"


extern TIM_HandleTypeDef htim4;



void ADS8691_SPI::Delay(uint32_t value)
{
	HAL_Delay(value);
}

void ADS8691_SPI::WriteRawData(uint8_t *txData, uint16_t len)
{
	uint8_t rxData[len];
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) Arg;
	HAL_SPI_TransmitReceive(hspi, txData, rxData, len, 100);
}

void ADS8691_SPI::ReadRawData(uint8_t *rxData, uint16_t len)
{
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *) Arg;
	uint8_t *txData = (uint8_t *) GetVoidBuf();
	HAL_SPI_TransmitReceive(hspi, txData, rxData, len, 100);
}

uint16_t ADS8691_SPI::ReadRVS(void)
{
	return (ADC_RVS_GPIO_Port->IDR & ADC_RVS_Pin);
}

void ADS8691_SPI::WriteConv(bool state)
{
	switch(state)
	{
	case CONV_SET:
		ADC_CONV_GPIO_Port->ODR |= ADC_CONV_Pin;
		break;

	case CONV_RESET:
		ADC_CONV_GPIO_Port->ODR &= ~ADC_CONV_Pin;
		break;
	}
}


void ADS8691_SPI::StartContinuousConv(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = ADC_CONV_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(ADC_CONV_GPIO_Port, &GPIO_InitStruct);

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

void ADS8691_SPI::StopContinuousConv(void)
{
	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = ADC_CONV_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(ADC_CONV_GPIO_Port, &GPIO_InitStruct);

	ADC_CONV_GPIO_Port->ODR |= ADC_CONV_Pin;
}

