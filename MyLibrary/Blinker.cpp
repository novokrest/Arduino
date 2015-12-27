#if defined(ARDUINO) && ARDUINO >= 100
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#include "Blinker.h"


Blinker::Blinker()
    : led_(13)
{
    pinMode(led_, OUTPUT);
}

Blinker::Blinker(int led)
    : led_(led)
{
    pinMode(led_, OUTPUT);
}

void Blinker::blink()
{
    digitalWrite(led_, HIGH);
    delay(150);
    digitalWrite(led_, LOW);
    delay(350);
}