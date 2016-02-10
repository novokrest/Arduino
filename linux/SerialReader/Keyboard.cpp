#include "Keyboard.h"
#include "Utils.h"
#include <iostream>

Keyboard::Keyboard()
{

}

void Keyboard::NotifyAboutData(const Data &data)
{
    std::cout << "Keyboard: " << Utils::ToString(data) << std::endl;
}

void KeyboardTests::NotifyAboutDataTest()
{
    const Data data = {131, 80, 131};
    Keyboard keyboard;

    keyboard.NotifyAboutData(data);
}
