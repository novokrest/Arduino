#include "DesCryptor.h"
#include <stdlib.h>
#include <cstring>
#include <rpc/des_crypt.h>

int des_cryptor_encrypt(DesKey key, const DesBlock inBlock, DesBlock encryptedBlock)
{
    for (size_t i = 0; i < sizeof(encryptedBlock); ++i) {
        encryptedBlock[i] = inBlock[i];
    }

    int res = ecb_crypt((char *)key, encryptedBlock, sizeof(encryptedBlock), DES_ENCRYPT);
    if (res == DESERR_NONE || res == DESERR_NOHWDEVICE) {
        return DES_CRYPTOR_DECRYPT_SUCCESS;
    }

    return DES_FAILED(res) == 1 ? DES_CRYPTOR_DECRYPT_SUCCESS : DES_CRYPTOR_DECRYPT_FAIL;
}

int des_cryptor_decrypt(DesKey key, const DesBlock encryptedBlock, DesBlock decryptedBlock)
{
    for (size_t i = 0; i < sizeof(encryptedBlock); ++i) {
        decryptedBlock[i] = encryptedBlock[i];
    }

    int res = ecb_crypt((char *)key, decryptedBlock, sizeof(decryptedBlock), DES_DECRYPT);
    if (res == DESERR_NONE || res == DESERR_NOHWDEVICE) {
        return DES_CRYPTOR_DECRYPT_SUCCESS;
    }

    return DES_FAILED(res) == 1 ? DES_CRYPTOR_DECRYPT_SUCCESS : DES_CRYPTOR_DECRYPT_FAIL;
}

DesCryptor::DesCryptor()
{
    DesKey key = {1,2,3,4,5,6,7,8};
    memcpy(key_, key, sizeof(DesKey));
}

DesCryptor::~DesCryptor()
{

}

void DesCryptor::Encrypt(const Data &data, Data &encryptedData)
{
    DesBlock inBlock, outBlock;

    size_t dataSize = data.size();
    size_t encryptedSize = dataSize % sizeof(DesBlock)
            ? (dataSize / sizeof(DesBlock)) * sizeof(DesBlock) + sizeof(DesBlock)
            : dataSize;
    encryptedData.resize(encryptedSize);

    for (size_t i = 0; i < encryptedSize; i += sizeof(DesBlock)) {
        size_t rest = dataSize - i;
        size_t count = rest < sizeof(DesBlock) ? rest : sizeof(DesBlock);
        memset(inBlock, 0, sizeof(inBlock));
        memcpy(inBlock, &data[i], count);
        des_cryptor_encrypt(key_, inBlock, outBlock);
        memcpy(&encryptedData[i], outBlock, sizeof(outBlock));
    }

    encryptedData.push_back(dataSize);
}

void DesCryptor::Decrypt(const Data &data, Data &decryptedData)
{
    DesBlock inBlock, outBlock;

    size_t decryptedSize = data.back();
    decryptedData.resize(decryptedSize);

    for (size_t i = 0; i < decryptedSize; i += sizeof(DesBlock)) {
        size_t rest = decryptedSize - i;
        size_t count = rest < sizeof(DesBlock) ? rest : sizeof(DesBlock);
        memset(inBlock, 0, sizeof(inBlock));
        memcpy(inBlock, &data[i], sizeof(inBlock));
        des_cryptor_decrypt(key_, inBlock, outBlock);
        memcpy(&decryptedData[i], outBlock, count);
    }
}

#include "Utils.h"
#include <iostream>

void DesCryptorTester::DecryptEncryptTest()
{
    const Data data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Data: " << Utils::ToString(data) << std::endl;

    Data encryptedData, decryptedData;
    DesCryptor cryptor_;

    cryptor_.Encrypt(data, encryptedData);
    std::cout << "Encrypted: " << Utils::ToString(encryptedData) << std::endl;

    cryptor_.Decrypt(encryptedData, decryptedData);
    std::cout << "Decrypted: " << Utils::ToString(decryptedData) << std::endl;
}
