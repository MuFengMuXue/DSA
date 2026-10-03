#include "sequential_queue.h"
#include <windows.h>

int init_queue(SeqQueue* q);
int is_empty(SeqQueue* q);
int is_full(SeqQueue* q);
int enqueue(SeqQueue* q, int v);
int dequeue(SeqQueue* q, int* v);

static void line(const char* no, const char* name, int ok){
    printf("%s  %s\t\t%s\n", no, name, ok ? "成功" : "失败");
}

int main(void){
    SetConsoleOutputCP(65001);
    SeqQueue q;
    int v = 0;
    int ok = 0;

    line("2-1-1", "类型定义", 1);

    ok = (init_queue(&q) == OK);
    line("2-1-2", "初始化队列", ok);

    ok = (is_empty(&q) == 1);
    line("2-1-3", "判断队空", ok);

    ok = (is_full(&q) == 0);
    line("2-1-4", "判断队满", ok);

    ok = (enqueue(&q, 10) == OK && enqueue(&q, 20) == OK && enqueue(&q, 30) == OK);
    line("2-1-5", "入队", ok);

    ok = (dequeue(&q, &v) == OK && v == 10);
    line("2-1-6", "出队", ok);

    return 0;
}
