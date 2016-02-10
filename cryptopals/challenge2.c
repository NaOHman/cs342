#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "base64.h"

// Compile with: gcc challenge2.c base64.c crypto-utils.c -g -o challenge2

static char* sInStr1 = "1c0111001f010100061a024b53535009181c";
static char* sInStr2 = "686974207468652062756c6c277320657965";

static char* sOutputStr = "746865206b696420646f6e277420706c6179";

char* xorBuf(char* buf1, char* buf2, int blen) {
    char *bout = (char *) malloc(blen); //no sizeof because the buff is a byte buffer
    if (!bout){
        return NULL;
    }
    for (int i=0; i<blen; i++){
        bout[i] = buf1[i] ^ buf2[i];
    }
    return bout;
}

int main(int argc, char *argv[]) {
  int bytesLen1 = 0;
  int bytesLen2 = 0;
  char *bytes1 = hexStrToBytes(sInStr1, &bytesLen1);
  char *bytes2 = hexStrToBytes(sInStr2, &bytesLen2);
  char *outbytes = hexStrToBytes(sOutputStr, NULL);
  if (!bytes1 || !bytes2) {
    printf("Failure! Couldn't convert hex to bytes.\n");
    return 1;
  }
  if (bytesLen1 != bytesLen2){
      printf("Failure! buffers must be the same length");
  }
  char *xored = xorBuf(bytes1, bytes2, bytesLen1);
  if (!xored) {
    printf("Failure! Couldn't alloc buffer for xored string.\n");
    return 1;
  }
  char *hexStr = bytesToHexStr(xored, bytesLen1);
  printf("%s\n", xored);
  printf("%s\n", hexStr);
  if (strcmp(sOutputStr, hexStr) == 0) {
    printf("Success!\n");
  } else {
    printf("Failure!\n");
  }

  free(hexStr);
  free(xored);
  return 0;
}
