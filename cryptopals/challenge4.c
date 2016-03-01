#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crypto-utils.h"
#include "base64.h"
#include "xor-utils.h"

// Frequencies from http://fitaly.com/board/domper3/posts/136.html

static const char* freqFile = "frequencies.txt";
static const char* testFileName = "encrypted_strings.txt";

/**
 * Reads a line from file into the memory pointed to by *line
 * without a trailing newline character, returns the length of
 * the resulting string
 */
int getTrimLine(FILE* file, char** line, size_t *size) {
    int len = getline(line, size, file);
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
char* findMessage(FILE* file, double* freqMap, char* key, int* lineno, size_t *size){
    int bytelen = 0, currentLine=0;
    size_t lineSz = 0;
    double highest = 0.0, testscore = 0.0;
    char testkey;
    char *message=NULL;
    char* line = NULL;
    *lineno = 0;
    while (getTrimLine(file, &line, &lineSz) > 0){
        currentLine++; //lines indexed from 1
        char* bytes = hexStrToBytes(line, &bytelen);
        if (!bytes) {
            printf("Could not convert line to bytes");
            exit(1);
        }
        //get likely key
        char* testMsg = crackXOR(bytes, bytelen, freqMap, &testkey, &testscore);
        if (testscore > highest){
            highest = testscore;
            *key = testkey;
            *lineno = currentLine;
            *size = lineSz;
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
    int lineno = 0;
    double *freqMap = readFreqMap(freqFile);
    char key;
    size_t size = 0;

    FILE* testFile = fopen(testFileName, "r");
    if (!testFile) {
        printf("Couldn't open file %s", testFileName);
        exit(1);
    }
    char* message = findMessage(testFile, freqMap, &key, &lineno, &size);
    fclose(testFile);
    if (!message) {
        printf("Could not find message\n");
        exit(1);
    }
    char* str_message = malloc(1 + size);
    memcpy(str_message, message, size);
    str_message[size] = '\0';
    printf("The message is on line %d: %s\n",lineno, str_message);
    printf("The key is: %c\n", key);
    free(message);
    free(str_message);
    free(freqMap);
    return 0;
}
