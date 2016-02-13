#include "DesCryptor.h"
#include "Des.h"

static void des_cryptor_encrypt(char key[8], char inbuf[8], char outbuf[8])
{
    des_enc((void *)outbuf, (const void *)inbuf, (const void *)key);
}

static void des_cryptor_decrypt(char key[8], char inbuf[8], char outbuf[8])
{
    des_dec((void *)outbuf, (const void *)inbuf, (const void *)key);
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
