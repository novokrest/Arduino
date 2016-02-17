#ifndef COMMON_H
#define COMMON_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <StandardCplusplus.h>
#include <vector>

#define KEYBOARD_BUFFER_LENGTH 8

typedef std::vector<byte> Data;
typedef std::vector<Data> Keys;

#endif // COMMON_H
