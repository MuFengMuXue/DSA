#include "singly_linked_list.h"


//尾插
Node* create_from_tail(void){
    Node* head = NULL,*tail = NULL,*node = NULL;
    int v = 0;
    while(scanf("%d",&v) == 1){
        node = (Node*)malloc(sizeof(Node));
        if(node == NULL){
            printf("ERROR");
            return head;
        }
        node->next = NULL;
        node->data = v;
        if(head!=NULL){
            tail->next = node;
            tail = node;
        }
        else{
            head = node;
            tail = node;
        }

    }
    return head;
}

//头插
Node* create_from_head(void){
    Node* head = NULL,*node = NULL;
    int v = 0;
    while(scanf("%d",&v) == 1){
        node = (Node*)malloc(sizeof(Node));
        if(node == NULL){
            printf("ERROR");
            return head;
        }
        node->data = v;
        node->next = head;
        head = node;        
    }
    return head;
}

//求表长
int list_length(Node* head){
    if(head == NULL){
        return ERR;
    }
    int count = 0;
    Node* cur = head;
    while(cur!=NULL){
        count++;
        cur = cur->next;
    }
    return count;
}

//按序号取节点
Node* get_node(Node* head,int index){
    if(head == NULL || index<0){
        printf("ERROR");
        return NULL;
    }
    int count = 0;
    Node* tmp = head;
    while(tmp!=NULL){
        count++;
        tmp = tmp->next;
    }
    if((count-1)<index){
        printf("NOT_FOUND");
        return NULL;
    }
    Node* cur = head;
    for(int i = 0;i<index;i++){
        cur = cur->next;
    }
    return cur;
}

//按值定位结点
Node* locate_node(Node* head,int value,int* index){
    int i = 0;
    Node* cur = head;
    while(cur!=NULL&&cur->data!=value){
        cur = cur->next;
        i++;
    }
    if(index!=NULL){
        *index = i;
    }
    return cur;
}

//显示链表
int display_list(Node* head){
    if(head == NULL){
        printf("这是空链表");
        return 0;
    }
    Node* cur = head;
    while(cur!=NULL){
        printf("%d ",cur->data);
        cur = cur->next;
    }
    return 0;
}

//插入结点
Node* insert_list(Node* head,int index,int value){
    Node* dummy = (Node*)malloc(sizeof(Node));
    if(dummy == NULL){
        return head;
    }
    dummy->data = 0;
    dummy->next = head;
    Node* cur = dummy;
    for(int i = 0;i<index;i++){
        if(cur->next == NULL){
            break;
        }
        cur = cur->next;
    }
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL){
        free(dummy);
        return head;
    }
    node->data = value;
    node->next = cur->next;
    cur->next = node;
    Node* newHead = dummy->next;
    free(dummy);
    return newHead;
}

//删除结点
Node* delete_node(Node* head,int index){
    if(head==NULL||index<0){
        return head;
    }
    Node* dummy = (Node*)malloc(sizeof(Node));
    if(dummy == NULL){
        return head;
    }
    dummy->next = head;
    Node* cur = dummy;
    for(int i = 0;i<index&&cur->next!=NULL;i++){
        cur = cur->next;
    }
    if(cur->next == NULL){
        free(dummy);
        return head;
    }
    Node* tmp = cur->next;
    cur->next = tmp->next;
    free(tmp);
    Node* newHead = dummy->next;
    free(dummy);
    return newHead;
}