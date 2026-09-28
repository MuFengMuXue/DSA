#ifndef OTHER_H
#define OTHER_H

#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

#define ERR -1
#define SUCCESS 0

Node* create_josephus_ring();

int josephus_ring(Node* head,int m,int k);

int reserve_list(Node** head);


#endif