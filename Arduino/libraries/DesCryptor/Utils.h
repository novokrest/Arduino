#ifndef UTILS_H
#define UTILS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class Utils
{
    Utils() = delete;
    Utils(const Utils&) = delete;
    Utils(const Utils&&) = delete;

public:
    static void Blink();
    static void RepeatedBlink(byte count);
    static void ReportErrorIfFalse(bool b);
    static void ReportErrorIfNotZero(byte b);

    static void ReportSpecialErrorIfFalse(bool b);
};

#endif // UTILS_H
