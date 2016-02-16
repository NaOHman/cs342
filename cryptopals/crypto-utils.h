#include <stdio.h>

char hexCharToByte(char aHexChar);

char* hexStrToBytes(char* aHexStr, int* aOutLen);

char* bytesToHexStr(char* buf, int blen);

double *readFreqMap(const char* file);
