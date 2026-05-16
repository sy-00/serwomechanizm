#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"

#define DetectorOut_bm 1<<10

enum DetectorState {ACTIVE, INACTIVE};
enum ServoState {CALLIB, IDLE, IN_PROGRESS};

struct Servo
{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
	unsigned int uiOffset; //tym przesuwam punkt zerowy
	unsigned char ucOffsetActive; //aktywowac czy nie
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
				sServo.uiCurrentPosition = 0;
				//sServo.uiCurrentPosition = 0;
				//sServo.uiDesiredPosition = 0;
				//sServo.eState = IDLE;                    ------te przed zmiana
				if(sServo.ucOffsetActive == 1)
				{
					// jesli mamy offset to idziemy do niego w kalibracji
					sServo.uiDesiredPosition = sServo.uiOffset;
					sServo.eState = IN_PROGRESS;
				}
				else
				{
					// brak offsetu - kalibracja noramlnie nad detektor
					sServo.uiDesiredPosition = 0;
					sServo.eState = IDLE;
				}
			}
			break;
			
		case IDLE:
			if(sServo.uiCurrentPosition != sServo.uiDesiredPosition)
			{
				sServo.eState = IN_PROGRESS;
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
			//warunek zakonczenia ruchu
			if(sServo.uiCurrentPosition == sServo.uiDesiredPosition)
			{
				//robie ze teraz offset jest nowym zerem
				if(sServo.ucOffsetActive == 1)
				{
					sServo.uiCurrentPosition = 0;
					sServo.uiDesiredPosition = 0;
					sServo.ucOffsetActive = 0; //usuwanie aktywacji offsetu
				}
				sServo.eState = IDLE;
			}
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency)
{
	LedInit();
	DetectorInit();
	sServo.eState = CALLIB;
	sServo.uiOffset = 0; //trzeba zainicjalizowac
	sServo.ucOffsetActive = 0; //to tez
	Timer0Interrupts_Init(1000000/uiServoFrequency, &Automat);
}

//funkcja w argumencie przyjmuje offseta zamiast void
void ServoCallib(unsigned int uiOffset)
{
	//tu jest tak duzo bo inaczej main za szybko resetuje stan
	if(sServo.eState != CALLIB && sServo.ucOffsetActive == 0)
	{
		sServo.uiOffset = uiOffset;
		sServo.ucOffsetActive = (uiOffset > 0) ? 1 : 0; //skrocony if else do poprawy pewnie ale mniej miejsca zajmuje
		sServo.eState = CALLIB;
	}
}

void ServoGoTo(unsigned int uiPosition)
{
	//tu zeby sie nie ruszalo poki sie nie skalibruje
	if(sServo.eState != CALLIB && sServo.ucOffsetActive == 0)
	{
		sServo.uiDesiredPosition = uiPosition;
	}
}
