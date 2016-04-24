#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <set>
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

#define KEYBOARD_BOOT_PROTOCOL_MESSAGE_SIZE 8
#define MOUSE_BOOT_PROTOCOL_MESSAGE_SIZE    8
#define MOUSE_LEFT_BUTTON_MASK         ((1) & 0xFF)
#define MOUSE_RIGHT_BUTTON_MASK   	 ((1 << 1) & 0xFF)
#define MOUSE_MIDDLE_BUTTON_MASK       ((1 << 2) & 0xFF)

typedef std::vector<unsigned char> Data;
typedef std::vector<uint8_t> Uint8Vec;
typedef uint8_t KeyCode;

typedef Uint8Vec KeyCodesVec;

enum class KeyboardKey;
typedef std::set<KeyboardKey> KeySet;

#endif // COMMON_H
