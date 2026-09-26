#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include<stdio.h>
#include<stdlib.h>

#define ERR -1

#define NOT_FOUND -2

typedef struct Node{
    int data;
    struct Node* next;
} Node;

Node* create_from_tail();

Node* create_from_head(void);

int list_length(Node* head);

Node* get_node(Node* head,int index);

Node* locate_node(Node* head,int value,int* index);

int display_list(Node* head);

Node* insert_list(Node* head,int index,int value);

Node* delete_node(Node* head,int index);
#endif
