#ifndef XORUTILS

#define XORUTILS
#define BYTE_SIZE 256

unsigned char* encryptRepeatingXOR(unsigned char *buf, unsigned char *key);

double scoreText(unsigned char *text, int tlen, double* freqMap);

unsigned char* singleXOR(unsigned char* buf, unsigned char mask, int blen);

unsigned char* highScore(unsigned char *text, int tlen, double* freqMap, unsigned char* key, double* score);
#endif
