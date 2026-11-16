#include "sequential_string.h"
#include <windows.h>
#include <string.h>

#pragma execution_character_set("utf-8")

int str_assign(SeqString* s,char* src);
int str_substr(SeqString* sub,const SeqString* s,int pos,int len);
int index_bf(const SeqString* s,const SeqString* t,int pos);

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
    printf("%-10s", id);
    printf("%s", name);
    pad = 18 - disp_width(name);
    for (i = 0; i < pad; i++)
        putchar(' ');
    printf("%s\n", ok ? "成功" : "失败");
}

static int assign(SeqString* s, const char* src)
{
    return str_assign(s, (char*)src) == OK;
}

static int equals(const SeqString* s, const char* str)
{
    int n = (int)strlen(str);
    return (s->len == n) && (memcmp(s->ch, str, n) == 0);
}

int main(void)
{
    SetConsoleOutputCP(65001);

    {
        SeqString s = {0}, sub = {0};
        int ok = assign(&s, "abcdef")
              && (str_substr(&sub, &s, 2, 3) == OK)
              && equals(&sub, "bcd");
        report("4-3-12-1", "取中间子串", ok);
    }

    {
        SeqString s = {0}, sub = {0};
        int ok = assign(&s, "abcdef")
              && (str_substr(&sub, &s, 1, 6) == OK)
              && equals(&sub, "abcdef");
        report("4-3-12-2", "取整个串", ok);
    }

    {
        SeqString s = {0}, sub = {0};
        int ok = assign(&s, "abcdef")
              && (str_substr(&sub, &s, 6, 1) == OK)
              && equals(&sub, "f");
        report("4-3-12-3", "取最后一个字符", ok);
    }

    {
        SeqString s = {0}, sub = {0};
        int ok = assign(&s, "abcdef")
              && (str_substr(&sub, &s, 3, 0) == OK)
              && equals(&sub, "");
        report("4-3-12-4", "取长度为0的子串", ok);
    }

    {
        SeqString s = {0}, sub = {0};
        int ok = assign(&s, "abcdef")
              && (str_substr(&sub, &s, 7, 1) == ERR)
              && (str_substr(&sub, &s, 0, 1) == ERR)
              && (str_substr(&sub, &s, 2, 6) == ERR);
        report("4-3-12-5", "参数越界返回ERR", ok);
    }

    {
        SeqString s = {0}, t = {0};
        int ok = assign(&s, "ababcabcacbab")
              && assign(&t, "abcac")
              && (index_bf(&s, &t, 1) == 6);
        report("4-3-13-1", "BF基本匹配", ok);
    }

    {
        SeqString s = {0}, t = {0};
        int ok = assign(&s, "ababc")
              && assign(&t, "abc")
              && (index_bf(&s, &t, 2) == 3)
              && (index_bf(&s, &t, 4) == 0);
        report("4-3-13-2", "从指定位置开始匹配", ok);
    }

    {
        SeqString s = {0}, t = {0};
        int ok = assign(&s, "hello world")
              && assign(&t, "world")
              && (index_bf(&s, &t, 1) == 7);
        report("4-3-13-3", "匹配末尾", ok);
    }

    {
        SeqString s = {0}, t = {0};
        int ok = assign(&s, "abcdef")
              && assign(&t, "xyz")
              && (index_bf(&s, &t, 1) == 0);
        report("4-3-13-4", "未找到返回0", ok);
    }

    {
        SeqString s = {0}, t = {0};
        int ok = assign(&s, "abc")
              && assign(&t, "")
              && (index_bf(&s, &t, 2) == 2);
        report("4-3-13-5", "空模式串", ok);
    }

    {
        SeqString s = {0}, t = {0};
        int ok = assign(&s, "abc")
              && assign(&t, "ab")
              && (index_bf(&s, &t, 0) == ERR)
              && (index_bf(&s, &t, 5) == ERR);
        report("4-3-13-6", "非法起点返回ERR", ok);
    }

    printf("\n共 %d 项，通过 %d 项\n", g_total, g_pass);
    return (g_pass == g_total) ? 0 : 1;
}
