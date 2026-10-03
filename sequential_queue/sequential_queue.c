#include "sequential_queue.h"

int init_queue(SeqQueue* q){
    if(q == NULL){
        return ERR;
    }
    q->front = 0;
    q->rear = 0;
    return OK;
}

int is_empty(SeqQueue* q){
    if(q==NULL){
        return 1;
    }
    return (q->front==q->rear);
}

int is_full(SeqQueue* q){
    if(q==NULL){
        return 1;
    }
    return ((q->rear+1)%M == q->front);
}

int enqueue(SeqQueue* q,int v){
    if(q==NULL||is_full(q)){
        return ERR;
    }
    q->data[q->rear]=v;
    q->rear = (q->rear+1)%M;
    return OK;
}

int dequeue(SeqQueue* q,int* v){
    if(q==NULL||v==NULL||is_empty(q)){
        return ERR;
    }
    *v = q->data[q->front];
    q->front = (q->front+1)%M;
    return OK;
}

