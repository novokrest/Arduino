#ifndef UTILS_H
#define UTILS_H

#include "common.h"
#include <string>

class Utils
{
private:
    Utils() = delete;
    Utils(const Utils &utils) = delete;
    Utils& operator=(const Utils &utils) = delete;

public:
    static std::string ToString(Data const &data);
    static std::string ToCharString(const Data& data);
    static void ToData(const std::string& str, Data& data);
    static Data ToData(const std::string& str);
};


class UtilsTests
{
public:
    static void ToStringTest();
    static void ToCharStringTest();
    static void ToDataTest();
};

#endif // UTILS_H
