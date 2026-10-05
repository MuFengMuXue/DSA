#ifndef LINKED_QUEUE_H
#define LINKED_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define ERR -1

typedef struct Node{
    int data;
    struct Node* next;
}Node;

typedef struct LinkedQueue{
    Node* front;
    Node* rear;
}LinkedQueue;

#endif