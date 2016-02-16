#include <stdlib.h>
#include <string.h>
#include "xor-utils.h"
#include "crypto-utils.h"

unsigned char* singleXOR(unsigned char* buf, unsigned char mask, int blen) {
    unsigned char *bout = (unsigned char *) malloc(blen + 1); 
    if (!bout){
        printf("Failed to malloc buffer");
        exit(1);
    }
    for (int i=0; i<blen; i++){
        bout[i] = buf[i] ^ mask;
    }
    bout[blen] = '\0';
    return bout;
}

unsigned char* encryptRepeatingXOR(unsigned char *buf, unsigned char *key) {
    int len = strlen(buf);
    int klen = strlen(key);
    unsigned char *bout = (unsigned char *) malloc(len + 1); //no sizeof because the buff is a byte buffer
    for (int i=0;i<len;i++){
        bout[i] = buf[i] ^ key[i % klen];
    }
    bout[len] = '\0';
    return bout;
}

double scoreText(unsigned char *text, int tlen, double* freqMap) {
    double score = 0.0;
    for (int i=0;i<tlen;i++){
        score += freqMap[text[i]];
    }
    return score;
}

unsigned char* highScore(unsigned char *text, int len, double* freqMap, unsigned char* key, double* score){
    char* decrypted;
    *score = 0;
    double highestFreq = 0;
    for (int c=0; c<BYTE_SIZE-1; c++){
        unsigned char* testStr = singleXOR(text, (unsigned char) c, len);
        double testScore = scoreText(testStr, len, freqMap);
        if (testScore > *score) {
            decrypted = testStr;
            *score = testScore;
            *key = (unsigned char) c;
        } else {
            free(testStr);
        }
    }
    return decrypted;
}
