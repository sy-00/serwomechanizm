#include "keyboard.h"
#include "servo.h"


int main ()
{
	KeyboardInit();
	ServoInit(67);

	while(1)
	{
		
		switch(eKeyboardRead())
		{
			case BUTTON_0:
				ServoCallib(6); //do argyumentu teraz dajemy offset (zamiast nic)
				break;
			case BUTTON_1:
				ServoGoTo(12);
				break;
			case BUTTON_2:
				ServoGoTo(24);
				break;
			case BUTTON_3:
				ServoGoTo(36);
				break;
			case RELASED:
				break;
		}
	}
}
