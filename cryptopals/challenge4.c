#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "crypto-utils.h"
#include "base64.h"
#include "xor-utils.h"

// Frequencies from http://fitaly.com/board/domper3/posts/136.html
// Compile with: gcc challenge2.c base64.c crypto-utils.c -g -o challenge2

#define FREQ_FILE "frequencies.txt"
#define TEST_FILE "encrypted_strings.txt"
#define N_LINES 327
#define LINE_LEN 61

char** parseTestFile(FILE* file, int *linelen){
    char** lines = (char**) malloc(N_LINES * sizeof(char*));
    char* line = (char*) malloc(LINE_LEN);
    int len;
    size_t cap;
    for (int i=0; i<N_LINES; i++){
        if ((len = getline(&line, &cap, file)) <= 0) {
            printf("Error parsing file");
            exit(1);
        }
        if (line[len-1] == '\n')
            line[len-1] = '\0';
        lines[i] = hexStrToBytes(line, linelen);
    }
    return lines;
}

char* findMessage(char** lines, int len, double* freqMap, char* key, int* lineno){
    *lineno = 0;
    double highest = 0.0, testscore;
    char testkey, *message;
    for(int i=0; i<N_LINES; i++){
        char* testMsg = highScore(lines[i], len, freqMap, &testkey, &testscore);
        if (testscore > highest){
            highest = testscore;
            *lineno = i;
            *key = testkey;
            message = testMsg;
        } else {
            free(testMsg);
        }
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
    char key;
    int lineno, len;
    char** lines = parseTestFile(testFile, &len);
    fclose(testFile);
    char* message = findMessage(lines, len, freqMap, &key, &lineno);
    printf("The message is on line %d: %s\n",lineno, message);
    printf("The key is: %c\n", key);
    free(lines);
    free(message);
    free(freqMap);
    return 0;
}
