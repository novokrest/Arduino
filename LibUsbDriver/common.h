#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <cstdint>

#define ARDUINO_START_DATA_KEY  131
#define ARDUINO_END_DATA_KEY    131

#define ARDUINO_START_DATA_KEY_SYMBOL  (uint8_t)131
#define ARDUINO_END_DATA_KEY_SYMBOL    (uint8_t)131

#define ARDUINO_START_DATA_KEY_INT  (int)131
#define ARDUINO_END_DATA_KEY_INT    (int)131


typedef std::vector<unsigned char> Data;

#endif // COMMON_H
