#include "Utils.h"

void Utils::Blink()
{
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(100);              // wait for a second
}

void Utils::RepeatedBlink(byte count)
{
    for (byte i = 0; i < count; ++i) {
        Blink();
    }
}

void Utils::ReportErrorIfFalse(bool b)
{
    if (!b) {
        Serial.println("Error");
        //RepeatedBlink(200);
    }
}

void Utils::ReportErrorIfNotZero(byte b)
{
    ReportErrorIfFalse(b == 0);
}

void Utils::ReportSpecialErrorIfFalse(bool b)
{
    if (!b) {
        RepeatedBlink(10);
        delay(1500);
        RepeatedBlink(10);
        delay(1500);
        RepeatedBlink(10);
        delay(1500);
    }
}

bool Utils::Contain(const Data &data, byte e)
{
    for (byte i = 2, len = data.size(); i < len; ++i) {
        if (e == data.at(i)) {
            return true;
        }
    }
    return false;
}
