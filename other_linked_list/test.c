#define _POSIX_C_SOURCE 200809L
#include "other.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                                      \
    do {                                                      \
        if (cond) {                                           \
            g_pass++;                                         \
        } else {                                              \
            g_fail++;                                         \
            printf("  [FAIL] %s (line %d)\n", msg, __LINE__); \
        }                                                     \
    } while (0)

/* 输出串比对失败时把实际收到的东西打出来，方便定位 */
#define CHECK_OUT(cond, name, got)                            \
    do {                                                      \
        if (cond) {                                           \
            g_pass++;                                         \
        } else {                                              \
            g_fail++;                                         \
            printf("  [FAIL] %s -> got \"%s\"\n", name, got); \
        }                                                     \
    } while (0)

/* ---------- helpers ---------- */

/* 建一个 1..n 的循环单链表（环） */
static Node* build_ring(int n) {
    if (n <= 0) {
        return NULL;
    }
    Node* head = (Node*)malloc(sizeof(Node));
    head->data = 1;
    Node* cur = head;
    for (int i = 2; i <= n; i++) {
        cur->next = (Node*)malloc(sizeof(Node));
        cur = cur->next;
        cur->data = i;
    }
    cur->next = head; /* 尾指向首，成环 */
    return head;
}

/* 建一个普通（非循环）单链表 */
static Node* build_list(const int* a, int n) {
    Node* head = NULL;
    Node* tail = NULL;
    for (int i = 0; i < n; i++) {
        Node* p = (Node*)malloc(sizeof(Node));
        p->data = a[i];
        p->next = NULL;
        if (head == NULL) {
            head = p;
        } else {
            tail->next = p;
        }
        tail = p;
    }
    return head;
}

static void free_list(Node* head) {
    while (head != NULL) {
        Node* next = head->next;
        free(head);
        head = next;
    }
}

static void free_ring(Node* head) {
    if (head == NULL) {
        return;
    }
    Node* p = head->next;
    while (p != head) {
        Node* next = p->next;
        free(p);
        p = next;
    }
    free(head);
}

/* 环的内容是否等于 a[0..n-1] */
static int ring_equals(Node* head, const int* a, int n) {
    if (n == 0) {
        return head == NULL;
    }
    if (head == NULL) {
        return 0;
    }
    Node* p = head;
    int i = 0;
    do {
        if (i >= n || p->data != a[i]) {
            return 0;
        }
        i++;
        p = p->next;
    } while (p != head);
    return i == n;
}

/* 普通链表内容是否等于 a[0..n-1]，且没有多余结点 */
static int equals_array(Node* head, const int* a, int n) {
    Node* p = head;
    for (int i = 0; i < n; i++) {
        if (p == NULL || p->data != a[i]) {
            return 0;
        }
        p = p->next;
    }
    return p == NULL;
}

/*
 * 把 josephus_ring 打到 stdout 的内容截获到 out 里。
 * 返回 josephus_ring 的返回值。
 */
static int capture_josephus(Node* ring, int m, int k, char* out, int cap) {
    fflush(stdout);
    int saved = dup(fileno(stdout));
    FILE* tmp = tmpfile();
    if (tmp == NULL) {
        out[0] = '\0';
        return ERR;
    }
    dup2(fileno(tmp), fileno(stdout));

    int ret = josephus_ring(ring, m, k);

    fflush(stdout);
    dup2(saved, fileno(stdout));
    close(saved);

    rewind(tmp);
    size_t r = fread(out, 1, (size_t)cap - 1, tmp);
    out[r] = '\0';
    fclose(tmp);
    return ret;
}

/* 用给定的 stdin 内容调用 create_josephus_ring */
static Node* create_from_input(const char* input) {
    FILE* tmp = tmpfile();
    if (tmp == NULL) {
        return NULL;
    }
    fputs(input, tmp);
    rewind(tmp);

    int saved = dup(fileno(stdin));
    dup2(fileno(tmp), fileno(stdin));
    clearerr(stdin); /* 上一次读到 EOF 后要清掉，否则这儿直接判 EOF */

    Node* head = create_josephus_ring();

    dup2(saved, fileno(stdin));
    close(saved);
    fclose(tmp);
    return head;
}

/* ---------- tests ---------- */

static void test_create(void) {
    printf("create_josephus_ring:\n");

    {
        int exp[] = {1, 2, 3, 4, 5};
        Node* h = create_from_input("1 2 3 4 5");
        CHECK(ring_equals(h, exp, 5), "读入 5 个数 -> 环 1..5");
        free_ring(h);
    }
    {
        int exp[] = {42};
        Node* h = create_from_input("42");
        CHECK(ring_equals(h, exp, 1), "读入 1 个数 -> 单结点");
        CHECK(h != NULL && h->next == h, "单结点 next 指向自己");
        free_ring(h);
    }
}

static void test_josephus(void) {
    printf("josephus_ring:\n");

    /* exp == NULL 表示期望返回 ERR 且没有任何输出 */
    struct {
        int n, m, k;
        const char* exp;
        const char* name;
    } cases[] = {
        {5, 3, 1, "3 1 5 2 4", "n=5 m=3 k=1"},
        {5, 1, 1, "1 2 3 4 5", "n=5 m=1 k=1 (修复前会崩)"},
        {5, 1, 5, "5 1 2 3 4", "n=5 m=1 k=5"},
        {5, 3, 3, "5 3 2 4 1", "n=5 m=3 k=3"},
        {5, 80, 1, "5 4 2 1 3", "n=5 m=80 k=1"},
        {5, 3, 45, "2 5 4 1 3", "n=5 m=3 k=45"},
        {1, 3, 1, "1", "n=1 m=3 k=1 (只剩一个)"},
        {5, 0, 1, NULL, "m=0 -> ERR"},
        {5, 3, 0, NULL, "k=0 -> ERR"},
        {5, -1, 1, NULL, "m=-1 -> ERR"},
        {5, 3, -2, NULL, "k=-2 -> ERR"},
        {0, 3, 1, NULL, "空表 -> ERR"},
    };
    int total = (int)(sizeof(cases) / sizeof(cases[0]));
    for (int i = 0; i < total; i++) {
        char out[256];
        const char* exp = (cases[i].exp != NULL) ? cases[i].exp : "";
        int ret = capture_josephus(build_ring(cases[i].n), cases[i].m, cases[i].k, out, sizeof(out));
        int want_ret = (cases[i].exp != NULL) ? SUCCESS : ERR;
        CHECK_OUT(ret == want_ret && strcmp(out, exp) == 0, cases[i].name, out);
    }
}

static void test_reserve(void) {
    printf("reserve_list:\n");

    {
        int base[] = {1, 2, 3, 4, 5};
        int exp[] = {5, 4, 3, 2, 1};
        Node* h = build_list(base, 5);
        CHECK(reserve_list(&h) == SUCCESS, "5 个结点 -> SUCCESS");
        CHECK(equals_array(h, exp, 5), "5 个结点 -> [5,4,3,2,1]");
        free_list(h);
    }
    {
        int base[] = {42};
        int exp[] = {42};
        Node* h = build_list(base, 1);
        CHECK(reserve_list(&h) == SUCCESS, "单结点 -> SUCCESS");
        CHECK(equals_array(h, exp, 1), "单结点 -> [42]");
        free_list(h);
    }
    {
        Node* h = NULL;
        CHECK(reserve_list(&h) == ERR, "空表 -> ERR");
        CHECK(h == NULL, "空表 -> 仍为空");
    }
    CHECK(reserve_list(NULL) == ERR, "NULL 头指针 -> ERR");
}

int main(void) {
    test_create();
    test_josephus();
    test_reserve();

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
