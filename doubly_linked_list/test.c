#include "doubly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

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

/* ---------- helpers ---------- */

/* Build a list whose contents are arr[0..n-1] by appending one by one. */
static DNode* build(const int* arr, int n) {
    DNode* head = NULL;
    for (int i = 0; i < n; i++) {
        if (insert_list(&head, i, arr[i]) != SUCCESS) {
            return head;
        }
    }
    return head;
}

/* Forward comparison: list content must equal arr[0..n-1] and have no extra node. */
static int equals_array(DNode* head, const int* arr, int n) {
    DNode* p = head;
    for (int i = 0; i < n; i++) {
        if (p == NULL || p->data != arr[i]) {
            return 0;
        }
        p = p->next;
    }
    return p == NULL;
}

/*
 * Structural check:
 *  - head->prev == NULL
 *  - forward walk yields exactly n nodes matching arr
 *  - backward walk via prev yields the reverse sequence and ends exactly at head
 */
static int links_consistent(DNode* head, const int* arr, int n) {
    if (head == NULL) {
        return n == 0;
    }
    if (head->prev != NULL) {
        return 0;
    }

    DNode* tail = head;
    int cnt = 0;
    for (DNode* p = head; p != NULL; p = p->next) {
        tail = p;
        cnt++;
    }
    if (cnt != n) {
        return 0;
    }

    int i = n - 1;
    for (DNode* p = tail; p != NULL; p = p->prev) {
        if (i < 0 || p->data != arr[i]) {
            return 0;
        }
        i--;
    }
    return i == -1;
}

static void free_list(DNode* head) {
    while (head != NULL) {
        DNode* next = head->next;
        free(head);
        head = next;
    }
}

/* ---------- tests ---------- */

static void test_insert(void) {
    printf("insert_list:\n");

    /* 1. insert into empty list at index 0 */
    {
        DNode* h = NULL;
        int exp[] = {42};
        CHECK(insert_list(&h, 0, 42) == SUCCESS, "empty list, idx 0 -> SUCCESS");
        CHECK(equals_array(h, exp, 1), "empty list, idx 0 -> content [42]");
        CHECK(links_consistent(h, exp, 1), "empty list, idx 0 -> links ok");
        free_list(h);
    }

    /* 2. insert at head of a non-empty list */
    {
        int base[] = {10, 20, 30};
        DNode* h = build(base, 3);
        int exp[] = {5, 10, 20, 30};
        CHECK(insert_list(&h, 0, 5) == SUCCESS, "head insert -> SUCCESS");
        CHECK(equals_array(h, exp, 4), "head insert -> content [5,10,20,30]");
        CHECK(links_consistent(h, exp, 4), "head insert -> links ok (no self-cycle)");
        free_list(h);
    }

    /* 3. insert in the middle */
    {
        int base[] = {10, 30};
        DNode* h = build(base, 2);
        int exp[] = {10, 20, 30};
        CHECK(insert_list(&h, 1, 20) == SUCCESS, "middle insert -> SUCCESS");
        CHECK(equals_array(h, exp, 3), "middle insert -> content [10,20,30]");
        CHECK(links_consistent(h, exp, 3), "middle insert -> links ok");
        free_list(h);
    }

    /* 4. insert at the tail (index == length) */
    {
        int base[] = {10, 20};
        DNode* h = build(base, 2);
        int exp[] = {10, 20, 30};
        CHECK(insert_list(&h, 2, 30) == SUCCESS, "tail insert (idx==len) -> SUCCESS");
        CHECK(equals_array(h, exp, 3), "tail insert -> content [10,20,30]");
        CHECK(links_consistent(h, exp, 3), "tail insert -> links ok");
        free_list(h);
    }

    /* 5. insert out of range: must fail and leave the list untouched */
    {
        int base[] = {10, 20};
        DNode* h = build(base, 2);
        CHECK(insert_list(&h, 5, 99) == ERR, "out-of-range insert -> ERR");
        CHECK(equals_array(h, base, 2), "out-of-range insert -> list unchanged");
        CHECK(links_consistent(h, base, 2), "out-of-range insert -> links ok");
        free_list(h);
    }

    /* 6. negative index */
    {
        DNode* h = NULL;
        CHECK(insert_list(&h, -1, 1) == ERR, "negative index -> ERR");
        CHECK(h == NULL, "negative index -> list still empty");
    }

    /* 7. NULL head pointer */
    CHECK(insert_list(NULL, 0, 1) == ERR, "NULL head -> ERR");
}

static void test_delete(void) {
    printf("delete_list:\n");

    /* 8. delete from empty list */
    {
        DNode* h = NULL;
        int v = 123;
        CHECK(delete_list(&h, 0, &v) == ERR, "empty list delete -> ERR");
    }

    /* 9. delete head */
    {
        int base[] = {10, 20, 30};
        DNode* h = build(base, 3);
        int v = 0;
        int exp[] = {20, 30};
        CHECK(delete_list(&h, 0, &v) == SUCCESS, "delete head -> SUCCESS");
        CHECK(v == 10, "delete head -> value == 10");
        CHECK(equals_array(h, exp, 2), "delete head -> content [20,30]");
        CHECK(links_consistent(h, exp, 2), "delete head -> links ok, head->prev NULL");
        free_list(h);
    }

    /* 10. delete middle */
    {
        int base[] = {10, 20, 30};
        DNode* h = build(base, 3);
        int v = 0;
        int exp[] = {10, 30};
        CHECK(delete_list(&h, 1, &v) == SUCCESS, "delete middle -> SUCCESS");
        CHECK(v == 20, "delete middle -> value == 20");
        CHECK(equals_array(h, exp, 2), "delete middle -> content [10,30]");
        CHECK(links_consistent(h, exp, 2), "delete middle -> links ok");
        free_list(h);
    }

    /* 11. delete tail */
    {
        int base[] = {10, 20, 30};
        DNode* h = build(base, 3);
        int v = 0;
        int exp[] = {10, 20};
        CHECK(delete_list(&h, 2, &v) == SUCCESS, "delete tail -> SUCCESS");
        CHECK(v == 30, "delete tail -> value == 30");
        CHECK(equals_array(h, exp, 2), "delete tail -> content [10,20]");
        CHECK(links_consistent(h, exp, 2), "delete tail -> links ok");
        free_list(h);
    }

    /* 12. delete out of range: fail, list untouched */
    {
        int base[] = {10, 20};
        DNode* h = build(base, 2);
        int v = 0;
        CHECK(delete_list(&h, 5, &v) == ERR, "out-of-range delete -> ERR");
        CHECK(equals_array(h, base, 2), "out-of-range delete -> list unchanged");
        CHECK(links_consistent(h, base, 2), "out-of-range delete -> links ok");
        free_list(h);
    }

    /* 13. NULL value pointer */
    {
        int base[] = {10};
        DNode* h = build(base, 1);
        CHECK(delete_list(&h, 0, NULL) == ERR, "NULL value -> ERR");
        CHECK(equals_array(h, base, 1), "NULL value -> list unchanged");
        free_list(h);
    }

    /* 14. NULL head pointer */
    {
        int v = 0;
        CHECK(delete_list(NULL, 0, &v) == ERR, "NULL head -> ERR");
    }

    /* 15. delete one by one until empty, then delete again */
    {
        int base[] = {1, 2, 3};
        DNode* h = build(base, 3);
        int v = 0;
        CHECK(delete_list(&h, 0, &v) == SUCCESS && v == 1, "drain: pop 1");
        CHECK(delete_list(&h, 0, &v) == SUCCESS && v == 2, "drain: pop 2");
        CHECK(delete_list(&h, 0, &v) == SUCCESS && v == 3, "drain: pop 3");
        CHECK(h == NULL, "drain: list now empty");
        CHECK(delete_list(&h, 0, &v) == ERR, "drain: delete on empty -> ERR");
    }
}

int main(void) {
    test_insert();
    test_delete();

    printf("\n%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
