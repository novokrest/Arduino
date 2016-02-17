#ifndef _DES_CRYPTOR_H_
#define _DES_CRYPTOR_H_

#include "Common.h"

typedef char DesKey[8];
typedef char DesBlock[8];

class IDataCryptor
{
public:
    virtual ~IDataCryptor() {}

    virtual void Decrypt(const Data& ecnryptedData, Data& data) = 0;
    virtual void Encrypt(const Data& data, Data& encryptedData) = 0;
};

class DesCryptor : public IDataCryptor
{
    DesKey key_;

public:
    DesCryptor();
    ~DesCryptor() override;

    void Decrypt(const Data& ecnryptedData, Data& data) override;
    void Encrypt(const Data& data, Data& encryptedData) override;
};

#endif /* _DES_CRYPTOR_H_ */
