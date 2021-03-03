/*
 * tsm.h
 *
 *  Created on: Dec 8, 2020
 *      Author: serge
 *      TSM - Timer State Machine
 */

#ifndef TSM_INC_TSM_HPP_
#define TSM_INC_TSM_HPP_


#include "main.h"

// Статусы
typedef enum{
	TSM_OK = 0,
	TSM_ERR = 1,
	TSM_BUSY = 2,
	TSM_EXCEPT = 3,
	TSM_NORESULT = 4
}TSM_Status;


class TSM{
private:
	static uint32_t Tick;
	uint8_t Status;
	uint32_t TickStart;
	uint32_t GetTick(void);
public:
	static void IncTick(void);
	void Handle(uint32_t period, void *arg, TSM_Status (*pf)(void *arg));
	void Execute(void);
};

#endif /* TSM_INC_TSM_HPP_ */
