/*
 * tsm.c
 *
 *  Created on: Dec 8, 2020
 *      Author: serge
 */

#include <sequencer.hpp>


uint32_t Sequencer::Tick;


// Получение значения таймера планировщика
uint32_t Sequencer::GetTick(void)
{
	return Tick;
}

// Инкремент таймера планировщика
void Sequencer::IncTick(void)
{
	Tick++;
}

// Добавить задачу
int8_t Sequencer::AddTask(Task *task)
{
	if (TaskCount < SEQUENCER_MAX_TASK_CNT)
	{
		Tasks[TaskCount] = task;
		TaskCount++;
		return 0;
	}

	return -1;
}

// Добавление задачи с инициализацией функции и периода
int8_t Sequencer::AddTask(Task *task, void (*pf)(void *arg), uint32_t period)
{
	task->SetFunction(pf);
	task->SetPeriod(period);
	return AddTask(task);
}

// Добавление задачи с инициализацией функции, периода и аргумента
int8_t Sequencer::AddTask(Task *task, void (*pf)(void *arg), void *arg, uint32_t period)
{
	task->SetFunction(pf);
	task->SetPeriod(period);
	task->SetArg(arg);
	return AddTask(task);
}


// Удалчение задачи
int8_t Sequencer::RemoveTask(Task *task)
{
	bool status = false;

	for (uint32_t i = 0; i < TaskCount; i++)
	{
		if (Tasks[i] == task)
		{
			if (TaskCount > 0)
			{
				TaskCount--;
			}

			status = true;
		}

		if (status)
		{
			Tasks[i] = Tasks[i + 1];
		}
	}

	if (status)
	{
		return 0;
	}

	return -1;
}

// Обработка задачи
void Sequencer::Task::Proccess(void)
{
	if (!(State & ACTIVATED))
	{
		return;
	}


	if (((GetTick() - TickStart) >= Period) || (State == RUN_IMMEDIATELY_REQUEST))
	{
		if (Pf != nullptr)
		{
			Pf(Arg);
		}

		TickStart = GetTick();

		if (State & SINGLE_LAUNCH)
		{
			StopExecuting();
		}
	}
}



// Процесс планировщика
void Sequencer::Run(void)
{
	for (uint32_t i = 0; i < TaskCount; i++)
	{
		Tasks[i]->Proccess();
	}
}


