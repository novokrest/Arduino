#ifndef ARDUINO_COMMUNICATOR_H
#define ARDUINO_COMMUNICATOR_H

#include "common.h"
#include "DataObserver.h"
#include"DesCryptor.h"

class IArduinoCommunicator
{
public:
    virtual ~IArduinoCommunicator() {}

    virtual void Open() = 0;
    virtual void Read(Data &data) = 0;
    virtual void Write(const Data& data) = 0;
};

class ArduinoCommunicator : public IArduinoCommunicator
{
    std::string serialPort_;
    int fd_;

    void OpenArduinoSerial();
    void CloseArduinoSerial();

    void ReadAndCheckMessage(const std::string& message);
    void WriteMessage(const std::string& message);

    void ReadData(Data& data);
    void WriteData(const Data& data);

public:
    ArduinoCommunicator();
    ~ArduinoCommunicator() override;

    void WriteSymbol(char symbol);
    void ReadSymbol(char *symbol);

    void Open() override;
    void Read(Data& data) override;
    void Write(const Data &data) override;
};

class EncryptedArduinoCommunicator : public IArduinoCommunicator
{
    IArduinoCommunicator *communicator_;
    IDataCryptor *cryptor_;

public:
    EncryptedArduinoCommunicator();
    ~EncryptedArduinoCommunicator() override;

    void Open() override;
    void Read(Data& data) override;
    void Write(const Data& data) override;
};

class ArduinoSession : public DataNotifier
{
    IArduinoCommunicator *communicator_;

    void WriteMessage(const std::string& message);
    std::string ReadMessage();
    void ReadAndCheckMessage(const std::string& expected);

    void ShakeHands();
    void StartLoop();

public:
    ArduinoSession();
    ~ArduinoSession();

    void Run();
};

#endif // ARDUINO_COMMUNICATOR_H
