#include <stdio.h>
#include <stdlib.h>
#include "gllist.h"

// -------------------------------------------------
// Simple test program for the linked list library
// -------------------------------------------------
int main(void) {
    gllist list;
    list_init(&list);

    printf("=== Linked List Test ===\n");

    // test: push_front
    int a = 10, b = 20, c = 30;
    list_push_front(&list, &a);
    list_push_front(&list, &b);
    list_push_front(&list, &c);
    printf("After push_front (c, b, a):\n");
    list_print_int(&list);

    // test: push_back
    int d = 40, e = 50;
    list_push_back(&list, &d);
    list_push_back(&list, &e);
    printf("After push_back (d, e):\n");
    list_print_int(&list);

    // test: pop_front
    int *front_val = (int *)list_pop_front(&list);
    if (front_val != NULL) {
        printf("Popped from front: %d\n", *front_val);
    }
    list_print_int(&list);

    // test: pop_back
    int *back_val = (int *)list_pop_back(&list);
    if (back_val != NULL) {
        printf("Popped from back: %d\n", *back_val);
    }
    list_print_int(&list);

    // test: destroy
    list_destroy(&list);
    printf("After destroy:\n");
    list_print_int(&list);

    return 0;
}
