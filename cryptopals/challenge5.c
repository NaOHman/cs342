#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "xor-utils.h"

static char* key = "ICE";
static char* sInStr = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";

static char* sOutputStr = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

int main(int argc, char *argv[]) {
  int bytesLen = 0;
  char *outbytes = hexStrToBytes(sOutputStr, &bytesLen);
  if (!outbytes || !bytesLen) {
    printf("Failure! Couldn't convert hex to bytes.\n");
    return 1;
  }
  char *encrypted = encryptRepeatingXOR(sInStr, key);
  if (strncmp(encrypted, outbytes, bytesLen) == 0) {
    printf("Success!\n");
  } else {
    printf("Failure!\n");
  }
  free(outbytes);
  free(encrypted);
  return 0;
}
