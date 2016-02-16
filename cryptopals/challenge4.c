#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crypto-utils.h"
#include "base64.h"
#include "xor-utils.h"

// Frequencies from http://fitaly.com/board/domper3/posts/136.html

static const char* freqFile = "frequencies.txt";
static const char* testFileName = "encrypted_strings.txt";
static const int MAX_READ = 62;

/**
 * Reads a line from file into the memory pointed to by *line
 * without a trailing newline character, returns the length of
 * the resulting string
 */
int getTrimLine(FILE* file, char** line) {
    size_t bufSize;
    int len = getline(line, &bufSize, file);
    if (len > 0 && (*line)[len-1] == '\n'){
        len--;
        (*line)[len] = '\0';
    }
    return len;
}

/**
 * finds the encrypted line in a file, returns the decrypted message, 
 * sets the *key to the working key and *lineno to the line number of
 * the encrypted message
 */
unsigned char* findMessage(FILE* file, double* freqMap, unsigned char* key, int* lineno){
    int bytelen, currentLine=0;
    double highest = 0.0, testscore;
    char testkey, *message;
    *lineno = 0;
    char* line = (char*) malloc(MAX_READ);
    while (getTrimLine(file, &line) > 0){
        currentLine++; //lines indexed from 1
        unsigned char* bytes = hexStrToBytes(line, &bytelen);
        if (!bytes) {
            printf("Could not convert line to bytes");
            exit(1);
        }
        //get likely key
        unsigned char* testMsg = crackXOR(bytes, bytelen, freqMap, &testkey, &testscore);
        if (testscore > highest){
            highest = testscore;
            *key = testkey;
            *lineno = currentLine;
            if (message) {
                free(message);
            }
            message = testMsg;
        } else {
            free(testMsg);
        }
    }
    return message;
}

int main(int argc, char *argv[]) {
    int blen, lineno;
    double *freqMap = readFreqMap(freqFile);
    unsigned char key;

    FILE* testFile = fopen(testFileName, "r");
    if (!testFile) {
        printf("Couldn't open file %s", testFile);
        exit(1);
    }
    unsigned char* message = findMessage(testFile, freqMap, &key, &lineno);
    fclose(testFile);

    printf("The message is on line %d: %s\n",lineno, message);
    printf("The key is: %c\n", key);
    free(message);
    free(freqMap);
    return 0;
}
