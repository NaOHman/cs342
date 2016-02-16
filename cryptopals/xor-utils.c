#include <stdlib.h>
#include <string.h>
#include "xor-utils.h"
#include "crypto-utils.h"

//encrypts buffer buf with xor mask mask 
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

//encrypts a string with the repeating xor key key, both str and key are 
//expected to be null terminated
unsigned char* encryptRepeatingXOR(unsigned char *str, unsigned char *key) {
    int len = strlen(str);
    int klen = strlen(key);
    unsigned char *bout = (unsigned char *) malloc(len + 1); //no sizeof because the buff is a byte buffer
    for (int i=0;i<len;i++){
        bout[i] = str[i] ^ key[i % klen];
    }
    bout[len] = '\0';
    return bout;
}

//the likelyhood that some text is english
double scoreText(unsigned char *text, int tlen, double* freqMap) {
    double score = 0.0;
    for (int i=0;i<tlen;i++){
        score += freqMap[text[i]];
    }
    return score;
}

//Cracks an encrypted message and returns the result
unsigned char* crackXOR(unsigned char *text, int len, double* freqMap, unsigned char* key, double* score){
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
