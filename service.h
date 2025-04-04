#ifndef service
#define service

struct header {
    char file_identifier[3];
    char version[2];
    char flags;
    unsigned size;
};

struct extendedHeader {
    unsigned size;
    unsigned short flagSize;
    unsigned flags;
};

struct frameHeader {
    char frameID[4];
    unsigned size;
    unsigned short flags;
};

struct frame {
    struct frameHeader* frameHeader;
    char* data;
};

struct metadata {
    char* fileName;
    struct header* header;
    struct extendedHeader* extendedHeader;
    struct frame** frames;
    unsigned frameCounter;
    unsigned dataSize;
    char* data;
};

unsigned reverseBytes(unsigned n);

char isCorrectFrame(struct frameHeader* frameHeader);

struct metadata* readFile(char* fileName);

char updateMetadata(struct metadata* metadata);

void printFrameData(struct frame* frame);

#endif

