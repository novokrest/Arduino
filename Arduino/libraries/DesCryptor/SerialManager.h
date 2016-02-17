#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include "Common.h"

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
