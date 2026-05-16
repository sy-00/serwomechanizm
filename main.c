#include "keyboard.h"
#include "servo.h"

int main ()
{
	KeyboardInit();
	ServoInit(20);
	
	while(1)
	{
		switch(eKeyboardRead())
		{
			case BUTTON_0:
				ServoCallib();
				break;
			case RELASED:
				break;
		}
	}
}
