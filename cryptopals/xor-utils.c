#include <stdlib.h>
#include <string.h>
#include "xor-utils.h"
#include "crypto-utils.h"

//encrypts buffer buf with xor mask mask 
char* singleXOR(char* buf, char mask, int blen) {
    char *bout = (char *) malloc(blen); 
    if (!bout){
        printf("Failed to malloc buffer");
        exit(1);
    }
    for (int i=0; i<blen; i++){
        bout[i] = buf[i] ^ mask;
    }
    return bout;
}

//encrypts a string with the repeating xor key key, both str and key are 
//expected to be null terminated
char* encryptRepeatingXOR(char *str, char *key) {
    int len = strlen(str);
    int klen = strlen(key);
    char *bout = (char *) malloc(len); //no sizeof because the buff is a byte buffer
    for (int i=0;i < len;i++){
        bout[i] = str[i] ^ key[i % klen];
    }
    return bout;
}

//the likelyhood that some text is english
double scoreText(char *bytes, int blen, double* freqMap) {
    double score = 0.0;
    for (int i=0;i<blen;i++){
        score += freqMap[(unsigned char) bytes[i]];
    }
    return score;
}

//Cracks an encrypted message and returns the result
char* crackXOR(char *bytes, int len, double* freqMap, char* key, double* score){
    char* decrypted = NULL;
    *score = 0;
    for (int c=0; c<BYTE_SIZE-1; c++){
        char* testBytes = singleXOR(bytes, (char) c, len);
        double testScore = scoreText(testBytes, len, freqMap);
        if (testScore > *score) {
            decrypted = testBytes;
            *score = testScore;
            *key = (char) c;
        } else {
            free(testBytes);
        }
    }
    return decrypted;
}
