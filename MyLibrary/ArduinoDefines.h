#ifndef __ARDUINO_DEFINES__
#define __ARDUINO_DEFINES__

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#endif //__ARDUINO_DEFINES__