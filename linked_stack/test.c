#include "linked_stack.h"
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

static void free_stack(Node *head)
{
    while (head != NULL) {
        Node *tmp = head->next;
        free(head);
        head = tmp;
    }
}

int main(void)
{
    Node *s;
    int v;

    SetConsoleOutputCP(65001);

    {
        Node n;
        int ok;
        n.data = 42;
        n.next = NULL;
        ok = (n.data == 42) && (n.next == NULL);
        report("2-1-1", "类型定义", ok);
    }

    {
        int ok;
        s = init_stack();
        ok = (s != NULL) && (s->next == NULL);
        report("2-1-2", "init_stack", ok);
        free_stack(s);
    }

    {
        int ok;
        s = init_stack();
        ok = (is_empty(s) != 0);
        push(s, 7);
        ok = ok && (is_empty(s) == 0);
        pop(s, &v);
        ok = ok && (is_empty(s) != 0);
        report("2-1-3", "is_empty", ok);
        free_stack(s);
    }

    {
        int ok;
        s = init_stack();
        ok = (push(s, 10) == OK) && (s->next != NULL)
          && (s->next->data == 10) && (s->next->next == NULL);
        ok = ok && (push(s, 20) == OK) && (s->next->data == 20);
        report("2-1-4", "push", ok);
        free_stack(s);
    }

    {
        int a = 0, b = 0, ok;
        s = init_stack();
        push(s, 1);
        push(s, 2);
        ok = (pop(s, &a) == OK) && (a == 2) && (s->next->data == 1)
          && (pop(s, &b) == OK) && (b == 1) && (s->next == NULL);
        ok = ok && (pop(s, &v) == ERR);
        report("2-1-5", "pop", ok);
        free_stack(s);
    }

    {
        int t = 0, ok;
        s = init_stack();
        push(s, 5);
        push(s, 9);
        ok = (get_top(s, &t) == OK) && (t == 9) && (s->next->data == 9);
        report("2-1-6", "get_top", ok);
        free_stack(s);
    }

    printf("\n共 %d 项，通过 %d 项\n", g_total, g_pass);
    return (g_pass == g_total) ? 0 : 1;
}
