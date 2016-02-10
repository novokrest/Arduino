#ifndef ARDUINO_COMMUNICATOR_H
#define ARDUINO_COMMUNICATOR_H

#include "common.h"
#include "DataObserver.h"

class ArduinoCommunicator : public DataNotifier
{
    std::string serialPort_;
    int fd_;

    void OpenArduinoSerial();
    void CloseArduinoSerial();
    void ShakeHands();
    void StartReceivingDataLoop();

    void WriteSymbol(char symbol);
    void WriteMessage(const std::string& message);

    void ReadSymbol(char *symbol);
    void ReadData(Data& data);
    void ReadAndCheckMessage(const std::string& message);

public:
    ArduinoCommunicator();
    ~ArduinoCommunicator();

    void Run();
};

#endif // ARDUINO_COMMUNICATOR_H
