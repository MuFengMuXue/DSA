#include "heap_string.h"

int assign(HeapString* t,char* src){
    if(t==NULL||src==NULL){
        return ERR;
    }
    int len = strlen(src);
    if(len==0){
        t->ch=NULL;
        t->len=0;
        return 0;
    }
    t->ch = (char*)malloc(sizeof(char)*(len+1));
    if(t->ch==NULL){
        return ERR;
    }
    memcpy(t->ch,src,len);
    t->len = len;
    return OK;
}

int empty(HeapString* t){
    if(t==NULL){
        return ERR;
    }
    if(t->len==0){
        return 1;
    }
    return 0;
}

int insert(HeapString* t,HeapString* src,int pos){
    if(t==NULL||src==NULL){
        return ERR;
    }
    pos--;
    if(pos<0||pos>t->len){
        return ERR;
    }
    int new = t->len + src->len;
    char* tmp = (char*)malloc(sizeof(char)*(new+1));
    memcpy(tmp,t->ch,pos);
    memcpy(tmp+pos,src->ch,src->len);
    if(t->len-pos>0){
        memcpy(tmp+pos+src->len,t->ch+pos,t->len-pos);
    }
    free(t->ch);
    t->ch=tmp;
    t->len=new;
    return OK;
}

int copy(HeapString* s,HeapString* t){
    if(s==NULL||t==NULL){
        return ERR;
    }
    if(s==t){
        return OK;
    }
    if(s->ch!=NULL){
        free(s->ch);
        s->ch=NULL;
    }
    s->len = t->len;
    s->ch = (char*)malloc(sizeof(char)*(s->len+1));
    memcpy(s->ch,t->ch,t->len);
    return OK;
}

int len(HeapString* s){
    if(s==NULL){
        return ERR;
    }
    return s->len;
}

int display(HeapString* s){
    if(s==NULL||s->len==0||s->ch==NULL){
        printf("空");
        return OK;
    }
    for(int i = 0;i<s->len;i++){
        putchar(s->ch[i]);
    }
    return OK;
}

void clear(HeapString* s) {
    if (s == NULL) return;
    if (s->ch != NULL) {
        free(s->ch);
        s->ch = NULL;
    }
    s->len = 0;
}

int compare(const HeapString* s, const HeapString* t) {
    if (s == NULL || t == NULL) return 0;
    int i = 0;
    while (i < s->len && i < t->len) {
        if (s->ch[i] != t->ch[i])
            return (s->ch[i] > t->ch[i]) ? 1 : -1;
        i++;
    }
    if (s->len == t->len) return 0;
    return (s->len > t->len) ? 1 : -1;
}

int cat(HeapString* s,HeapString* src){
    if(s==NULL||src==NULL){
        return ERR;
    }
    if(src->len==0){
        return OK;
    }
    int new = s->len+src->len;
    char* tmp = (char*)malloc(sizeof(char)*new);
    memcpy(tmp,s->ch,s->len);
    memcpy(tmp+s->len,src->ch,src->len);
    free(s->ch);
    s->ch = tmp;
    s->len = new;
    return OK;
}

int substr(HeapString* sub, const HeapString* s, 
int pos, int len) {
if (sub == NULL || s == NULL) return ERR;
pos--;  // 0-based
if (pos < 0 || pos >= s->len || len < 0)
return ERR;
if (pos + len > s->len) len = s->len - pos;
// 释放 sub 原有空间
if (sub->ch != NULL) free(sub->ch);
sub->ch = NULL;
sub->len = len;
if (len == 0) return OK;
sub->ch = (char*)malloc(len);
if (sub->ch == NULL) return ERR;
memcpy(sub->ch, s->ch + pos, len);
return OK;
}

int index(const HeapString* s, const HeapString* t, int* pos) {
    if (s == NULL || t == NULL || pos == NULL) return ERR;
    if (t->len == 0) return ERR;
    if (*pos < 1 || *pos > s->len) return ERR;
    int i = *pos - 1;  // 转为0-based
    int j = 0;
    while (i < s->len && j < t->len)
        if (s->ch[i] == t->ch[j]) {
            i++; j++;
        } else {
            i = i - j + 1; j = 0;
        }
    if (j == t->len) {
        *pos = i - j + 1;  // 转为1-based
        return OK;
    }
    return ERR;
}
