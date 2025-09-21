/*====================================
 * File: gmbdllist.h
 * Author: Alessandro Giuliani
 * Date: Spetember 2025
 * ===================================*/

#ifndef GMBDLLIST_H
#define GMBDLLIST_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* ================================
 * Configuration
 * ================================ */

/* Maximum number of nodes in the pool */
#ifndef GMB_DLLIST_MAX_NODES
#define GMB_DLLIST_MAX_NODES 64
#endif

/* Return codes */
#define gmbdll_OK    0
#define gmbdll_ERR  -1

/* ================================
 * Forward declarations
 * ================================ */
typedef struct gmbdll_Node gmbdll_Node;
typedef struct gmbdll_Pool gmbdll_Pool;

/* ================================
 * Data structures
 * ================================ */

/* Node structure */
typedef struct gmbdll_Node {
    void *data;
    gmbdll_Node *next;
    gmbdll_Node *prev;
} gmbdll_Node;

/* Node pool structure */
typedef struct gmbdll_Pool{
    gmbdll_Node nodes[GMB_DLLIST_MAX_NODES];
    gmbdll_Node *free_list;  /* List of free nodes */
    size_t capacity;  /* Total capacity of the pool */
    size_t used;  /* 0 = free, 1 = used */
} gmbdll_Pool;

/* Linked list structure */
typedef struct gmbdll_List{
    gmbdll_Node *head;
    gmbdll_Node *tail;
    size_t size;
    gmbdll_Pool *pool;
} gmbdll_List;

/* ================================
 * API
 * ================================ */

/* Initialize pool object. Must be called before using the pool. */
int gmbdll_pool_init(gmbdll_Pool *pool);

/* Initialize list and associate with a pool */
int gmbdll_list_init(gmbdll_List *list, gmbdll_Pool *pool);

/* Return 1 if list empty, 0 otherwise */
int gmbdll_list_is_empty(const gmbdll_List *list);

/* Return current size */
size_t gmbdll_list_size(const gmbdll_List *list);

/* Push element at the beginning of the list */
int gmbdll_push_front(gmbdll_List *list, void *data);

/* Push element at the end of the list */
int gmbdll_push_back(gmbdll_List *list, void *data);

/* Pop element from the beginning of the list */
void *gmbdll_pop_front(gmbdll_List *list);

/* Pop element from the end of the list */
void *gmbdll_pop_back(gmbdll_List *list);

/* Generic find: returns pointer to node or NULL.
   cmp callback: returns 0 if equal, non-zero otherwise.
   key is passed to cmp. */
gmbdll_Node *gmbdll_list_find(gmbdll_List *list, \
int (*cmp)(const void *item, const void *key), const void *key); 

/* Remove a node given its pointer. If data_out != NULL, store the data pointer there.
   Returns 0 on success, -1 if node or list invalid. */
int gmbdll_list_remove_node(gmbdll_List *list, gmbdll_Node *node, void **data_out);

/* Clear list. If free_func != NULL, it is called for each data pointer.
   This frees all nodes back to the pool and resets list to empty. */
void gmbdll_list_clear(gmbdll_List *list, void (*free_func)(void *));

/* Optional: expose pool statistics */
size_t gmbdll_pool_capacity(const gmbdll_Pool *pool);

size_t gmbdll_pool_used(const gmbdll_Pool *pool);

/* Debug: print list of integers (for testing only) */
void gmbdll_print_int(const gmbdll_List *list);

#endif /* GMBDLLIST_H */

