#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERR -1

typedef struct {
    char* ch;
    int len;
}HeapString;