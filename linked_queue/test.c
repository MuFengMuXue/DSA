#include "linked_queue.h"
#include <windows.h>

#pragma execution_character_set("utf-8")

int init_queue(LinkedQueue* q);
int is_empty(LinkedQueue* q);
int enqueue(LinkedQueue* q, int v);
int dequeue(LinkedQueue* q, int* v);

static UINT g_old_cp;

static void restore_cp(void) {
    SetConsoleOutputCP(g_old_cp);
}

int main(void) {
    g_old_cp = GetConsoleOutputCP();
    SetConsoleOutputCP(CP_UTF8);
    atexit(restore_cp);

    LinkedQueue q;

    if (init_queue(&q) == OK && q.front != NULL && q.front == q.rear) {
        printf("init_queue 成功\n");
    } else {
        printf("init_queue 失败\n");
        return 1;
    }

    if (is_empty(&q) == 1) {
        printf("is_empty 成功\n");
    } else {
        printf("is_empty 失败\n");
        return 1;
    }

    int ok = 1;
    for (int i = 1; i <= 3; i++) {
        if (enqueue(&q, i) != OK) {
            ok = 0;
        }
    }
    if (ok && is_empty(&q) == 0 && q.rear->data == 3) {
        printf("enqueue 成功\n");
    } else {
        printf("enqueue 失败\n");
        return 1;
    }

    ok = 1;
    int v;
    for (int i = 1; i <= 3; i++) {
        if (dequeue(&q, &v) != OK || v != i) {
            ok = 0;
        }
    }
    if (ok && is_empty(&q) == 1) {
        printf("dequeue 成功\n");
    } else {
        printf("dequeue 失败\n");
        return 1;
    }

    free(q.front);
    return 0;
}
