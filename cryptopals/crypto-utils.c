#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"

static char mask1 = 0xf0;
static char mask2 = 0x0f;

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

char byteToHexChar(char byte){
    byte += '0';
    if (byte > '9') {
        byte += '9' - 'a';
    }
    return byte;
}

char* bytes2HexStr(char* buf, int blen) {
    if (!buf || !blen){
        return NULL;
    }
    char* hex = malloc(blen * 2);
    for (int i=0; i<blen; i++){
        char bytes1 = buf[i] & mask1;
        char bytes2 = (buf[i] & mask2) >> 4;
        hex[i*2] = byteToHexChar(bytes1);
        hex[i*2 + 1] = byteToHexChar(bytes2);
    }
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
