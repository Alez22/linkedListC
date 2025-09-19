/*
 * gmbdllist.c
 * Embedded-friendly doubly-linked list using a fixed node pool.
 *
 * No dynamic allocation. All nodes are taken from a preallocated pool.
 * The library purposely leaves ownership of user data to the caller.
 *
 * Author: Alessandro Giuliani
 * Date: September 2025
 * 
 */

#include <stddef.h>  // size_t, NULL
#include "gmbdllist.h" // declare types and prototypes

/* ---------------------------
   Internal helper macros
   --------------------------- */
#ifndef gmbdll_POOL_MAX_NODES
#define gmbdll_POOL_MAX_NODES 64
#endif

/* Return codes */
#define gmbdll_OK    0
#define gmbdll_ERR  -1

/* ---------------------------
   Internal (static) functions
   --------------------------- */

/* Initialize pool: link all nodes into the free_list. */
static void pool_init(gmbdll_Pool *pool) {
    if (!pool) return;
    pool->free_list = NULL;
    pool->capacity = gmbdll_POOL_MAX_NODES;
    pool->used = 0;
    /* Link all nodes into free list */
    for (size_t i = 0; i < pool->capacity; ++i) {
        gmbdll_Node *n = &pool->nodes[i];
        n->data = NULL;
        n->prev = NULL;
        n->next = pool->free_list; /* push onto free_list */
        pool->free_list = n;
    }
}

/* Allocate one node from pool. Returns NULL if pool exhausted. */
static gmbdll_Node *pool_alloc_node(gmbdll_Pool *pool) {
    if (!pool || pool->free_list == NULL) return NULL;
    gmbdll_Node *n = pool->free_list;
    pool->free_list = n->next; /* next free */
    n->next = NULL;
    n->prev = NULL;
    n->data = NULL;
    pool->used++;
    return n;
}

/* Free node back to pool. Caller must ensure node is not in any list. */
static void pool_free_node(gmbdll_Pool *pool, gmbdll_Node *node) {
    if (!pool || !node) return;
    node->data = NULL;
    node->prev = NULL;
    /* push node back to free_list */
    node->next = pool->free_list;
    pool->free_list = node;
    if (pool->used > 0) pool->used--;
}

/* ---------------------------
   Public API
   (implementations for prototypes in gmbdllist.h)
   --------------------------- */

/* Initialize pool object. Must be called before using the pool. */
int gmbd_pool_init(gmbdll_Pool *pool) {
    if (!pool) return gmbdll_ERR;
    pool_init(pool);
    return gmbdll_OK;
}

/* Initialize a list and attach pool to it. pool must be valid and initialized. */
int gmbd_list_init(gmbdll_List *list, gmbdll_Pool *pool) {
    if (!list || !pool) return gmbdll_ERR;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->pool = pool;
    return gmbdll_OK;
}

/* Return 1 if list empty, 0 otherwise */
int gmbd_list_is_empty(const gmbdll_List *list) {
    if (!list) return 1;
    return (list->size == 0);
}

/* Return current size */
size_t gmbd_list_size(const gmbdll_List *list) {
    if (!list) return 0;
    return list->size;
}

/* Push at front. Returns 0 on success, -1 on failure (pool exhausted). */
int gmbd_list_push_front(gmbdll_List *list, void *data) {
    if (!list || !list->pool) return gmbdll_ERR;
    gmbdll_Node *node = pool_alloc_node(list->pool);
    if (!node) return gmbdll_ERR;
    node->data = data;
    node->prev = NULL;
    node->next = list->head;
    if (list->head) {
        list->head->prev = node;
    } else {
        /* empty list */
        list->tail = node;
    }
    list->head = node;
    list->size++;
    return gmbdll_OK;
}

/* Push at back. Returns 0 on success, -1 on failure (pool exhausted). */
int gmbd_list_push_back(gmbdll_List *list, void *data) {
    if (!list || !list->pool) return gmbdll_ERR;
    gmbdll_Node *node = pool_alloc_node(list->pool);
    if (!node) return gmbdll_ERR;
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;
    if (list->tail) {
        list->tail->next = node;
    } else {
        /* empty list */
        list->head = node;
    }
    list->tail = node;
    list->size++;
    return gmbdll_OK;
}

/* Pop from front. Returns data pointer or NULL if empty. */
void *gmbd_list_pop_front(gmbdll_List *list) {
    if (!list || !list->head) return NULL;
    gmbdll_Node *old = list->head;
    void *data = old->data;
    list->head = old->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        /* became empty */
        list->tail = NULL;
    }
    list->size--;
    /* return node to pool */
    pool_free_node(list->pool, old);
    return data;
}

/* Pop from back. Returns data pointer or NULL if empty. */
void *gmbd_list_pop_back(gmbdll_List *list) {
    if (!list || !list->tail) return NULL;
    gmbdll_Node *old = list->tail;
    void *data = old->data;
    list->tail = old->prev;
    if (list->tail) {
        list->tail->next = NULL;
    } else {
        /* became empty */
        list->head = NULL;
    }
    list->size--;
    pool_free_node(list->pool, old);
    return data;
}

/* Generic find: returns pointer to node or NULL.
   cmp callback: returns 0 if equal, non-zero otherwise.
   key is passed to cmp. */
gmbdll_Node *gmbd_list_find(gmbdll_List *list, int (*cmp)(const void *item, const void *key), const void *key) {
    if (!list || !cmp) return NULL;
    gmbdll_Node *cur = list->head;
    while (cur) {
        if (cmp(cur->data, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

/* Remove a node given its pointer. If data_out != NULL, store the data pointer there.
   Returns 0 on success, -1 if node or list invalid. */
int gmbd_list_remove_node(gmbdll_List *list, gmbdll_Node *node, void **data_out) {
    if (!list || !node) return gmbdll_ERR;

    /* Verify node belongs to this list is omitted for speed in embedded;
       caller must ensure correctness. */

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        /* node was head */
        list->head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        /* node was tail */
        list->tail = node->prev;
    }

    if (data_out) *data_out = node->data;

    list->size--;
    pool_free_node(list->pool, node);
    return gmbdll_OK;
}

/* Clear list. If free_func != NULL, it is called for each data pointer.
   This frees all nodes back to the pool and resets list to empty. */
void gmbd_list_clear(gmbdll_List *list, void (*free_func)(void *)) {
    if (!list) return;
    gmbdll_Node *cur = list->head;
    while (cur) {
        gmbdll_Node *next = cur->next;
        if (free_func && cur->data) {
            free_func(cur->data);
        }
        pool_free_node(list->pool, cur);
        cur = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/* Optional: expose pool statistics */
size_t gmbd_pool_capacity(const gmbdll_Pool *pool) {
    if (!pool) return 0;
    return pool->capacity;
}

size_t gmbd_pool_used(const gmbdll_Pool *pool) {
    if (!pool) return 0;
    return pool->used;
}

/* End of file */

