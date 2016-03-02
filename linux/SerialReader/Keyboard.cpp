#include "Keyboard.h"
#include "Utils.h"
#include <iostream>

Keyboard::Keyboard()
{

}

void Keyboard::NotifyAboutData(const Data &data)
{
    std::cout << "Keyboard: " << Utils::ToCharString(data)
              << " ( " << Utils::ToString(data) << " )" << std::endl;
}

void KeyboardTests::NotifyAboutDataTest()
{
    const Data data = {(char)131, (char)80, (char)131};
    Keyboard keyboard;

    keyboard.NotifyAboutData(data);
}
