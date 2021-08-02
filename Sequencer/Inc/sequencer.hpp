/*
 * tsm.h
 *
 *  Created on: Dec 8, 2020
 *      Author: serge
 *      TSM - Timer State Machine
 */

#ifndef TSM_INC_TSM_HPP_
#define TSM_INC_TSM_HPP_


#include <cstdint>


class Sequencer{

public:
	class Task
	{
	private:

		// Состояния
		typedef enum{
			ACTIVATED = (1 << 0),
			SINGLE_LAUNCH = (1 << 1),
			RUN_IMMEDIATELY_REQUEST = (1 << 2)
		}TaskStates;

		void (*Pf)(void *arg) = nullptr;
		uint8_t State = ACTIVATED;
		uint32_t TickStart = 0;
		uint32_t Period = 0;
		void *Arg = nullptr;
	public:

		inline Task(void)
		{

		}

		inline Task(void (*pf)(void *arg), uint32_t period)
		{
			Pf = pf;
			Period = period;
		}
		inline Task(void (*pf)(void *arg), uint32_t period, void *arg)
		{
			Pf = pf;
			Period = period;
			Arg = arg;
		}

		inline void SetFunction(void (*pf)(void *arg))
		{
			Pf = pf;
		}

		inline void SetPeriod(uint32_t period)
		{
			Period = period;
		}

		inline uint32_t GetPeriod(void)
		{
			return Period;
		}

		inline void SetArg(void *arg)
		{
			Arg = arg;
		}

		inline void *GetArg(void)
		{
			return Arg;
		}

		void SingleLaunchOn(void)
		{
			State |= SINGLE_LAUNCH;
			State &= ~ACTIVATED;
		}

		void SingleLaunchOff(void)
		{
			State &= ~SINGLE_LAUNCH;
			State |= ACTIVATED;
		}

		void StopExecuting(void)
		{
			State &= ~ACTIVATED;
		}

		void StartExecuting(void)
		{
			State |= ACTIVATED;
		}

		void Execute(void)
		{
			State |= RUN_IMMEDIATELY_REQUEST;
			State |= ACTIVATED;
		}

		bool IsActivated(void)
		{
			return (State & ACTIVATED);
		}

		bool InSingleLaunchMode(void)
		{
			return (State & SINGLE_LAUNCH);
		}

		void Proccess(void);

	};
private:

	Task **Tasks = nullptr;
	uint32_t TaskCount = 0;
	uint32_t Length = 0;

	void HandleTask(Task *task);
	static uint32_t Tick;
	static uint32_t GetTick(void);

public:
	static constexpr uint32_t GetBufferSize(uint32_t task_count)
	{
		return (task_count * sizeof(Task *));
	}
	
	// Конструктор с динамической инициализацией
	Sequencer(uint32_t task_count)
	{
		Tasks = new Task*[task_count];
		Length = task_count;
	}
	
	// Конструктор со статической инициализацией
	Sequencer(void *buf, uint32_t task_count)
	{
		Tasks = (Task**)buf;
		Length = task_count;
	}
	
	// Освобождение ресурсов после статической инициализации
	void Dispose(void)
	{
		delete[] Tasks;
		Length = 0;
	}
	
	static void IncTick(void);
	void Run(void);
	int8_t AddTask(Task *task);
	int8_t AddTask(Task *task, void (*pf)(void *arg), uint32_t period);
	int8_t AddTask(Task *task, void (*pf)(void *arg), void *arg, uint32_t period);
	int8_t RemoveTask(Task *task);
};

#endif /* TSM_INC_TSM_HPP_ */
