#include <ads8665.hpp>
#include "stdio.h"


static uint32_t voidBuf[MAX_DEVICES_NUMBER] = {0};
static uint32_t cmdBuf[MAX_DEVICES_NUMBER] = {0};


/************************ШАБЛОНЫ БАЗЫОВЫХ МЕТОДОВ****************************/
// Функции передачи данных данными
__weak void ADS8665::WriteRawData(uint8_t *txData, uint16_t len)
{
	UNUSED(txData);
	UNUSED(len);
}

// Функция приема данных
__weak void ADS8665::ReadRawData(uint8_t *rxData, uint16_t len)
{
	UNUSED(rxData);
	UNUSED(len);
}

// Чтения RVS
__weak uint16_t ADS8665::ReadRVS(void)
{
	return 0;
}
// Обработчик окончания преобразования
__weak void ADS8665_ConvComplete(ADS8665 *hadc, float *dataf)
{
	UNUSED(hadc);
	UNUSED(dataf);
}

// Функция управления пином CONV
__weak void ADS8665::WriteConv(bool state)
{
	UNUSED(state);
}

// Функция запуска непрерывного преобразования
__weak void ADS8665::StartContinuousConv(ADS8665 *hadc)
{
	UNUSED(hadc);
}

// Функция остановки непрерывного преобразования
__weak void ADS8665::StopContinuousConv(ADS8665 *hadc)
{
	UNUSED(hadc);
}

uint32_t* ADS8665::GetVoidBuf(void)
{
	return voidBuf;
}
/******************************************************/

// Конструктор класса без аргумента
ADS8665::ADS8665(uint8_t deviceNumber, int32_t *adc_buf)
:DeviceNumber(deviceNumber),
 Result(adc_buf)
{
	VoidBuf = voidBuf;
	CmdBuf = cmdBuf;
};

// Конструктор класса с аргументом
ADS8665::ADS8665(uint8_t deviceNumber, int32_t *adc_buf, void *arg)
:Arg(arg),
 DeviceNumber(deviceNumber),
 Result(adc_buf)
{
	VoidBuf = voidBuf;
	CmdBuf = cmdBuf;
}

//Функция передачи данных на АЦП
void ADS8665::Write(uint8_t *txData, uint16_t len)
{
	uint16_t timeOut = 3000;

	WriteConv(ADS8691_CONV_RESET);

	while (ReadRVS())
	{
		HAL_Delay(1);

		if (timeOut > 0)
			timeOut--;
		else
		{
#ifdef DEBUG
			printf("Error of init ADC!\r\n");
#endif
			break;
		}

	}

	WriteRawData(txData, len);
	WriteConv(ADS8691_CONV_SET);
}


//Функция передачи данных на АЦП
void ADS8665::Read(uint8_t *rxData, uint16_t len)
{
	WriteConv(ADS8691_CONV_RESET);
	while (ReadRVS());
	ReadRawData(rxData, len);
	WriteConv(ADS8691_CONV_SET);
}

// Функция отправки команд
void ADS8665::SendCommands(void)
{
	Write((uint8_t *)CmdBuf, DeviceNumber * 4);
}


// Функция подготовки команды
uint8_t ADS8665::CmdPrepare(uint8_t deviceNumver, uint32_t cmd, uint16_t adr, uint16_t data)
{
	uint32_t tmp = cmd | (adr << 16) | data;
	uint8_t *ptr = (uint8_t *) &CmdBuf[deviceNumver];

	ptr[0] = ((tmp & 0xFF000000) >> 24);
	ptr[1] = ((tmp & 0x00FF0000) >> 16);
	ptr[2] = ((tmp & 0x0000FF00) >> 8);
	ptr[3] = ((tmp & 0x000000FF));

	return sizeof(uint32_t);
}

//Функция обработки результата
void ADS8665::HandleResult(int32_t *output)
{
	uint8_t *input = (uint8_t *)Result;
	int32_t res = 0;
	uint8_t pos = 0;
	for (uint8_t i = 0; (i < DeviceNumber * 4); i+=4)
	{
		res = ((input[i] << 4) | (input[i+1] >> 4)) - 2048;
		output[pos] = res;
		pos++;
	}
}


