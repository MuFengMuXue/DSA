#ifndef SEQUENTIAL_QUEUE_H
#define SEQUENTIAL_QUEUE_H

#include <stdlib.h>
#include <stdio.h>

#define M 10000

#define ERR -1
#define OK 0

typedef struct SeqQueue{
    int data[M];
    int front;
    int rear;
}SeqQueue;

#endif