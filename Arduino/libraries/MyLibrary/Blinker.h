#ifndef __BLINKER__
#define __BLINKER__

/**
  * Cause LED blinking (default #13)
 */

class Blinker
{
    int led_;
	int status_;
    
	void setStatus(int status);

public:
    Blinker();
    
    void blink();
	void error();

	void turnLightOn();
	void turnLightOff();
};

#endif //__BLINKER__