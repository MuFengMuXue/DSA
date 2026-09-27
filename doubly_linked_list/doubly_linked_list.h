#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H
#include <stdio.h>
#include <stdlib.h>

#define ERR -1
#define SUCCESS 0

typedef struct DNode
{
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

int insert_list(DNode** head,int index,int value);
int delete_list(DNode** head,int index,int* value);

#endif