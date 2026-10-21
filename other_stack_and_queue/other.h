#ifndef OTHER_H
#define OTHER_H

#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define ERR -1

//定义链栈
typedef struct NodeS{
    int data;
    struct NodeS* next;
}NodeS;
 
//定义队列
typedef struct NodeQ{
    int data;
    struct NodeQ* next;    
}NodeQ;

typedef struct LinkedQueue{
    NodeQ* front;
    NodeQ* rear;
}LinkedQueue;


#endif