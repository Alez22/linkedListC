/* ==========================================
 * test_gmbdllist.c
 * Unit test for gmbdllist library
 * Covers all public API functions
 * ========================================== */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "gmbdllist.h"

/* Comparison function for integers */
static int cmp_int(const void *item, const void *key) {
    int a = *(const int *)item;
    int b = *(const int *)key;
    return (a == b) ? 0 : (a < b ? -1 : 1);
}

/* Free function for integers */
static void free_int(void *p) {
    free(p);
}

int main(void) {
    printf("=== gmbdllist test start ===\n");

    gmbdll_Pool pool;
    gmbdll_List list;

    /* Test pool init */
    assert(gmbdll_pool_init(&pool) == gmbdll_OK);
    assert(gmbdll_pool_capacity(&pool) == GMB_DLLIST_MAX_NODES);
    assert(gmbdll_pool_used(&pool) == 0);

    /* Test list init */
    assert(gmbdll_list_init(&list, &pool) == gmbdll_OK);
    assert(gmbdll_list_is_empty(&list) == 1);
    assert(gmbdll_list_size(&list) == 0);

    /* Prepare some integers */
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    int *c = malloc(sizeof(int));
    *a = 10; *b = 20; *c = 30;

    /* Test push_front and push_back */
    assert(gmbdll_push_front(&list, a) == gmbdll_OK);
    assert(gmbdll_push_back(&list, b) == gmbdll_OK);
    assert(gmbdll_push_back(&list, c) == gmbdll_OK);

    assert(gmbdll_list_is_empty(&list) == 0);
    assert(gmbdll_list_size(&list) == 3);
    assert(gmbdll_pool_used(&pool) == 3);

    gmbdll_print_int(&list);

    /* Test find */
    int key = 20;
    gmbdll_Node *found = gmbdll_list_find(&list, cmp_int, &key);
    assert(found != NULL);
    assert(*(int *)found->data == 20);

    /* Test remove_node */
    void *removed_data = NULL;
    assert(gmbdll_list_remove_node(&list, found, &removed_data) == gmbdll_OK);
    assert(*(int *)removed_data == 20);
    free(removed_data); // free manually
    assert(gmbdll_list_size(&list) == 2);
    assert(gmbdll_pool_used(&pool) == 2);

    gmbdll_print_int(&list);

    /* Test pop_front */
    void *data1 = gmbdll_pop_front(&list);
    assert(*(int *)data1 == 10);
    free(data1);
    assert(gmbdll_list_size(&list) == 1);
    assert(gmbdll_pool_used(&pool) == 1);

    /* Test pop_back */
    void *data2 = gmbdll_pop_back(&list);
    assert(*(int *)data2 == 30);
    free(data2);
    assert(gmbdll_list_size(&list) == 0);
    assert(gmbdll_list_is_empty(&list) == 1);
    assert(gmbdll_pool_used(&pool) == 0);

    /* Test clear with free_func */
    int *d = malloc(sizeof(int));
    int *e = malloc(sizeof(int));
    *d = 40; *e = 50;
    gmbdll_push_back(&list, d);
    gmbdll_push_back(&list, e);
    assert(gmbdll_list_size(&list) == 2);
    gmbdll_list_clear(&list, free_int);
    assert(gmbdll_list_is_empty(&list) == 1);
    assert(gmbdll_pool_used(&pool) == 0);

    /* Test debug print on empty list */
    gmbdll_print_int(&list);

    printf("=== gmbdllist test passed ===\n");
    return 0;
}
