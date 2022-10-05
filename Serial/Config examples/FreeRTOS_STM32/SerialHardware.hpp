/*
 * SerialHardware.hpp
 *
 *  Created on: Oct 4, 2022
 *      Author: serge
 */

#ifndef INC_SERIALHARDWARE_HPP_
#define INC_SERIALHARDWARE_HPP_

#include <Serial.hpp>
#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"


class SerialHardware : public Serial
{
private:
	static constexpr uint32_t LOCK_TIMEOUT = 1500;
	osMutexId_t RxMutex;
	osSemaphoreId_t RxSemaphore;
	osSemaphoreId_t TxSemaphore;

	using Serial::Serial;

public:

	void InitOsAttributes()
	{
		RxMutex = osMutexNew(nullptr);
		RxSemaphore = osSemaphoreNew(1, 1, nullptr);
		TxSemaphore = osSemaphoreNew(1, 1, nullptr);
	}


	uint8_t SendData(uint8_t *data, uint16_t len) override
	{
		UART_HandleTypeDef *UartRef = (UART_HandleTypeDef *) Arg;

		if (!TxLock())
		{
			return 1;
		}
		return HAL_UART_Transmit_DMA(UartRef, data, len);
	}


	void TxCompleteHandle(void)
	{
		TxUnLock();
	}


	bool WaitForData(uint32_t timeout)
	{
		if (Available())
		{
			osSemaphoreAcquire(RxSemaphore, 0);
			return true;
		}

		if (osSemaphoreAcquire(RxSemaphore, timeout) == osOK)
		{
			if (Available())
			{
				return true;
			}
		}

		return false;
	}

private:
	void TimeOutCallBack() override
	{
		osSemaphoreRelease(RxSemaphore);
	}

	void RxLock(void)override
	{
		osMutexAcquire(RxMutex, LOCK_TIMEOUT);
	}

	void RxUnLock(void) override
	{
		osMutexRelease(RxMutex);
	}

	bool TxLock(void)
	{
		if (osSemaphoreAcquire(TxSemaphore, LOCK_TIMEOUT) == osOK)
		{
			return true;
		}

		return false;
	}

	void TxUnLock(void)
	{
		osSemaphoreRelease(TxSemaphore);
	}

	void TimeOutHandle(uint16_t value) override
	{
		osDelay(value);
	}

};



#endif /* INC_SERIALHARDWARE_HPP_ */
