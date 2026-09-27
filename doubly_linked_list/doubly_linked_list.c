#include "doubly_linked_list.h"

int insert_list(DNode** head,int index,int value){
    if(head == NULL||index<0){
        return ERR;
    }

    DNode* node = (DNode*)malloc(sizeof(DNode));
    if(node == NULL){
        return ERR;
    }

    node->data = value;
    node->next = NULL;
    node->prev = NULL;

    if(index == 0){
        node->next =  *head;
        if(*head!=NULL){
            (*head)->prev = node;
        }
        *head = node;
        return SUCCESS;
    }

    DNode* cur = *head;
    int i = 0;
    while(cur!=NULL&&i<index-1){
        cur = cur->next;
        i++;
    }

    if(cur==NULL){
        free(node);
        return ERR;
    }
    node->next = cur->next;
    if(cur->next != NULL){
        cur->next->prev = node;
    }
    node->prev = cur;
    cur->next = node;
    return SUCCESS;
}

int delete_list(DNode** head,int index,int* value){
    if(head == NULL || *head == NULL || index<0 || value == NULL){
        return ERR;
    }

    DNode* cur = *head;

    if(index == 0){
        *value = cur->data;
        *head = cur->next;
        if(*head!=NULL){
            (*head)-> prev = NULL;
        }
        free(cur);
        return SUCCESS;
    }

    int i = 0;
    while(cur!=NULL&&i<index){
        cur = cur->next;
        i++;
    }

    if(cur==NULL){
        return ERR;
    }
    *value = cur->data;
    cur->prev->next = cur->next;
    if(cur->next!=NULL){
        cur->next->prev = cur->prev;
    }
    free(cur);
    return SUCCESS;
}