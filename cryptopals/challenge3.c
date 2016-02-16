#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "base64.h"
#include "xor-utils.h"

// Frequencies from http://fitaly.com/board/domper3/posts/136.html

#define FREQ_FILE "frequencies.txt"

static char* inputString = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

int main(int argc, char *argv[]) {
    int blen;
    FILE* freqFile = fopen(FREQ_FILE, "r");
    if (!freqFile) {
        printf("Couldn't open file %s", FREQ_FILE);
        exit(1);
    }
    double *freqMap = readFreqMap(freqFile);
    fclose(freqFile);
    char* inputBytes = hexStrToBytes(inputString, &blen);
    if (!inputBytes || !blen) {
        printf("Error converting input to hex");
        exit(1);
    }
    char key;
    double score;
    char *message = highScore(inputBytes, blen, freqMap, &key, &score);
    printf("The message is: %s\n", message);
    printf("The key is: %c\n", key);
    free(inputBytes);
    free(message);
    free(freqMap);
    return 0;
}
