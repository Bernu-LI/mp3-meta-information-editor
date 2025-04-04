#ifndef functions
#define functions

#include "service.h"

void showInfo(struct metadata* metadata);

char setInfo(struct metadata* metadata, char propertyName[4], char* value);

void getInfo(struct metadata* metadata, char propertyName[4]);

#endif