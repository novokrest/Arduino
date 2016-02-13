#ifndef COMMON_H
#define COMMON_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <StandardCplusplus.h>
#include <vector>

typedef std::vector<byte> Data;

#endif // COMMON_H
