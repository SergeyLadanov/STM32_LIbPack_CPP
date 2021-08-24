/*
 * ads8691_spi.hpp
 *
 *  Created on: 23 авг. 2021 г.
 *      Author: serge
 */

#ifndef ADS8691_SPI_HPP_
#define ADS8691_SPI_HPP_


#include "ads8691.hpp"


class ADS8691_SPI : public ADS8691
{
public:

	ADS8691_SPI(uint8_t device_number = 0, ChannelBuffer *buffer = nullptr, void *arg = nullptr)
		:ADS8691(device_number, buffer, arg)
	{

	}

	virtual void StartContinuousConv(void);
	virtual void StopContinuousConv(void);

private:
	virtual void ConversionComplete(int32_t *data);
	virtual void Delay(uint32_t value);
	virtual void WriteRawData(uint8_t *txData, uint16_t len);
	virtual void ReadRawData(uint8_t *rxData, uint16_t len);
	virtual uint16_t ReadRVS(void);
	virtual void WriteConv(bool state);
};




#endif /* ADS8691_SPI_HPP_ */
