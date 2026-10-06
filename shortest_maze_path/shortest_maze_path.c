#include <stdio.h>
#include <stdlib.h>

#define ERR -1
#define OK 0

typedef struct Node{
    int data;
    struct Node* next;
}Node;

Node* init(void){
    Node* head = (Node*)malloc(sizeof(Node));
    if(head!=NULL){
        head->data=0;
        head->next = NULL;
    }
    return head;
}

int is_empty(Node* head){
    if(head==NULL){
        return ERR;
    }
    return (head->next==NULL);
}

int pop(Node* head,int* v){
    if(head==NULL||v==NULL||is_empty(head)){
        return ERR;
    }
    Node* delete = head->next;
    *v = delete->data;
    head->next = delete->next;
    free(delete);
    return OK;
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

int get_top(Node* head,int* v){
    if(head==NULL||v==NULL){
        return ERR;
    }
    *v = head->next->data;
    return OK;
}

int main(){
    int l=0;
    scanf("%d",&l);
    int* maze = (int*)malloc(l*l*sizeof(int));
    int* visited = (int*)calloc(l*l,sizeof(int));

    for(int i = 0;i<l*l;i++){
        scanf("%d",&maze[i]);
    }

    if(maze[l+1]!=0){
        printf("NO");
        free(maze);
        free(visited);
        return 0;
    }

    int dirs[4][2] = {{1,0},{0,1},{-1.0},{0,-1}};
    Node* stack = init();
    push(stack,l+1);
    visited[l+1] = 1;
    int found = 0;
    while(!is_empty(stack)){
        int pos;
        get_top(stack,&pos);
        int x = pos%l;
        int y = pos/l;
        if(x == l-2 && y == l-2){
            found = 1;
            break;
        }
        int move = 0;
        for(int i = 0;i<4;i++){
            int nx = x+dirs[i][0],ny = y+dirs[i][1];
            if(nx>=0&&nx<l&&ny>=0&&ny<l){
                int npos = ny*l+nx;
                if(maze[npos]==0&&visited[npos]==0){
                    push(stack,npos);
                    visited[npos] = 1;
                    move = 1;
                    break;
                }
            }
        }
        if(move==0){
            int tmp = 0;
            pop(stack,&tmp);
        }
    }
    if(found){
        Node* rev = init();
        while(!is_empty(stack)){
            int p;
            pop(stack,&p);
            push(rev,p);
        }
        while(!is_empty(rev)){
            int q;
            pop(rev,&q);
            printf("(%d,%d)",q/l,q%l);
        }
        free(rev);
    }
    else{
        printf("NO");
    }
    free(stack);
    free(maze);
    free(visited);
    return 0;
}