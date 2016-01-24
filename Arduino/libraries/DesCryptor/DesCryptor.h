#ifndef _DES_CRYPTOR_H_
#define _DES_CRYPTOR_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class DesCryptor
{
public:
    void Encrypt(char inbuf[8], char outbuf[8], char key[8]);
    void Decrypt(char inbuf[8], char outbuf[8], char key[8]);
};

#endif /* _DES_CRYPTOR_H_ */