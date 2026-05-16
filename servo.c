#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"

#define DetectorOut_bm (1<<10)

enum DetectorState {ACTIVE, INACTIVE};
enum ServoState {CALLIB, IDLE}; 

struct Servo
{
	enum ServoState eState;
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
				sServo.eState = IDLE;
			}
			break;

		case IDLE:
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency)
{
	LedInit();
	DetectorInit();
	
	sServo.eState = CALLIB;
	
	Timer0Interrupts_Init(1000000/uiServoFrequency, &Automat);
	
	while(sServo.eState != IDLE)
	{ }
}
