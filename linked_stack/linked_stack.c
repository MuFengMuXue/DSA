#include "linked_stack.h"

Node* init_stack(){
    Node* head = (Node*)malloc(sizeof(Node));
    if(head!=NULL){
        head->data = 0;
        head->next=NULL;
    }
    return head;
}

int is_empty(Node* head){
    if(head==NULL){
        return ERR;
    }
    return (head->next==NULL);
}

int push(Node* head,int v){
    if(head==NULL){
        return ERR;
    }
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL){
        return ERR;
    }
    node->data = v;
    node->next = head->next;
    head->next = node;
    return OK;
}

int get_top(Node* head,int *v){
    if(head==NULL||is_empty(head)||v==NULL){
        return ERR;
    }
    *v = head->next->data;
    return OK;
}

int pop(Node* head,int *v){
    if(head==NULL||is_empty(head)||v==NULL){
        return ERR;
    }
    Node* delete = head->next;
    *v = delete->data;
    head->next = delete->next;
    free(delete);
    return OK;
}