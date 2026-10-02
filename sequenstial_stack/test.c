#include "sequential_stack.h"
#include <windows.h>

static int g_total = 0;
static int g_pass = 0;

static int disp_width(const char *s)
{
    int w = 0;
    while (*s) {
        unsigned char c = (unsigned char)*s;
        if ((c & 0xC0) != 0x80)
            w += (c < 0x80) ? 1 : 2;
        s++;
    }
    return w;
}

static void report(const char *id, const char *name, int ok)
{
    int pad, i;
    g_total++;
    if (ok)
        g_pass++;
    printf("%-7s", id);
    printf("%s", name);
    pad = 16 - disp_width(name);
    for (i = 0; i < pad; i++)
        putchar(' ');
    printf("%s\n", ok ? "成功" : "失败");
}

int main(void)
{
    SeqStack s;
    int v;

    SetConsoleOutputCP(65001);

    {
        SeqStack t;
        int ok;
        t.top = 0;
        t.data[0] = 42;
        ok = (t.top == 0) && (t.data[0] == 42);
        report("2-1-1", "类型定义", ok);
    }

    {
        int ok;
        init_stack(&s);
        ok = (init_stack(&s) == OK) && (s.top == -1);
        report("2-1-2", "init_stack", ok);
    }

    {
        int ok;
        init_stack(&s);
        ok = (is_empty(&s) != 0);
        push(&s, 7);
        ok = ok && (is_empty(&s) == 0);
        pop(&s, &v);
        report("2-1-3", "is_empty", ok);
    }

    {
        int i, ok;
        init_stack(&s);
        for (i = 0; i < M; i++)
            push(&s, i);
        ok = (is_full(&s) != 0);
        for (i = 0; i < M; i++)
            pop(&s, &v);
        report("2-1-4", "is_full", ok);
    }

    {
        int ok;
        init_stack(&s);
        ok = (push(&s, 10) == OK) && (s.top == 0) && (s.data[0] == 10)
          && (push(&s, 20) == OK) && (s.top == 1) && (s.data[1] == 20);
        report("2-1-5", "push", ok);
    }

    {
        int a = 0, b = 0, ok;
        init_stack(&s);
        push(&s, 1);
        push(&s, 2);
        ok = (pop(&s, &a) == OK) && (a == 2) && (s.top == 0)
          && (pop(&s, &b) == OK) && (b == 1) && (s.top == -1);
        report("2-1-6", "pop", ok);
    }

    {
        int t = 0, ok;
        init_stack(&s);
        push(&s, 5);
        push(&s, 9);
        ok = (get_top(&s, &t) == OK) && (t == 9) && (s.top == 1);
        report("2-1-7", "get_top", ok);
    }

    printf("\n共 %d 项，通过 %d 项\n", g_total, g_pass);
    return (g_pass == g_total) ? 0 : 1;
}
