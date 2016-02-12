#import <stdlib.h>
#import <string.h>
#import "xor-utils.h"
#import "crypto-utils.h"

char* singleXOR(char* buf, char mask, int blen) {
    char *bout = (char *) malloc(blen); //no sizeof because the buff is a byte buffer
    if (!bout){
        return NULL;
    }
    for (int i=0; i<blen; i++){
        bout[i] = buf[i] ^ mask;
    }
    return bout;
}

char* encryptRepeatingXOR(char *buf, char *key) {
    int len = strlen(buf);
    int klen = strlen(key);
    char *bout = (char *) malloc(len + 1); //no sizeof because the buff is a byte buffer
    for (int i=0;i<len;i++){
        bout[i] = buf[i] ^ key[i % klen];
    }
    bout[len] = '\0';
    return bout;
}

double scoreText(char *text, int tlen, double* freqMap) {
    double score = 0.0;
    for (int i=0;i<tlen;i++){
        score += freqMap[text[i]];
    }
    return score;
}

char* highScore(char *text, int tlen, double* freqMap, char* key, double* score){
    char* decrypted;
    *score = 0;
    double highestFreq = 0;
    for (int c=0; c<BYTE_SIZE-1; c++){
        char* testStr = singleXOR(text, (char) c, tlen);
        double testScore = scoreText(testStr, tlen, freqMap);
        if (testScore > *score) {
            decrypted = testStr;
            *score = testScore;
            *key = c;
        } else {
            free(testStr);
        }
    }
    return decrypted;
}
