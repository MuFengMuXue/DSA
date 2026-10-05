#include "linked_queue.h"

int init_queue(LinkedQueue* q){
    if(q==NULL){
        return ERR;
    }
    Node* head = (Node*)malloc(sizeof(Node));
    if(head==NULL){
        return ERR;
    }
    q->front = head;
    q->rear = head;
    return OK;
}

int is_empty(LinkedQueue* q){
    if(q==NULL){
        return ERR;
    }
    return (q->front==q->rear);
}

int enqueue(LinkedQueue* q,int v){
    if(q==NULL){
        return ERR;
    }
    Node* node=(Node*)malloc(sizeof(Node));
    if(node==NULL)
    {
        return ERR;
    }
    node->next=NULL;
    node->data = v;
    q->rear->next = node;
    q->rear = node;
    return OK;
}

int dequeue(LinkedQueue* q,int* v){
    if(q==NULL||v==NULL||is_empty(q)){
        return ERR;
    }
    Node* head=q->front;
    Node* delete = head->next;
    *v = delete->data;
    head->next = delete->next;
    if(q->rear == delete){
        q->rear = head;
    }
    free(delete);
    return OK;
}

