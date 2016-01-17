#include "ArduinoDefines.h"
#include "Blinker.h"


Blinker::Blinker()
    : led_(13)
{
    pinMode(led_, OUTPUT);
	setStatus(LOW);
}

void Blinker::setStatus(int status)
{
	if (status_ != status) {
		digitalWrite(led_, status);
		status_ = status;
	}
}

void Blinker::blink()
{
	turnLightOn();
    delay(150);
	turnLightOff();
	delay(150);
}

// ___ . . . . . ___
void Blinker::error()
{
	digitalWrite(led_, HIGH);
	delay(1000);

	for (int i = 0; i < 5; ++i) {
		digitalWrite(led_, LOW);
		delay(150);
		digitalWrite(led_, HIGH);
		delay(150);
	}

	digitalWrite(led_, LOW);
	delay(150);
	
	digitalWrite(led_, HIGH);
	delay(1000);
}

void Blinker::turnLightOn()
{
	setStatus(HIGH);
}

void Blinker::turnLightOff()
{
	setStatus(LOW);
}