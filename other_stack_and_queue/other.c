#include "other.h"

//栈操作
NodeS* init_stack(){
    NodeS* head = (NodeS*)malloc(sizeof(NodeS));
    if(head!=NULL){
        head->data = 0;
        head->next=NULL;
    }
    return head;
}

int is_empty_stack(NodeS* head){
    if(head==NULL){
        return ERR;
    }
    return (head->next==NULL);
}

int push(NodeS* head,int v){
    if(head==NULL){
        return ERR;
    }
    NodeS* node = (NodeS*)malloc(sizeof(NodeS));
    if(node == NULL){
        return ERR;
    }
    node->data = v;
    node->next = head->next;
    head->next = node;
    return OK;
}

int get_top_stack(NodeS* head,int *v){
    if(head==NULL||is_empty(head)||v==NULL){
        return ERR;
    }
    *v = head->next->data;
    return OK;
}

int pop(NodeS* head,int *v){
    if(head==NULL||is_empty(head)||v==NULL){
        return ERR;
    }
    NodeS* delete = head->next;
    *v = delete->data;
    head->next = delete->next;
    free(delete);
    return OK;
}

//队列操作

int init_queue(LinkedQueue* q){
    if(q==NULL){
        return ERR;
    }
    NodeQ* head = (NodeQ*)malloc(sizeof(NodeQ));
    if(head==NULL){
        return ERR;
    }
    q->front = head;
    q->rear = head;
    return OK;
}

int is_empty_queue(LinkedQueue* q){
    if(q==NULL){
        return ERR;
    }
    return (q->front==q->rear);
}

int enqueue(LinkedQueue* q,int v){
    if(q==NULL){
        return ERR;
    }
    NodeQ* node=(NodeQ*)malloc(sizeof(NodeQ));
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
    NodeQ* head=q->front;
    NodeQ* delete = head->next;
    *v = delete->data;
    head->next = delete->next;
    if(q->rear == delete){
        q->rear = head;
    }
    free(delete);
    return OK;
}

static int priority(char op){
    switch (op)
    {
    case '+' : case '-': return 1;
    case '*' : case '/': return 2;    
    default: return 0;
    }
}

//进制转换
int convert(int n,int b){
    NodeS* stack = init_stack();
    int digit = 0;
    int tmp = n;
    if(stack==NULL){
        printf("ERR");
        return ERR;
    }
    do{
        push(stack,tmp%b);
        tmp/=b;
    }while(tmp!=0);
    while(!is_empty(stack)){
        pop(stack,&digit);
        if(digit<10){
            printf("%d",digit);
        }
        else{
            printf("%c",'A'+digit-10);
        }
    }
    free(stack);
    return OK;
}

//迷宫
void maze(int row,int col){
    int* maze = (int*)malloc(row*col*sizeof(int));
    int* visited = (int*)calloc(row*col,sizeof(int));
    if(maze==NULL||visited==NULL){
        printf("ERR");
        free(maze);
        free(visited);
        return;
    }
    for(int i=0;i<row*col;i++){
        scanf("%s",&maze[i]);    
    }
    int dirs[4][2] = {{0,-1},{1,0},{0,1},{-1,0}};
    NodeS* stack = init_stack();
    if(stack==NULL){
        return;
    }
    push(stack,0);
    visited[0] = 1;
    int found = 0;
    while (!is_empty_stack(stack))
    {
        int pos = 0;
        get_top_stack(stack,&pos);
        int x = pos%col, y = pos/col;
        if(x==col-1&&y==row-1){
            found = 1;
            break;
        }
        int moved = 1;
        for(int i = 0;i<4;i++){
            int nx = x+dirs[i][0], ny = y + dirs[i][1];
            if(nx>=0&&nx<col&&ny>=0&&ny<row){
                int npos = ny*col + row;
                if(maze[npos]==0&&visited[npos]==0){
                    push(stack,npos);
                    visited[npos] = 1;
                    moved = 1;
                    break;
                }
            }
        }
        if(!moved){
            int tmp;
            pop(stack,&tmp);
        }
    }
    if(found){
        NodeS* rev = init_stack();
        while(!is_empty_stack(stack)){
            int p = 0;
            pop(stack,&p);
            push(rev,p);
        }
        while(!is_empty_stack(rev)){
            int q = 0;
            pop(rev,&q);
            printf("(%d,%d)",q%col,q/col);
        }
        free(rev);
    }
    else{
        printf("Not Found");
    }
    free(stack);
    free(maze);
    free(visited);
}

void bracket_match(){
    char str[256];
    scanf("%s",str);
    NodeS* stack = init_stack();
    if(stack==NULL){
        printf("ERR");
        return;
    }
    for(int i = 0;str[i]!='\0';i++){
        char ch = str[i];
        if(ch == '('||ch == '['||ch=='{'){
            push(stack,ch);
        }
        else if(ch==')'||ch==']'||ch=='}'){
            if(is_empty_stack(stack)){
                break;
            }
            int topChar;
            get_top_stack(stack,&topChar);
            char left = (char)topChar;
            if(left == '('&&ch == ')' || 
               left == '['&&ch==']' ||
               left == '{'&&ch=='}'){
                int tmp;
                pop(stack,&tmp); 
            }
        }
        else{
            break;
        }
    }
    if(str=='\0'&&is_empty_stack(stack)){
        printf("匹配成功");
    }
    else{
        printf("匹配失败");
    }
    while(is_empty_stack(stack)){
        int tmp;
        pop(stack,&tmp);
    }
    free(str);
    return;
}



