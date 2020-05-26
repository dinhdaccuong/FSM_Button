#pragma once
#ifndef __TIM_H__
#define __TIM_H__

#include "timer.h"

typedef enum
{
	TIMER_MODE_SINGLE_SHOT,
	TIMER_MODE_REPREAT
} TimerMode;

typedef struct
{
	Timer time;
	TimerMode timerMode;
} TimerT;					// Timer Type

#define APP_TIM_DEF(timer)						\
TimerT timer									\

typedef void(*timerHandler)();

void timerInit(TimerT* timerInstance, TimerMode timerMode, timerHandler timerHander, unsigned long timeOut = 1000);

void timerStart(TimerT* timeInstance, unsigned long time);

void timerStop(TimerT* timerInstance);

void timerUpdate(TimerT* timerInstance);

#endif // !__TIMER_H__
