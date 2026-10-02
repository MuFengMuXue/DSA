#include "sequential_stack.h"

int init_stack(SeqStack* s){
    if(s==NULL){
        return ERR;
    }
    s->top = -1;
    return OK;
}

int is_full(SeqStack* s){
    if(s==NULL){
        return ERR;
    }
    return (s->top == M-1);
}

int is_empty(SeqStack* s){
    if(s==NULL){
        return ERR;
    }
    return (s->top == -1);
}

int pop(SeqStack* s,int *v){
    if(s==NULL||is_empty(s)||v==NULL){
        return ERR;
    }
    *v = s->data[s->top];
    s->top--;
    return OK;
}

int push(SeqStack* s,int v){
    if(s==NULL||is_full(s)){
        return ERR;
    }
    s->top++;
    s->data[s->top]=v;
    return OK;
}

int get_top(SeqStack* s,int *v){
    if(s==NULL||v==NULL){
        return ERR;
    }
    *v = s->data[s->top];
    return OK;
}