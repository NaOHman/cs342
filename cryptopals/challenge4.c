#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crypto-utils.h"
#include "base64.h"
#include "xor-utils.h"

// Frequencies from http://fitaly.com/board/domper3/posts/136.html

#define FREQ_FILE "frequencies.txt"
#define TEST_FILE "encrypted_strings.txt"
#define N_LINES 327
#define READ_LEN 62

unsigned char* findMessage(FILE* file, double* freqMap, unsigned char* key, int* lineno){
    int len;
    double highest = 0.0, testscore;
    unsigned char testkey, *message;
    *lineno = 0;
    for (int i=0; i<N_LINES; i++){
        unsigned char* line = (unsigned char*) malloc(READ_LEN); //one for \n one for \0
        //read line
        if (!fgets(line, READ_LEN, file)) { //line+2 to capture \n
            printf("Error parsing file");
            exit(1);
        }
        //remove newline char
        for(int j=0; j<READ_LEN; j++){
            if (line[READ_LEN-j] == '\n'){
                line[READ_LEN-j] = '\0';
            }
        }
        //convert to bytes
        unsigned char* bytes = hexStrToBytes(line, &len);
        if (!line) {
            printf("Could not convert line to bytes");
            exit(1);
        }
        //get likely key
        unsigned char* testMsg = highScore(bytes, len, freqMap, &testkey, &testscore);
        if (testscore > highest){
            highest = testscore;
            *lineno = i;
            *key = testkey;
            message = testMsg;
        } else {
            free(testMsg);
        }
        //cleanup
        free(line);
    }
    return message;
}

int main(int argc, char *argv[]) {
    int blen;
    FILE* freqFile = fopen(FREQ_FILE, "r");
    if (!freqFile) {
        printf("Couldn't open file %s", FREQ_FILE);
        exit(1);
    }
    double *freqMap = readFreqMap(freqFile);
    fclose(freqFile);

    FILE* testFile = fopen(TEST_FILE, "r");
    if (!testFile) {
        printf("Couldn't open file %s", TEST_FILE);
        exit(1);
    }
    unsigned char key;
    int lineno;
    unsigned char* message = findMessage(testFile, freqMap, &key, &lineno);
    fclose(testFile);
    printf("The message is on line %d: %s\n",lineno, message);
    printf("The key is: %c\n", key);
    free(message);
    free(freqMap);
    return 0;
}
