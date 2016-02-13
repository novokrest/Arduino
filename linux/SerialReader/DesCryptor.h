#ifndef DESCRYPTOR_H
#define DESCRYPTOR_H

#include "common.h"
#include <cstdlib>

#define DES_CRYPTOR_ENCRYPT_SUCCESS 0
#define DES_CRYPTOR_ENCRYPT_FAIL    1
#define DES_CRYPTOR_DECRYPT_SUCCESS 0
#define DES_CRYPTOR_DECRYPT_FAIL    1

typedef char DesKey[8];
typedef char DesBlock[8];
typedef int DesCryptorEncryptResult;
typedef int DesCryptorDecryptResult;

int des_cryptor_encrypt(DesKey key, const DesBlock inBlock, DesBlock encyptedBlock);
int des_cryptor_decrypt(DesKey key, const DesBlock encryptedBlock, DesBlock decryptedBlock);

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

class DesCryptorTester
{
    DesCryptorTester() = delete;
    DesCryptorTester(const DesCryptorTester&) = delete;
    DesCryptorTester(const DesCryptorTester&&) = delete;

public:
    static void DecryptEncryptTest();
};

#endif // DESCRYPTOR_H
