#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"
#include "DataObserver.h"

class Keyboard : public DataObserver
{
public:
    Keyboard();

    void NotifyAboutData(const Data &data) override;
};

class KeyboardTests
{
    KeyboardTests() = delete;
    KeyboardTests(const KeyboardTests&) = delete;
    KeyboardTests& operator=(const KeyboardTests&) = delete;

public:
    static void NotifyAboutDataTest();
};

#endif // KEYBOARD_H
