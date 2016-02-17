#include "PcCommunicator.h"
#include "SerialManager.h"
#include "Utils.h"

#define PC_START_KEY         131
#define PC_END_KEY           131
#define ARDUINO_START_KEY    131
#define ARDUINO_END_KEY      131

#define PC_START_KEY_SYMBOL       (byte)PC_START_KEY
#define PC_END_KEY_SYMBOL         (byte)PC_END_KEY
#define ARDUINO_START_KEY_SYMBOL  (byte)ARDUINO_START_KEY
#define ARDUINO_END_KEY_SYMBOL    (byte)ARDUINO_END_KEY


byte PcCommunicator::Read(Data& data)
{
    byte symbol, count;

    symbol = SerialManager::Instance().ReadByte();
    if (symbol != PC_START_KEY_SYMBOL) {
        return -1;
    }

    count = SerialManager::Instance().ReadByte();
    data.resize(count);

    for (byte i = 0; i < count; ++i) {
        symbol = SerialManager::Instance().ReadByte();
        data[i] = symbol;
    }

    symbol = SerialManager::Instance().ReadByte();
    if (symbol != PC_END_KEY_SYMBOL) {
        return -1;
    }

    return 0;
}

void PcCommunicator::Write(const byte *data, byte size)
{
    SerialManager::Instance().WriteByte(ARDUINO_START_KEY_SYMBOL);

    SerialManager::Instance().WriteByte(size);
    SerialManager::Instance().WriteBytes(data, size);

    SerialManager::Instance().WriteByte(ARDUINO_END_KEY_SYMBOL);
}

byte PcCommunicator::Write(const Data &data)
{
    Write(&data.front(), data.size());

    return 0;
}

void PcCommunicator::OpenPcSerial()
{
    Serial.begin(9600);
}

byte PcCommunicator::Open()
{
    OpenPcSerial();

    return 0;
}


EncryptedPcCommunicator::EncryptedPcCommunicator()
    : communicator_(new PcCommunicator()), cryptor_(new DesCryptor())
{
}

EncryptedPcCommunicator::~EncryptedPcCommunicator()
{
    delete communicator_;
    delete cryptor_;
}

byte EncryptedPcCommunicator::Open()
{
    return communicator_->Open();
}

byte EncryptedPcCommunicator::Read(Data &data)
{
    Data encryptedData;

    if (communicator_->Read(encryptedData) != 0) {
        return -1;
    }
    cryptor_->Decrypt(encryptedData, data);

    return 0;
}

byte EncryptedPcCommunicator::Write(const Data &data)
{
    Data encryptedData;

    cryptor_->Encrypt(data, encryptedData);
    communicator_->Write(encryptedData);

    return 0;
}
