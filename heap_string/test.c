#include "heap_string.h"
#include <windows.h>
#include <string.h>

#pragma execution_character_set("utf-8")

int assign(HeapString* t, char* src);
int empty(HeapString* t);
int insert(HeapString* t, HeapString* src, int pos);
int copy(HeapString* s, HeapString* t);
int len(HeapString* s);
int display(HeapString* s);
void clear(HeapString* s);
int compare(const HeapString* s, const HeapString* t);
int cat(HeapString* s, HeapString* src);
int substr(HeapString* sub, const HeapString* s, int pos, int len);
int index(const HeapString* s, const HeapString* t, int* pos);

static int g_total = 0;
static int g_pass = 0;

static void report(const char *name, int ok)
{
    g_total++;
    if (ok)
        g_pass++;
    printf("%-10s%s\n", name, ok ? "成功" : "失败");
}

static int equals(const HeapString *s, const char *str)
{
    int n = (int)strlen(str);
    return (s != NULL) && (s->len == n)
        && (n == 0 || memcmp(s->ch, str, n) == 0);
}

int main(void)
{
    SetConsoleOutputCP(65001);

    {
        HeapString s = {0};
        int ok = (assign(&s, "hello") == OK) && equals(&s, "hello");
        report("assign", ok);
        clear(&s);
    }

    {
        HeapString s = {0};
        int ok = (empty(&s) == 1)
              && (assign(&s, "abc") == OK)
              && (empty(&s) == 0)
              && (empty(NULL) == ERR);
        report("empty", ok);
        clear(&s);
    }

    {
        HeapString s = {0}, src = {0};
        int ok = (assign(&s, "abc") == OK)
              && (assign(&src, "XY") == OK)
              && (insert(&s, &src, 2) == OK)
              && equals(&s, "aXYbc");
        report("insert", ok);
        clear(&s);
        clear(&src);
    }

    {
        HeapString s = {0}, t = {0};
        int ok = (assign(&t, "abc") == OK)
              && (copy(&s, &t) == OK)
              && equals(&s, "abc")
              && (s.len == t.len);
        report("copy", ok);
        clear(&s);
        clear(&t);
    }

    {
        HeapString s = {0};
        int ok = (assign(&s, "abcdef") == OK)
              && (len(&s) == 6)
              && (len(NULL) == ERR);
        report("len", ok);
        clear(&s);
    }

    {
        HeapString s = {0};
        int ok = (assign(&s, "hello") == OK)
              && (display(&s) == OK);
        report("display", ok);
        clear(&s);
    }

    {
        HeapString s = {0};
        int ok = (assign(&s, "abc") == OK)
              && (clear(&s), s.len == 0)
              && (s.ch == NULL);
        report("clear", ok);
    }

    {
        HeapString a = {0}, b = {0}, c = {0};
        int ok = (assign(&a, "abc") == OK)
              && (assign(&b, "abd") == OK)
              && (assign(&c, "abc") == OK)
              && (compare(&a, &c) == 0)
              && (compare(&a, &b) < 0)
              && (compare(&b, &a) > 0);
        report("compare", ok);
        clear(&a);
        clear(&b);
        clear(&c);
    }

    {
        HeapString s = {0}, src = {0};
        int ok = (assign(&s, "ab") == OK)
              && (assign(&src, "cd") == OK)
              && (cat(&s, &src) == OK)
              && equals(&s, "abcd");
        report("cat", ok);
        clear(&s);
        clear(&src);
    }

    {
        HeapString s = {0}, sub = {0};
        int ok = (assign(&s, "abcdef") == OK)
              && (substr(&sub, &s, 2, 3) == OK)
              && equals(&sub, "bcd");
        report("substr", ok);
        clear(&s);
        clear(&sub);
    }

    {
        HeapString s = {0}, t = {0};
        int pos = 1;
        int ok = (assign(&s, "ababcabcacbab") == OK)
              && (assign(&t, "abcac") == OK)
              && (index(&s, &t, &pos) == OK)
              && (pos == 6);
        report("index", ok);
        clear(&s);
        clear(&t);
    }

    printf("\n共 %d 项，通过 %d 项\n", g_total, g_pass);
    return (g_pass == g_total) ? 0 : 1;
}
