#include "DesCryptor.h"
#include <stdlib.h>
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
