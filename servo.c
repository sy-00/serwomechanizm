#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"

#define DetectorOut_bm 1<<10

enum DetectorState {ACTIVE, INACTIVE};
enum ServoState {CALLIB, IDLE, IN_PROGRESS, WAIT}; 

struct Servo
{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
	unsigned int uiWaitCounter;
	unsigned int uiWaitTime;
};

struct Servo sServo;

void DetectorInit()
{
	IO0DIR = IO0DIR & (~(DetectorOut_bm));
}

enum DetectorState eReadDetector()
{
	if((IO0PIN & DetectorOut_bm) == 0)
	{
		return ACTIVE;
	}
	else
	{
		return INACTIVE;
	}
}

void Automat()
{
	switch(sServo.eState)
	{
		case CALLIB:
			if(eReadDetector() == INACTIVE)
			{
				LedStepRight();
				
			}
			else
			{
				sServo.uiDesiredPosition = 0;
				sServo.eState = IDLE;
				sServo.uiCurrentPosition = 0;
			}
			break;

		case IDLE:
			if(sServo.uiCurrentPosition != sServo.uiDesiredPosition)
			{
				sServo.eState = IN_PROGRESS;
			}
			else
			{
			  sServo.eState = IDLE;
			}			
			break;
		case IN_PROGRESS:
			if(sServo.uiCurrentPosition < sServo.uiDesiredPosition)
			{
				LedStepRight();
				sServo.uiCurrentPosition++;
			}
			else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition)
			{
				LedStepLeft();
				sServo.uiCurrentPosition--;
			}
			else if(sServo.uiCurrentPosition == sServo.uiDesiredPosition)
			{
				sServo.eState = IDLE;
			}
			break;
		case WAIT:
			if(sServo.uiWaitCounter < sServo.uiWaitTime)
			{
				sServo.uiWaitCounter++;
			}
			else
			{
				sServo.eState = IN_PROGRESS;
				sServo.uiWaitCounter = 0;
			}
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency)
{
	LedInit();
	DetectorInit();
	sServo.eState = CALLIB;
	sServo.uiWaitCounter = 0;
	Timer0Interrupts_Init(1000000/uiServoFrequency, &Automat);
	while(sServo.eState != IDLE)
	{
	}
}

void ServoCallib(void)
{
	sServo.eState = CALLIB;
	while(sServo.eState == CALLIB)
	{  
	}	
}

void ServoGoTo(unsigned int uiPosition)
{
	sServo.uiDesiredPosition = uiPosition;
	sServo.eState = IN_PROGRESS;	
  while(sServo.eState == IN_PROGRESS)
	{  
	}
}

void ServoWait(void)
{
	sServo.uiWaitTime = 20;
	sServo.uiWaitCounter = 0;
	sServo.eState = WAIT;
	while(sServo.eState == WAIT)
	{
	}
}
