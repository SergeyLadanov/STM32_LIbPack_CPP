/*
 * tsm.c
 *
 *  Created on: Dec 8, 2020
 *      Author: serge
 */

#include <tsm.hpp>


uint32_t TSM::Tick;
//----------------------------------------------------------------
uint32_t TSM::GetTick(void)
{
	return Tick;
}
//-----------------------------------------------------------------
void TSM::IncTick(void)
{
	Tick++;
}
//------------------------------------------------------------------
void TSM::Handle(uint32_t period, void *arg, TSM_Status (*pf)(void *arg))
{
	uint32_t wait = period;
	TSM_Status status;

	  if (Status == 0)
	  {
		  TickStart = GetTick();
		  Status = TSM_BUSY;
	  }
//---------------------------------------------------
	  if (wait < HAL_MAX_DELAY)
	  {
		  wait++;
	  }
//------------------------------------------------------
	  if (((GetTick() - TickStart) >= wait) || (Status == TSM_EXCEPT))
	  {
		  status = pf(arg);
		  if (status == TSM_OK)
		  {
			  TickStart = GetTick();
			  Status = TSM_BUSY;
		  }
		  else if (status == TSM_ERR)
		  {
			  TickStart = GetTick();
			  Status = TSM_ERR;
		  }
	  }
}
//--------------------------------------------------------
void TSM::Execute(void)
{
	Status = TSM_EXCEPT;
}
