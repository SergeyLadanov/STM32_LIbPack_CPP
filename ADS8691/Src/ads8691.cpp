#include <ads8691.hpp>




/************************ШАБЛОНЫ БАЗЫОВЫХ ФУНКЦИЙ****************************/

#if ADS8691_USE_VIRTUAL_INTERFACE == 0

#ifndef __weak
#define __weak __attribute__((weak))
#endif


#ifndef UNUSED
#define UNUSED(X) (void)X
#endif

// Функции передачи данных данными
__weak void ADS8691::WriteRawData(uint8_t *txData, uint16_t len)
{;
	UNUSED(txData);
	UNUSED(len);
}

// Функция приема данных
__weak void ADS8691::ReadRawData(uint8_t *rxData, uint16_t len)
{
	UNUSED(rxData);
	UNUSED(len);
}

// Чтения RVS
__weak uint16_t ADS8691::ReadRVS(void)
{
	UNUSED(nullptr);

	return 0;
}

// Функция запуска непрерывного преобразования
__weak void ADS8691::StartContinuousConv(void)
{
	UNUSED(nullptr);
}
// Функция остановки непрерывного преобразования
__weak void ADS8691::StopContinuousConv(void)
{
	UNUSED(nullptr);
}
// Функция управления пином CONV
__weak void ADS8691::WriteConv( bool state)
{
	UNUSED(state);
}
/**********************************************************************************/
#endif


// Передача аргумента
void ADS8691::SetArg(void *arg)
{
	Arg = arg;
}

// Получение указателя на пустой буфер
uint32_t *ADS8691::GetVoidBuf(void)
{
	return VoidBuf;
}

//Функция передачи данных на АЦП
void ADS8691::Write(uint8_t *txData, uint16_t len)
{
	uint16_t timeOut = 3000;

	WriteConv(CONV_RESET);

	while (ReadRVS())
	{
		Delay(1);

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
	WriteConv(CONV_SET);
}

//-------------------------------------------------
void ADS8691::Read(uint8_t *rxData, uint16_t len)
{
	WriteConv(CONV_RESET);
	while (ReadRVS());
	ReadRawData(rxData, len);
	WriteConv(CONV_SET);
}

//---------------------------------------------------
void ADS8691::SendCommands(void)
{
	Write((uint8_t *) Buf, DeviceNumber * 4);
}

// Функция подготовки команды
uint8_t ADS8691::CmdPrepare(uint8_t deviceNumver, uint32_t cmd, uint16_t adr, uint16_t data)
{
	uint32_t tmp = cmd | (adr << 16) | data;
	uint8_t *ptr = (uint8_t *) &Buf[deviceNumver];

	ptr[0] = ((tmp & 0xFF000000) >> 24);
	ptr[1] = ((tmp & 0x00FF0000) >> 16);
	ptr[2] = ((tmp & 0x0000FF00) >> 8);
	ptr[3] = ((tmp & 0x000000FF));

	return sizeof(uint32_t);
}

//Функция обработки результата
void ADS8691::HandleResult(int32_t *output)
{
	int32_t result = 0;
	uint8_t pos = 0;
	uint8_t *tmp_buf = (uint8_t *) Buf;
	for (uint8_t i = 0; (i < DeviceNumber * 4); i+=4)
	{

		result = (((tmp_buf[i] << 16) | (tmp_buf[i+1] << 8) | (tmp_buf[i+2])) >> 6) - 131072;

		output[pos] = result;
		pos++;
	}
}

//Функция обработки результата
void ADS8691::HandleResult(void)
{
	int32_t *output = (int32_t *)Buf;

	HandleResult(output);

	ConversionComplete(output);
}


// Получение указателя на буфер
uint32_t* ADS8691::GetBuf(void)
{
	return Buf;
}

























