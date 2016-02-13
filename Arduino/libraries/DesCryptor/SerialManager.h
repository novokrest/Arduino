#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class SerialManager
{
    SerialManager();
    SerialManager(const SerialManager&) = delete;
    SerialManager(const SerialManager&&) = delete;

public:
    static SerialManager& Instance();

    byte ReadByte() const;
    void WriteByte(byte bbyte) const;
    void WriteBytes(const byte *bytes, byte count) const;
};

#endif // SERIALMANAGER_H
