#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "xor-utils.h"

static char mask = 0xF;

char hexCharToByte(char aHexChar) {
  if (aHexChar >= '0' && aHexChar <= '9') {
    return aHexChar - '0';
  }
  if (aHexChar >= 'a' && aHexChar <= 'f') {
    return (aHexChar - ('a' - 10));
  }
  printf("Failure! Invalid hex character.\n");
  exit(1);
}

char* bytesToHexStr(char* buf, int blen) {
    if (!buf || !blen){
        printf("Error Null buffer");
        exit(1);
    }
    char *hx_ptr, *hex = malloc(blen * 2 + 1);
    if (!hex){
        printf("could not malloc new string");
        exit(1);
    }
    for (int i=0; i<blen; i++){
        hx_ptr += sprintf(hx_ptr, "%02x", buf[i]);
    }
    *hx_ptr = '\0';
    return hex;
}

// returns a malloc'd byte buffer, caller frees
char* hexStrToBytes(char* aHexStr, int* aOutLen) {
  // need valid input
  if (!aHexStr || !aOutLen) {
    return NULL;
  }

  // need an even number of hex digits to convert to bytes
  if (strlen(aHexStr) % 2 != 0) {
    return NULL;
  }

  // calculate the length of the output buffer
  *aOutLen = strlen(aHexStr) / 2;
  if (*aOutLen == 0) {
    return NULL;
  }

  // create output buffer
  char *outBuf = malloc(*aOutLen);
  if (!outBuf) {
    return NULL;
  }

  // fill the output buffer
  int inputLen = strlen(aHexStr);
  for (int i = 0; i < inputLen; i = i + 2) {
    char char1 = aHexStr[i];
    char char2 = aHexStr[i + 1];

    char byte = 0;

    byte = hexCharToByte(char1) << 4;
    byte = byte | hexCharToByte(char2);

    outBuf[i / 2] = byte;
  }

  return outBuf;
}

/**
 * Reads a frequency map from a the given filename such
 * that map['a'] = the frequency of a. Prints error and
 * exits if unable to read the file
 */
double *readFreqMap(const char* filename) {
  FILE* freqFile = fopen(filename, "r");
  if (!freqFile) {
    printf("Couldn't open file %s", filename);
    exit(1);
  }
  double* freqMap = malloc(sizeof(double) * BYTE_SIZE);
  double freq;
  char ch;
  memset(freqMap, 0, BYTE_SIZE * sizeof(double));
  while (fscanf(freqFile, "%c%*c%lf%*c", &ch, &freq) == 2) {
    freqMap[ch] = freq;
  }
  fclose(freqFile);
  return freqMap;
}
