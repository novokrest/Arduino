/*
 * main.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: root
 */

#include "Keyboard.h"

int main()
{
	Keyboard keyboard(ArduinoKeyboardDescription);

	keyboard.Start();

	return 0;
}


