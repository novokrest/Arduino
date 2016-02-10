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
};


class UtilsTests
{
public:
    static void ToStringTest();
};

#endif // UTILS_H
