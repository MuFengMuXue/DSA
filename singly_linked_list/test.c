/*
 * 单链表测试程序 test.c
 *
 * 测试 singly_linked_list.c 中现有的全部函数：
 *   create_from_tail  尾插创建
 *   create_from_head  头插创建
 *   list_length       求表长
 *   get_node          按序号取结点
 *   locate_node       按值定位结点
 *   display_list      显示链表
 *   insert_list       插入结点
 *   delete_node       删除结点
 *
 * 编译：gcc -Wall -Wextra -o test test.c singly_linked_list.c
 * 运行：./test
 *
 * 说明：为了不污染最终输出，测试过程中函数自身打印的
 *       "ERROR" / "NOT_FOUND" / display 内容都会被临时重定向，
 *       屏幕最终只保留 2-1-x 的测试结论行。
 *       若某项失败，详细信息会输出到 stderr。
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "singly_linked_list.h"

/* ================= 测试基础设施 ================= */

static int g_fail = 0;                 /* 累计失败数 */

static void check(int cond, const char* msg){
    if(!cond){
        g_fail++;
        fprintf(stderr, "  [FAIL] %s\n", msg);
    }
}

/* 统计 UTF-8 字符串的字符数（用于对齐中文列） */
static int utf8_len(const char* s){
    int n = 0;
    for(; *s; ++s)
        if((*s & 0xC0) != 0x80) n++;
    return n;
}

/* 打印一行测试结论：失败数没变 => 成功 */
static void record(const char* code, const char* name, int fail_before){
    printf("%s  %s", code, name);
    for(int i = utf8_len(name); i < 8; i++) printf("  ");
    printf("%s\n", (g_fail == fail_before) ? "成功" : "失败");
}

/* ================= 构造/校验工具 ================= */

/* 直接用数组构造一条单链表（作为测试夹具，不依赖被测函数） */
static Node* make_list(const int* a, int n){
    Node* head = NULL, *tail = NULL;
    for(int i = 0; i < n; i++){
        Node* node = (Node*)malloc(sizeof(Node));
        if(node == NULL) exit(2);
        node->data = a[i];
        node->next = NULL;
        if(tail != NULL) tail->next = node;
        else             head = node;
        tail = node;
    }
    return head;
}

/* 判断链表内容是否等于数组 a[0..n-1] */
static int list_equals(Node* head, const int* a, int n){
    int i = 0;
    Node* cur = head;
    while(cur != NULL && i < n){
        if(cur->data != a[i]) return 0;
        cur = cur->next;
        i++;
    }
    return (cur == NULL && i == n);
}

static void free_list(Node* head){
    while(head != NULL){
        Node* tmp = head->next;
        free(head);
        head = tmp;
    }
}

/* ================= 重定向工具 ================= */

/* 把一段文本写入临时文件并重定向到 stdin，然后调用 create_from_tail */
static Node* create_from_tail_with(const char* input){
    FILE* f = fopen("__test_in.txt", "w");
    if(f == NULL) return NULL;
    fputs(input, f);
    fclose(f);
    if(freopen("__test_in.txt", "r", stdin) == NULL) return NULL;
    Node* head = create_from_tail();
    remove("__test_in.txt");
    return head;
}

static Node* create_from_head_with(const char* input){
    FILE* f = fopen("__test_in.txt", "w");
    if(f == NULL) return NULL;
    fputs(input, f);
    fclose(f);
    if(freopen("__test_in.txt", "r", stdin) == NULL) return NULL;
    Node* head = create_from_head();
    remove("__test_in.txt");
    return head;
}

/* 临时把 stdout 丢到 /dev/null（屏蔽被测函数的提示打印） */
static int g_saved_stdout = -1;

static void capture_begin(void){
    fflush(stdout);
    g_saved_stdout = dup(1);
    FILE* devnull = fopen("/dev/null", "w");
    if(devnull != NULL){
        dup2(fileno(devnull), 1);
        fclose(devnull);
    }
}

static void capture_end(void){
    fflush(stdout);
    if(g_saved_stdout >= 0){
        dup2(g_saved_stdout, 1);
        close(g_saved_stdout);
        g_saved_stdout = -1;
    }
}

/* 捕获 display_list 的输出到 buf */
static void capture_display(Node* head, char* buf, size_t cap){
    buf[0] = '\0';
    fflush(stdout);
    int saved = dup(1);
    FILE* f = fopen("__test_out.txt", "w");
    if(f == NULL){
        if(saved >= 0) close(saved);
        return;
    }
    dup2(fileno(f), 1);
    display_list(head);
    fflush(stdout);
    dup2(saved, 1);
    if(saved >= 0) close(saved);
    fclose(f);

    FILE* r = fopen("__test_out.txt", "r");
    if(r != NULL){
        size_t got = fread(buf, 1, cap - 1, r);
        buf[got] = '\0';
        fclose(r);
    }
    remove("__test_out.txt");
}

/* ================= 测试用例 ================= */

/* 2-1-1 类型定义 */
static void test_type(void){
    Node n;
    n.data = 7;
    n.next = NULL;
    check(n.data == 7, "Node.data 可读可写");
    check(n.next == NULL, "Node.next 可读可写");
    check(sizeof(Node) >= sizeof(int) + sizeof(void*), "Node 结构大小合理");
}

/* 2-1-2 尾插创建链表 */
static void test_create_tail(void){
    Node* head = create_from_tail_with("1 2 3 4 5");
    int exp[] = {1, 2, 3, 4, 5};
    check(head != NULL, "尾插得到非空链表");
    check(list_equals(head, exp, 5), "尾插结果应为 1 2 3 4 5");
    check(list_length(head) == 5, "尾插长度应为 5");
    free_list(head);

    Node* empty = create_from_tail_with("");
    check(empty == NULL, "尾插空输入应得到空表");
    free_list(empty);
}

/* 2-1-3 头插创建链表 */
static void test_create_head(void){
    Node* head = create_from_head_with("1 2 3");
    int exp[] = {3, 2, 1};   /* 头插是逆序 */
    check(head != NULL, "头插得到非空链表");
    check(list_equals(head, exp, 3), "头插结果应为 3 2 1");
    check(list_length(head) == 3, "头插长度应为 3");
    free_list(head);
}

/* 2-1-4 求链表长度 */
static void test_length(void){
    int a[] = {9, 8, 7, 6};
    Node* h4 = make_list(a, 4);
    check(list_length(h4) == 4, "四结点链表长度应为 4");
    free_list(h4);

    Node* h1 = make_list(a, 1);
    check(list_length(h1) == 1, "单结点链表长度应为 1");
    free_list(h1);
}

/* 2-1-5 插入结点 */
static void test_insert(void){
    int a[] = {1, 2, 3};
    Node* h = make_list(a, 3);

    h = insert_list(h, 0, 0);            /* 头部插入 */
    int e1[] = {0, 1, 2, 3};
    check(list_equals(h, e1, 4), "头部插入 0 后应为 0 1 2 3");

    h = insert_list(h, 4, 4);            /* index == 表长，尾部插入 */
    int e2[] = {0, 1, 2, 3, 4};
    check(list_equals(h, e2, 5), "尾部插入 4 后应为 0 1 2 3 4");

    h = insert_list(h, 2, 99);           /* 中间插入 */
    int e3[] = {0, 1, 99, 2, 3, 4};
    check(list_equals(h, e3, 6), "中间插入 99 后应为 0 1 99 2 3 4");
    check(list_length(h) == 6, "插入后长度应为 6");
    free_list(h);

    Node* single = insert_list(NULL, 0, 7);   /* 空表插入 */
    int e4[] = {7};
    check(list_equals(single, e4, 1), "空表插入 7 应得到单结点表");
    free_list(single);
}

/* 2-1-6 删除结点 */
static void test_delete(void){
    int a[] = {1, 2, 3, 4};
    Node* h = make_list(a, 4);

    h = delete_node(h, 0);               /* 删头 */
    int e1[] = {2, 3, 4};
    check(list_equals(h, e1, 3), "删除头结点后应为 2 3 4");

    h = delete_node(h, 1);               /* 删中间 */
    int e2[] = {2, 4};
    check(list_equals(h, e2, 2), "删除中间结点后应为 2 4");

    h = delete_node(h, 1);               /* 删尾 */
    int e3[] = {2};
    check(list_equals(h, e3, 1), "删除尾结点后应为 2");

    h = delete_node(h, 0);               /* 删唯一结点 */
    check(h == NULL, "删除唯一结点后应为空表");

    int b[] = {9, 8};
    Node* k = make_list(b, 2);
    k = delete_node(k, 5);               /* 越界删除 */
    check(list_equals(k, b, 2), "越界删除应保持原链表不变");
    free_list(k);

    Node* n = delete_node(NULL, 0);
    check(n == NULL, "空表删除应返回空");
}

/* 2-1-7 按值定位结点 */
static void test_locate(void){
    int a[] = {10, 20, 30};
    Node* h = make_list(a, 3);
    int idx = -1;

    Node* p = locate_node(h, 20, &idx);
    check(p != NULL && p->data == 20, "定位 20 应命中");
    check(idx == 1, "定位 20 的下标应为 1");

    p = locate_node(h, 10, &idx);
    check(p == h && idx == 0, "定位头结点 10 下标应为 0");

    p = locate_node(h, 99, &idx);
    check(p == NULL, "定位不存在的 99 应返回空");

    p = locate_node(NULL, 5, &idx);
    check(p == NULL, "空表定位应返回空");
    free_list(h);
}

/* 2-1-8 按序号取结点 */
static void test_get(void){
    int a[] = {5, 6, 7};
    Node* h = make_list(a, 3);
    Node* p;

    capture_begin();
    p = get_node(h, 0);
    capture_end();
    check(p != NULL && p->data == 5, "取下标 0 应得到 5");

    capture_begin();
    p = get_node(h, 2);
    capture_end();
    check(p != NULL && p->data == 7, "取下标 2 应得到 7");

    capture_begin();
    p = get_node(h, 3);
    capture_end();
    check(p == NULL, "越界下标应返回空");

    capture_begin();
    p = get_node(NULL, 0);
    capture_end();
    check(p == NULL, "空表取结点应返回空");
    free_list(h);
}

/* 2-1-9 显示链表 */
static void test_display(void){
    int a[] = {5, 6, 7};
    Node* h = make_list(a, 3);
    char buf[128];

    capture_display(h, buf, sizeof(buf));
    check(strcmp(buf, "5 6 7 ") == 0, "显示结果应为 \"5 6 7 \"");

    capture_display(NULL, buf, sizeof(buf));
    check(strcmp(buf, "这是空链表") == 0, "空表显示应为 \"这是空链表\"");
    free_list(h);
}

/* 2-1-10 空表与边界处理 */
static void test_edge(void){
    check(list_length(NULL) <= 0, "空表长度应为非正");

    Node* h = insert_list(NULL, 0, 1);
    int e[] = {1};
    check(list_equals(h, e, 1), "空表插入应得到单结点表");
    free_list(h);

    check(locate_node(NULL, 1, NULL) == NULL, "空表定位应返回空");
    check(delete_node(NULL, 0) == NULL, "空表删除应返回空");
}

/* ================= 主程序 ================= */

int main(void){
    int f;

    f = g_fail; test_type();        record("2-1-1",  "类型定义",     f);
    f = g_fail; test_create_tail(); record("2-1-2",  "尾插创建链表", f);
    f = g_fail; test_create_head(); record("2-1-3",  "头插创建链表", f);
    f = g_fail; test_length();      record("2-1-4",  "求链表长度",   f);
    f = g_fail; test_insert();      record("2-1-5",  "插入结点",     f);
    f = g_fail; test_delete();      record("2-1-6",  "删除结点",     f);
    f = g_fail; test_locate();      record("2-1-7",  "按值定位结点", f);
    f = g_fail; test_get();         record("2-1-8",  "按序号取结点", f);
    f = g_fail; test_display();     record("2-1-9",  "显示链表",     f);
    f = g_fail; test_edge();        record("2-1-10", "空表边界处理", f);

    if(g_fail != 0){
        fprintf(stderr, "\n共 %d 项检查失败，请查看上面的 [FAIL] 信息。\n", g_fail);
        return 1;
    }
    return 0;
}
