#ifndef XORUTILS

#define XORUTILS
#define BYTE_SIZE 256

/**
 * encrypts str with a the repeating XOR key key.
 * Both str and key must be null terminated
 */
unsigned char* encryptRepeatingXOR(unsigned char *str, unsigned char *key);

//unsigned char* singleXOR(unsigned char* buf, unsigned char mask, int blen);

/**
 * decrypts the byte buffer bytes that is blen bytes long using the frequency
 * map freqMap, stores the value of the highest scoring key in *key and the
 * highest scoring value in *score
 */
unsigned char* crackXOR(unsigned char *bytes, int blen, double* freqMap, unsigned char* key, double* score);
#endif
