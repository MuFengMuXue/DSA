#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define M 100000

typedef struct SeqStack{
    long long data[M];
    int top;
}SeqStack;

void initStack(SeqStack* s){
    s->top = -1;
}

int isEmpty(SeqStack* s){
    return s->top == -1;
}

int isFull(SeqStack* s){
    return s->top == M-1;
}

int push(SeqStack* s,long long v){
    if(isFull(s)){
        return 0;
    }
    s->data[++(s->top)] = v;
    return 1;
}

int pop(SeqStack* s,long long* v){
    if(isEmpty(s)){
        return 0;
    }
    *v = s->data[(s->top)--];
    return 1;
}

int getTop(SeqStack* s,long long* v){
    if(isEmpty(s)){
        return 0;
    }
    *v = s->data[s->top];
    return 1;
}

int size(SeqStack* s){
    return s->top+1;
}

int main(){
    SeqStack* s = (SeqStack*)malloc(sizeof(SeqStack));
    initStack(s);

    char ch[100];
    while(scanf("%s",ch) == 1){
        if(strcmp(ch,"#")==0){
            break;
        }

        if(strcmp(ch,"+")==0 || strcmp(ch,"-")==0 ||
           strcmp(ch,"/")==0 || strcmp(ch,"*")==0){
            
            if(size(s) < 2){
                long long topVal = 0;
                getTop(s,&topVal);
                printf("Expression Error: %lld\n",topVal);
                return 0;
            }

            long long a = 0,b = 0,c = 0;
            pop(s,&b);
            pop(s,&a);

            if(strcmp(ch,"+")==0){
                c = a+b;
            }
            else if(strcmp(ch,"-")==0){
                c = a-b;
            }
            else if(strcmp(ch,"*")==0){
                c = a*b;
            }
            else{
                if(b == 0){
                    printf("Error: %lld/0\n",a);
                    return 0;
                }
                c = a/b;
            }
            push(s,c);
            }
            else{
                push(s,atoll(ch));
            }
    }

    if(size(s)==1){
        long long ans = 0;
        getTop(s,&ans);
        printf("%lld\n",ans);
    }
    else if(size(s)>1){
        long long top;
        getTop(s,&top);
        printf("Expression Error: %lld\n",top);
    }
    else{
        printf("Expression Error: 0\n");
    }
    return 0;
    
}
