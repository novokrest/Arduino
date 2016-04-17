#include "Utils.h"
#include <sstream>
#include <cassert>

std::string Utils::ToString(const uint8_t* data, size_t size)
{
	std::stringstream ss;
	for (size_t i = 0; i < size; ++i) {
		ss << (int)data[i] << " ";
	}
	return ss.str();
}

std::string Utils::ToString(Data const &data)
{
    std::stringstream ss;
    for (auto const &p: data) {
        ss << (int)p << " ";
    }
    return ss.str();
}

std::string Utils::ToCharString(const Data &data)
{
    std::stringstream ss;
    for (auto const &p: data) {
        ss << (char)p;
    }
    return ss.str();
}

std::string Utils::CharsToIntString(unsigned char *data, size_t size)
{
	std::stringstream ss;

	ss << '{';
	for (size_t i = 0; i < size; ++i) {
		ss << ' ' << (int)data[i];
	}
	ss << " }";

	return ss.str();
}

std::string Utils::ToHexString(const Data& data, const std::string& delimiter)
{
	char hexStr[8];
	std::stringstream ss;
	for (auto const &p: data) {
		int len = sprintf(hexStr, "%02X", (unsigned int)(p & 0xFF));
		hexStr[len] = '\0';
		ss << hexStr << " ";
	}
	return ss.str();
}

void Utils::ToData(const std::string &str, Data &data)
{
    std::copy(str.begin(), str.end(), std::back_inserter(data));
}

Data Utils::ToData(const std::string &str)
{
    return Data(str.begin(),str.end());
}

void UtilsTests::ToStringTest()
{
    const Data data = {0, 1, 2, 3, 4, 5};
    const std::string dataStr = "0 1 2 3 4 5 ";

    std::string str = Utils::ToString(data);

    assert(str == dataStr);
}

void UtilsTests::ToCharStringTest()
{
    const Data data = {'A', 'B', 'C', 'D', 'E'};
    const std::string expected = "ABCDE";

    std::string actual = Utils::ToCharString(data);

    assert(actual == expected);
}

void UtilsTests::ToDataTest()
{
    const std::string str = "Test";
    const Data expected = {'T', 'e', 's', 't'};

    Data actual = Utils::ToData(str);

    assert(actual == expected);
}
