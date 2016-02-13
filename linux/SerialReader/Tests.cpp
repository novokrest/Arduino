#include "Tests.h"
#include "Utils.h"
#include "Keyboard.h"
#include "ArduinoCommunicator.h"
#include "DesCryptor.h"

void Tests::RunTests()
{
    UtilsTests::ToStringTest();
    UtilsTests::ToCharStringTest();
    UtilsTests::ToDataTest();

    KeyboardTests::NotifyAboutDataTest();
    KeyboardNotificationTest();
    DesCryptorTester::DecryptEncryptTest();
}

void Tests::KeyboardNotificationTest()
{
    Keyboard keyboard;
    DataNotifier notifier;

    keyboard.SubscribeOn(notifier);
    notifier.NotifyAboutData({1, 2, 3, 4, 5});
}
