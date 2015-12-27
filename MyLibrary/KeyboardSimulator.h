#ifndef __KEYBOARD_MANAGER__
#define __KEYBOARD_MANAGER__

#include <Max3421e.h>
#include <Usb.h>
#include "Blinker.h"

class KeyboardSimulator
{
    Blinker _blinker;
    
    bool _numLock;
    bool _capsLock;
    bool _scrollLock;
    
    MAX3421E _max;
    USB _usb;
    
    char _lastKeys[8];
    
    void KeyboardInit();
    int KeyboardPoll();
    bool IsNewKey(byte key);
    void BlinkError();
    
    void ReadPressedKeys();
    void SendPressedKeys();
    
public:
    KeyboardSimulator();
    
    void Run();
};


#endif //__KEYBOARD_MANAGER__