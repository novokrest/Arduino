#ifndef PC_COMMUNICATOR_H
#define PC_COMMUNICATOR_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <StandardCplusplus.h>
#include <vector>
#include "DesCryptor.h"

typedef std::vector<byte> Data;

class IPcCommunicator
{
public:
    virtual ~IPcCommunicator() {}

    virtual byte Open() = 0;
    virtual byte Read(Data& data) = 0;
    virtual byte Write(const Data& data) = 0;
};

class PcCommunicator : public IPcCommunicator
{
    void OpenPcSerial();

    void Write(const byte *data, byte size);

public:
    byte Open() override;
    byte Read(Data& data) override;
    byte Write(const Data& data) override;
};

class EncryptedPcCommunicator : public IPcCommunicator
{
    IPcCommunicator *communicator_;
    IDataCryptor *cryptor_;

public:
    EncryptedPcCommunicator();
    ~EncryptedPcCommunicator() override;

    byte Open() override;
    byte Read(Data& data) override;
    byte Write(const Data& data) override;

};

class PcSession
{
    IPcCommunicator *communicator_;

    void ShakeHands();
    void StartLoop();

public:
    PcSession();
    ~PcSession();

    void Run();
};

#endif /* PC_COMMUNICATOR_H */
