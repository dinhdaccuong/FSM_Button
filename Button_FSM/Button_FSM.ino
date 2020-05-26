#include "tim.h"
#include "bsp.h"

typedef struct
{
	uint8_t fsmStatus;
	uint8_t buttonStatus;
} fsm_t;

#define turnOnClickLed()				digitalWrite(LED_PIN_CLICK, LOW)
#define turnOffClickLed()				digitalWrite(LED_PIN_CLICK, HIGH)
#define turnOnDoubleClickLed()			digitalWrite(LED_PIN_DOUBLE_CLICK, LOW)
#define turnOffDoubleClickLed()			digitalWrite(LED_PIN_DOUBLE_CLICK, HIGH)
#define turnOnHoldPressLed()			digitalWrite(LED_PIN_HOLD_PRESS, LOW)
#define turnOffHoldPressLed()			digitalWrite(LED_PIN_HOLD_PRESS, HIGH)

APP_TIM_DEF(timerCheckClick);
APP_TIM_DEF(timerCheckPress);

void bspInit();
void timerInit();
void allTimerUpdate();
uint8_t buttonReadPin();
void buttonHandler();

void startTimerCheckClick(unsigned long time);
void stopTimerCheckClick();
void timerCheckClickHandler();

void startTimerCheckPress(unsigned long time);
void stopTimerCheckPress();
void timerCheckPressHandler();

void buttonPressActive();
void buttonClickActive();
void buttonDoubleClickActive();

fsm_t fsmInstance;

void fsm();
void fsmInit();


void setup() {
	Serial.begin(115200);
	bspInit();
	timerInit();
	fsmInit();
}



void loop() {
	allTimerUpdate();
}
/**************************************INIT*********************************************/
void bspInit()
{
	pinMode(LED_PIN_CLICK, OUTPUT);
	pinMode(LED_PIN_DOUBLE_CLICK, OUTPUT);
	pinMode(LED_PIN_HOLD_PRESS, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	digitalWrite(LED_PIN_CLICK, HIGH);
	digitalWrite(LED_PIN_DOUBLE_CLICK, HIGH);
	digitalWrite(LED_PIN_HOLD_PRESS, HIGH);

	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonHandler, CHANGE);
}

void timerInit()
{
	timerInit(&timerCheckClick, TIMER_MODE_SINGLE_SHOT, timerCheckClickHandler, 1000);
	timerInit(&timerCheckPress, TIMER_MODE_SINGLE_SHOT, timerCheckPressHandler, 1000);
}

/************************************FUNCTION********************************************/
uint8_t buttonReadPin()
{
	return digitalRead(BUTTON_PIN) ? 1 : 0;
}

void startTimerCheckClick(unsigned long time )
{
	timerStart(&timerCheckClick, time);
}

void stopTimerCheckClick()
{
	timerStop(&timerCheckClick);
}

void startTimerCheckPress(unsigned long time)
{
	timerStart(&timerCheckPress, time);
}
void stopTimerCheckPress()
{
	timerStop(&timerCheckPress);
}


void allTimerUpdate()
{
	timerUpdate(&timerCheckClick);
	timerUpdate(&timerCheckPress);
}

void buttonPressActive()
{
	static uint8_t ledPressStt = 0;
	if (ledPressStt == 0)
	{
		ledPressStt = 1;
		turnOnHoldPressLed();
	}
	else
	{
		ledPressStt = 0;
		turnOffHoldPressLed();
	}
}
void buttonClickActive()
{
	static uint8_t ledClickStt = 0;
	if (ledClickStt == 0)
	{
		ledClickStt = 1;
		turnOnClickLed();
	}
	else
	{
		ledClickStt = 0;
		turnOffClickLed();
	}
}
void buttonDoubleClickActive()
{
	static uint8_t ledDoubleClickStt = 0;
	if (ledDoubleClickStt == 0)
	{
		ledDoubleClickStt = 1;
		turnOnDoubleClickLed();
	}
	else
	{
		ledDoubleClickStt = 0;
		turnOffDoubleClickLed();
	}
}

void fsmInit()
{
	fsmInstance.buttonStatus = 0;
	fsmInstance.fsmStatus = 0;
}
void fsm()
{
	switch (fsmInstance.fsmStatus)
	{
	case 0:	
		if (!fsmInstance.buttonStatus)		// button down
		{
			fsmInstance.fsmStatus = 1;
			startTimerCheckPress(1000);
		}
		break;
	case 1:
		if (fsmInstance.buttonStatus)		// button up
		{
			fsmInstance.fsmStatus = 2;
			stopTimerCheckPress();
			startTimerCheckClick(200);
		}
		break;
	case 2:
		if (!fsmInstance.buttonStatus)
		{
			fsmInstance.fsmStatus = 0;
			stopTimerCheckClick();
			buttonDoubleClickActive();			
		}
		break;
	default:
		break;
	}
}
void buttonHandler()
{
	fsmInstance.buttonStatus = buttonReadPin() ? 1 : 0;
	fsm();
}

void timerCheckClickHandler()
{
	buttonClickActive();
	fsmInstance.fsmStatus = 0;
}

void timerCheckPressHandler()
{	
	buttonPressActive();
	fsmInstance.fsmStatus = 0;
}