/*====================================
 * File: gmbdllist.h
 * Author: Alessandro Giuliani
 * Date: Spetember 2025
 * ===================================*/

#ifndef GMBDLLIST_H
#define GMBDLLIST_H

#include <stddef.h>
#include <stdint.h>

/* ================================
 * Configuration
 * ================================ */

/* Maximum number of nodes in the pool */
#ifndef GMB_DLLIST_MAX_NODES
#define GMB_DLLIST_MAX_NODES 32
#endif

/* ================================
 * Data structures
 * ================================ */

/* Node structure */
typedef struct gmbdll_Node {
    void *data;
    struct gmbdll_Node *next;
} gmbdll_Node;

/* Linked list structure */
typedef struct {
    gmbdll_Node *head;
    gmbdll_Node *tail;
    size_t size;
} gmbdll_List;

/* Node pool structure */
typedef struct {
    gmbdll_Node nodes[GMB_DLLIST_MAX_NODES];
    uint8_t used[GMB_DLLIST_MAX_NODES];  /* 0 = free, 1 = used */
} gmbdll_Pool;

/* ================================
 * API
 * ================================ */

/* Initialize list and associate with a pool */
void gmbdll_init(gmbdll_List *list, gmbdll_Pool *pool);

/* Push element at the beginning of the list */
int gmbdll_push_front(gmbdll_List *list, gmbdll_Pool *pool, void *data);

/* Push element at the end of the list */
int gmbdll_push_back(gmbdll_List *list, gmbdll_Pool *pool, void *data);

/* Pop element from the beginning of the list */
void *gmbdll_pop_front(gmbdll_List *list, gmbdll_Pool *pool);

/* Pop element from the end of the list */
void *gmbdll_pop_back(gmbdll_List *list, gmbdll_Pool *pool);

/* Destroy list (frees all nodes, does not free user data) */
void gmbdll_destroy(gmbdll_List *list, gmbdll_Pool *pool);

/* Debug: print list of integers (for testing only) */
void gmbdll_print_int(const gmbdll_List *list);

#endif /* GMBDLLIST_H */

