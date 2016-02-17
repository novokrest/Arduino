#ifndef UTILS_H
#define UTILS_H

#include "Common.h"

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

    static bool Contain(const Data& data, byte e);
};

#endif // UTILS_H
