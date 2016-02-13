#ifndef DATA_H
#define DATA_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class Data
{
    byte *data_;
    byte size_;

public:
    Data();
    Data(byte *data, byte size);
    Data(const Data& data);
    Data(const Data&& data);

    byte operator[](byte index);
};

#endif // DATA_H
