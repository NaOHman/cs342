#ifndef XORUTILS

#define XORUTILS
#define BYTE_SIZE 256

double scoreText(char *text, int tlen, double* freqMap);

char* repeatingXOR(char* buf, char mask, int blen);

char* highScore(char *text, int tlen, double* freqMap, char* key, double* score);
#endif
