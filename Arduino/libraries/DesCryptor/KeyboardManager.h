#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#include <Max3421e.h>
#include <Usb.h>
#include "Common.h"


class KeyboardManager
{
    MAX3421E max_;
    USB usb_;
    EP_RECORD epRecord_[2];    /* endpoint record structure for the keyboard */

    bool numLock_;
    bool capsLock_;
    bool scrollLock_;

    Data lastKey_;

    byte InitKeyboard();
    byte PollKeyboard(Keys& keys);

    bool IsNewKey(byte key);

public:
    KeyboardManager();

    byte WaitForKeyPress(Keys& keys);
};

#endif // KEYBOARDMANAGER_H
