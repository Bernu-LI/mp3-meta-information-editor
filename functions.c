#include "functions.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

void showInfo(struct metadata* metadata) {
    for (int i = 0; i < metadata->frameCounter; i++) {
        printf("[%s]:\t", metadata->frames[i]->frameHeader->frameID);
        if (reverseBytes(metadata->frames[i]->frameHeader->size) > 100) {
            continue;
        }
        printFrameData(metadata->frames[i]);
    }
}

char setInfo(struct metadata* metadata, char propertyName[4], char* value) {
    for (int i = 0; i < metadata->frameCounter; i++) {
        if (strcmp(metadata->frames[i]->frameHeader->frameID, propertyName) == 0) {
            metadata->frames[i]->frameHeader->size = reverseBytes(strlen(value));
            realloc(metadata->frames[i]->data, reverseBytes(metadata->frames[i]->frameHeader->size));
            metadata->frames[i]->data = value;
            return updateMetadata(metadata);
        }
    }
    metadata->frames = realloc(metadata->frames, ++metadata->frameCounter * sizeof(struct frame));
    metadata->frames[metadata->frameCounter - 1] = malloc(sizeof(struct frame));
    metadata->frames[metadata->frameCounter - 1]->frameHeader = malloc(sizeof(struct frameHeader));
    metadata->frames[metadata->frameCounter - 1]->frameHeader->frameID[0] = propertyName[0];
    metadata->frames[metadata->frameCounter - 1]->frameHeader->frameID[1] = propertyName[1];
    metadata->frames[metadata->frameCounter - 1]->frameHeader->frameID[2] = propertyName[2];
    metadata->frames[metadata->frameCounter - 1]->frameHeader->frameID[3] = propertyName[3];
    metadata->frames[metadata->frameCounter - 1]->frameHeader->size = reverseBytes(strlen(value));
    metadata->frames[metadata->frameCounter - 1]->frameHeader->flags = 0;
    metadata->frames[metadata->frameCounter - 1]->data = value;
    return updateMetadata(metadata);
}

void getInfo(struct metadata* metadata, char propertyName[4]) {
    for (int i = 0; i < metadata->frameCounter; i++) {
        if (strcmp(metadata->frames[i]->frameHeader->frameID, propertyName) == 0) {
            printFrameData(metadata->frames[i]);
            return;
        }
    }
}
