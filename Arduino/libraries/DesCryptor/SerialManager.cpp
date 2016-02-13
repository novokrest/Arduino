#include "SerialManager.h"

SerialManager::SerialManager()
{

}

SerialManager& SerialManager::Instance()
{
    static SerialManager manager;
    return manager;
}

byte SerialManager::ReadByte() const
{
    while (Serial.available() <= 0) {}
    return (byte)Serial.read();
}

void SerialManager::WriteByte(byte bbyte) const
{
    Serial.write(bbyte);
}

void SerialManager::WriteBytes(const byte *bytes, byte count) const
{
    Serial.write(bytes, count);
}
