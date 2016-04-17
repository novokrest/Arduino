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
    static std::string ToString(const uint8_t* data, size_t size);
    static std::string ToString(Data const &data);
    static std::string ToCharString(const Data& data);
	static std::string CharsToIntString(unsigned char *data, size_t size);
	static std::string ToHexString(const Data& data, const std::string& delimiter);
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
