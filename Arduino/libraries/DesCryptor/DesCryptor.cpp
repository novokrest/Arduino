#include "DesCryptor.h"
#include "Des.h"

void DesCryptor::Encrypt(char inbuf[8], char outbuf[8], char key[8])
{
    des_enc((void *)outbuf, (const void *)inbuf, (const void *)key);
}

void DesCryptor::Decrypt(char inbuf[8], char outbuf[8], char key[8])
{
    des_dec((void *)outbuf, (const void *)inbuf, (const void *)key);
}