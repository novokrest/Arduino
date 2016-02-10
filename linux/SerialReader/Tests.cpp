#include "Tests.h"
#include "Utils.h"
#include "Keyboard.h"
#include "ArduinoCommunicator.h"

void Tests::RunTests()
{
    UtilsTests::ToStringTest();
    KeyboardTests::NotifyAboutDataTest();
    KeyboardNotificationTest();
}

void Tests::KeyboardNotificationTest()
{
    Keyboard keyboard;
    DataNotifier notifier;

    keyboard.SubscribeOn(notifier);
    notifier.NotifyAboutData({1, 2, 3, 4, 5});
}
