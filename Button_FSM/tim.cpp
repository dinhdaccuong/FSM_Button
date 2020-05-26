#include "tim.h"

void timerInit(TimerT* timerInstance, TimerMode timerMode, timerHandler timerHander, unsigned long timeInit = 1000)
{
	timerInstance->time.setCallback(timerHander);
	timerInstance->timerMode = timerMode;

	if (timerInstance->timerMode == TIMER_MODE_SINGLE_SHOT)
	{
		timerInstance->time.setInterval(timeInit);
	}
	timerInstance->time.stop();
}

/*void setInterval(TimerT* timerInstance, unsigned long interval)
{
	timerInstance->time.stop();
	if (timerInstance->timerMode == TIMER_MODE_REPREAT)
	{
		timerInstance->time.setInterval(interval);
	}
}*/


void timerStart(TimerT* timerInstance, unsigned long time)
{
	timerInstance->time.stop();
	timerInstance->time.reset();

	if (timerInstance->timerMode == TIMER_MODE_SINGLE_SHOT)
	{
		timerInstance->time.setTimeout(time);
	}
	else
	{
		timerInstance->time.setInterval(time);
	}
	
	timerInstance->time.start();
}

void timerStop(TimerT* timerInstance)
{
	timerInstance->time.stop();
}

void timerUpdate(TimerT* timerInstance)
{
	timerInstance->time.update();
}