#ifndef XORUTILS

#define XORUTILS
#define BYTE_SIZE 256

/**
 * encrypts str with a the repeating XOR key key.
 * Both str and key must be null terminated
 */
char* encryptRepeatingXOR(char *str, char *key);

/**
 * decrypts the byte buffer bytes that is blen bytes long using the frequency
 * map freqMap, stores the value of the highest scoring key in *key and the
 * highest scoring value in *score
 */
char* crackXOR(char *bytes, int blen, double* freqMap, char* key, double* score);
#endif
