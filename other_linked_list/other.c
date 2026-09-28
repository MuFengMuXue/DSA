#include "other.h"

Node* create_josephus_ring(){
    Node* head=NULL,*tail=NULL,*node = NULL;
    int v = 0;
    while(scanf("%d",&v)==1){
        node = (Node*)malloc(sizeof(Node));
        node->data = v;
        node->next = NULL;
        if(head==NULL){
            head = node;
            tail = node;
        }
        else{
            tail->next = node;
            tail = node;
        }
    }
    tail->next = head;
    return head;
}

int josephus_ring(Node* head,int m,int k){
    if(head==NULL||m<=0||k<=0){
        return ERR;
    }
    Node* tail = head;
    while(tail->next != head) 
        tail = tail->next;
    Node* cur = head,*pre = tail,*delete = NULL;
    for(int i = 1;i<k;i++){
        pre = cur;
        cur = cur->next;
    }
    while(cur!=cur->next){
        for(int j = 1;j < m;j++){
            pre = cur;
            cur = cur->next;
        }
        delete =cur;
        cur = cur->next;
        printf("%d ",delete->data);
        pre->next = cur;
        free(delete);
    }
    printf("%d",cur->data);
    free(cur);
    return SUCCESS;
}

int reserve_list(Node** head){
    Node* pre = NULL,*cur = NULL,*nxt = NULL;
    if(head==NULL||*head==NULL){
        return ERR;
    }
    cur = *head;
    while(cur!=NULL){
        nxt = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nxt;
    }
    *head = pre;
    return SUCCESS;

}