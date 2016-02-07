#ifndef DESCRYPTOR_H
#define DESCRYPTOR_H

#include <cstdlib>

#define DES_CRYPTOR_ENCRYPT_SUCCESS 0
#define DES_CRYPTOR_ENCRYPT_FAIL    1
#define DES_CRYPTOR_DECRYPT_SUCCESS 0
#define DES_CRYPTOR_DECRYPT_FAIL    1

typedef char const DesKey[8];
typedef char DesBlock[8];
typedef int DesCryptorEncryptResult;
typedef int DesCryptorDecryptResult;

int des_cryptor_encrypt(DesKey key, const DesBlock inBlock, DesBlock encyptedBlock);
int des_cryptor_decrypt(DesKey key, const DesBlock encryptedBlock, DesBlock decryptedBlock);

#endif // DESCRYPTOR_H
