#include "service.h"
#include "functions.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

int main(int argc, char* argv[]) {
    char* fileName = malloc(200 * sizeof(char));
    char* frameName = malloc(10 * sizeof(char));
    char* value = malloc(100 * sizeof(char));
    char showFlag = 0;
    char setFlag = 0;
    char getFlag = 0;
    for (int i = 1;i < argc; i++){
        if (strcmp(argv[i], "--show") == 0) {
            showFlag = 1;
            continue;
        }
        if (argv[i][2] == 'f') {
            fileName = strpbrk(argv[i], "=") + 1;
        }
        if (argv[i][2] == 'g') {
            getFlag = 1;
            frameName = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 's') {
            setFlag = 1;
            frameName = strpbrk(argv[i], "=") + 1;
            continue;
        }
        if (argv[i][2] == 'v') {
            value = strpbrk(argv[i], "=") + 1;
            continue;
        }
    }

    struct metadata* metadata = readFile(fileName);

    if (metadata == NULL) {
        printf("Error while opening file\n");
        return 0xF;
    }

    if (getFlag)
        getInfo(metadata, frameName);
    if (setFlag) {
        if (strlen(frameName) == 4)
            setInfo(metadata, frameName, value);
        else
            printf("Error: incorrect prop name\n");
    }
    if (showFlag)
        showInfo(metadata);
    return 0;

}