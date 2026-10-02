#ifndef SEQUENTIAL_STACK_H
#define SEQUENTIAL_STACK_H

#include <stdlib.h>
#include <stdio.h>

#define M 10000

typedef struct SeqStack
{
    int data[M];
    int top;
}SeqStack;

#define ERR -1
#define OK 0

#endif