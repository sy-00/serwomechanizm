#include "keyboard.h"
#include "servo.h"


int main ()
{
	KeyboardInit();
	ServoInit(20);
  	ServoCallib();
	ServoWait();
	ServoGoTo(24);
	ServoWait();
	ServoGoTo(12);

	
	while(1)
	{
		
		switch(eKeyboardRead())
		{
			case BUTTON_0:
				ServoCallib();
				break;
			case BUTTON_1:
				ServoGoTo(12);
				break;
			case BUTTON_2:
				ServoGoTo(24);
				break;
			case BUTTON_3:
				ServoWait();
				break;
			case RELASED:
				break;
		}
	}
}
