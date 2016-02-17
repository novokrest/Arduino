#ifndef PC_COMMUNICATOR_H
#define PC_COMMUNICATOR_H

#include "Common.h"
#include "DesCryptor.h"


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

#endif /* PC_COMMUNICATOR_H */
