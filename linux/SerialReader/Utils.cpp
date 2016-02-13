#include "Utils.h"
#include <sstream>
#include <cassert>

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
