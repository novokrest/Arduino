#ifndef __BLINKER__
#define __BLINKER__

/**
  * Cause LED blinking (default #13)
 */

class Blinker
{
    int led_;
    int highMs_;
    int lowMs_;
    
public:
    Blinker();
    Blinker(int led);
    
    void blink();
};

#endif //__BLINKER__