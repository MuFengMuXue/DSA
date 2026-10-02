#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

#define OK 0
#define ERR -1

Node* init_stack(void);
int is_empty(Node* head);
int push(Node* head, int v);
int get_top(Node* head, int* v);
int pop(Node* head, int* v);

#endif