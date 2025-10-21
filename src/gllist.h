#ifndef gllist_H
#define gllist_H

#include <stddef.h> // for size_t

// ------------------------------------
// Node structure
// ------------------------------------
typedef struct Node {
    void *data;           // pointer to user data
    struct Node *next;    // pointer to the next node
    struct Node *prev;    // pointer to the previous node
} Node;

// ------------------------------------
// Linked list structure
// ------------------------------------
typedef struct {
    Node *head;           // pointer to the first node
    Node *tail;           // pointer to the last node
    size_t size;          // number of elements in the list
} gllist;

// ------------------------------------
// Public API
// ------------------------------------

// Initialize an empty list
void list_init(gllist *list);

// Add element at the end of the list
int list_push_back(gllist *list, void *data);

// Add element at the beginning of the list
int list_push_front(gllist *list, void *data);

// Remove and return the element at the beginning of the list
void *list_pop_front(gllist *list);

// Remove and return the element at the end of the list
void *list_pop_back(gllist *list);

// Destroy the entire list (frees nodes, but not user data)
void list_destroy(gllist *list);

// Optional: debug function for printing list of integers
void list_print_int(const gllist *list);

#endif // gllist_H
