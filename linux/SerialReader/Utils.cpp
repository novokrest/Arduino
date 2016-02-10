#include "Utils.h"
#include <sstream>
#include <cassert>

std::string Utils::ToString(Data const &data)
{
    std::stringstream ss;
    for (auto const &p: data) {
        ss << p << " ";
    }
    return ss.str();
}

void UtilsTests::ToStringTest()
{
    const Data data = {0, 1, 2, 3, 4, 5};
    const std::string dataStr = "0 1 2 3 4 5 ";

    std::string str = Utils::ToString(data);

    assert(Utils::ToString(data) == dataStr);
}
