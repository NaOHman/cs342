#ifndef XORUTILS

#define XORUTILS
#define BYTE_SIZE 256

char* encryptRepeatingXOR(char *buf, char *key);

double scoreText(char *text, int tlen, double* freqMap);

char* singleXOR(char* buf, char mask, int blen);

char* highScore(char *text, int tlen, double* freqMap, char* key, double* score);
#endif
