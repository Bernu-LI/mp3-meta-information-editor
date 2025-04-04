#include "service.h"

#include "stdio.h"
#include "stdlib.h"

unsigned reverseBytes(unsigned n) {
    return  ( (n >> 24) & 0x000000ff) |
            ( (n >> 8) & 0x0000ff00) |
            ( (n << 8) & 0x00ff0000) |
            ( (n << 24) & 0xff000000);
}

char isCorrectFrame(struct frameHeader* frameHeader) {
    if (frameHeader->frameID[0] == 0 &&
        frameHeader->frameID[1] == 0 &&
        frameHeader->frameID[2] == 0 &&
        frameHeader->frameID[3] == 0)
        return 0;
    if (frameHeader->size == 0)
        return 0;
    return 1;
}

struct metadata* readFile(char* fileName) {
    FILE* file = fopen(fileName, "rb");

    if (file == NULL) {
        printf("File opening error");
        return NULL;
    }

    struct metadata* metadata = (struct metadata*)malloc(sizeof(struct metadata));

    metadata->fileName = fileName;

    metadata->header = (struct header*)malloc(sizeof(struct header));
    fread(metadata->header, sizeof(struct header), 1, file);

    if ( (metadata->header->flags >> 6) % 2) {
        metadata->extendedHeader = (struct extendedHeader*)malloc(sizeof(struct extendedHeader));
        fread(metadata->extendedHeader, sizeof(struct extendedHeader), 1, file);
    }
    else {
        metadata->extendedHeader = NULL;
    }

    metadata->frames = NULL;
    unsigned tagSize = reverseBytes(metadata->header->size);
    metadata->frameCounter = 0;
    while (ftell(file) < tagSize + sizeof(struct header)) {
        struct frame* frame = malloc(sizeof(struct frame));
        frame->frameHeader = malloc(sizeof(struct frameHeader));
        fread(frame->frameHeader, sizeof(struct frameHeader), 1, file);
        if (!isCorrectFrame(frame->frameHeader)) {
            break;
        }
        size_t frameDataSize = reverseBytes(frame->frameHeader->size);
        frame->data = malloc(frameDataSize * sizeof(char));
        fread(frame->data, sizeof(char), frameDataSize, file);
        metadata->frames = realloc(metadata->frames, ++metadata->frameCounter * sizeof(struct frame));
        metadata->frames[metadata->frameCounter - 1] = frame;
    }
    metadata->dataSize = tagSize + sizeof(struct header) - ftell(file);
    metadata->data = malloc(metadata->dataSize * sizeof(char));
    fread(metadata->data, sizeof(char), metadata->dataSize, file);

    fclose(file);

    return metadata;
}

void printFrameData(struct frame* frame) {
    for (int i = 0; i < reverseBytes(frame->frameHeader->size); i++) {
        if (frame->data[i] >= 32 && frame->data[i] <= 126) {
            printf("%c", frame->data[i]);
        }
    }
    printf("\n");
}

char updateMetadata(struct metadata* metadata) {
    FILE* file = fopen(metadata->fileName, "r+b");
    if (file == NULL)
        return 0x1F;
    fwrite(metadata->header, sizeof(struct header), 1, file);
    if (metadata->extendedHeader != NULL) {
        fwrite(metadata->extendedHeader, sizeof(struct extendedHeader), 1, file);
    }
    for (int i = 0; i < metadata->frameCounter; i++) {
        fwrite(metadata->frames[i]->frameHeader, sizeof(struct frameHeader), 1, file);
        fwrite(metadata->frames[i]->data, sizeof(char), reverseBytes(metadata->frames[i]->frameHeader->size), file);
    }
    fwrite(metadata->data, sizeof(char), metadata->dataSize, file);
    fclose(file);
}