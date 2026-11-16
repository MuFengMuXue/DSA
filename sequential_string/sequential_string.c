#include "sequential_string.h"

int str_assign(SeqString* s,char* src){
    if(s==NULL||src==NULL){
        return ERR;
    }
    int i = 0;
    while(src[i]!='\0'&&i<M){
        s->ch[i] = src[i];
        i++;
    }
    s->len = i;
    if(s->ch[i]!='\0'){
        return ERR;
    }
    return OK;
}

int is_empty(SeqString* s){
    if(s==NULL){
        return 1;
    }
    return (s->len==0)?1:0;
}

int insert(SeqString* t,SeqString* src,int pos){
    if(t==NULL||src==NULL){
        return ERR;
    }
    pos--;
    if(t->len+src->len>M){
        return ERR;
    }
    int ins = src->len;
    for(int i = t->len-1;i>=pos;i--){
        t->ch[i+ins] = t->ch[i];
    }
    for(int i=0;i<ins;i++){
        t->ch[pos+i] = src->ch[i];
    }
    t->len+=ins;
    return OK;
}

int delete(SeqString* s,int pos,int len){
    if(s==NULL){
        return ERR;
    }
    pos--;
    if(pos<0||pos>=s->len){
        return ERR;
    }
    if(pos+len>s->len){
        len = s->len-pos;
    }
    for(int i = pos;i<s->len;i++){
        s->ch[i] = s->ch[len+i];
    }
    s->len-=len;
    return OK;
}

int copy(SeqString* t,SeqString* src){
    if(t==NULL||src==NULL){
        return ERR;
    }
    for(int i = 0;i<src->len-1;i++){
        t->ch[i] = src->ch[i];
    }
    t->len = src->len;
    return 0;
}

int len(SeqString* s){
    if(s==NULL){
        return ERR;
    }
    return s->len;
}

int clear(SeqString* s){
    if(s==NULL){
        return ERR;
    }
    s->len=0;
    return OK;
}

int display(SeqString* s){
    if(s==NULL){
        return ERR;
    }
    for(int i= 0;i<s->len-1;i++){
        putchar(s->ch[i]);
    }
    return OK;
}

int compare(SeqString* s,SeqString* t){
    if(s==NULL||t==NULL){
        return -2;
    }
    int i=0;
    while(i<s->len&&i<t->len){
        return (s->ch[i]>t->ch[i])?1:-1;
    }
    if(s->len==t->len){
        return 0;
    }
    return (s->len>t->len)?1:-1;
}


int cat(SeqString* t,SeqString* src){
    if(t==NULL||src==NULL){
        return ERR;
    }
    if(t->len+src->len>M){
        return ERR;
    }
    int i=0,j=t->len;
    while(i<src->len-1&&j<M){
        t->ch[j] = src->ch[i];
    }
    return OK;
}

int str_substr(SeqString* sub,const SeqString* s,int pos,int len){
    if(sub==NULL||s==NULL){
        return ERR;
    }
    if(pos<1||pos>s->len||len<0||len>s->len-pos+1){
        return ERR;
    }
    for(int i=0;i<len;i++){
        sub->ch[i] = s->ch[pos-1+i];
    }
    sub->len = len;
    if(len<M){
        sub->ch[len]='\0';
    }
    return OK;
}

int index_bf(const SeqString* s,const SeqString* t,int pos){
    if(s==NULL||t==NULL){
        return ERR;
    }
    if(pos<1||pos>s->len+1){
        return ERR;
    }
    if(t->len==0){
        return pos;
    }
    int i = pos-1,j = 0;
    while(i<s->len&&j<t->len){
        if(s->ch[i]==t->ch[j]){
            i++;
            j++;
        }else{
            i = i-j+1;
            j = 0;
        }
    }
    if(j==t->len){
        return i-t->len+1;
    }
    return 0;
}

