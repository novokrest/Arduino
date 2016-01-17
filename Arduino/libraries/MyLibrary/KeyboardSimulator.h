#ifndef __KEYBOARD_MANAGER__
#define __KEYBOARD_MANAGER__

#include <Max3421e.h>
#include <Usb.h>

class KeyboardSimulator
{
    //Blinker _blinker;
    //
    bool _numLock;
    bool _capsLock;
    bool _scrollLock;

	MAX3421E& _max;
	USB& _usb;
    
    char _lastKeys[8];
    
    void KeyboardInit();
    int KeyboardPoll();
    bool IsNewKey(byte key);
    //void BlinkError();
    
    //void ReadPressedKeys();
    //void SendPressedKeys();
    
public:
	KeyboardSimulator(MAX3421E &max, USB &usb);
    
	void Init();
    void Run();
};


#endif //__KEYBOARD_MANAGER__