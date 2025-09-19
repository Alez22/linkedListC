#include <stdio.h>
#include "gmbdllist.h"

int main(void) {
    /* Create pool and list */
    gmbdll_Pool pool = {0};
    gmbdll_List list;

    /* Initialize list */
    gmbdll_init(&list, &pool);

    printf("Initial list:\n");
    gmbdll_print_int(&list);

    /* Insert some integers */
    int a = 10, b = 20, c = 30, d = 40;

    gmbdll_push_back(&list, &pool, &a);
    gmbdll_push_back(&list, &pool, &b);
    gmbdll_push_front(&list, &pool, &c);
    gmbdll_push_back(&list, &pool, &d);

    printf("\nAfter pushes:\n");
    gmbdll_print_int(&list);

    /* Pop elements */
    int *front = (int *)gmbdll_pop_front(&list, &pool);
    printf("\nPopped front: %d\n", front ? *front : -1);

    int *back = (int *)gmbdll_pop_back(&list, &pool);
    printf("Popped back: %d\n", back ? *back : -1);

    printf("\nAfter pops:\n");
    gmbdll_print_int(&list);

    /* Destroy list */
    gmbdll_destroy(&list, &pool);

    printf("\nAfter destroy:\n");
    gmbdll_print_int(&list);

    return 0;
}

