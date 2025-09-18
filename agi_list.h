#ifndef AgiList_H
#define AgiList_H

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
} AgiList;

// ------------------------------------
// Public API
// ------------------------------------

// Initialize an empty list
void list_init(AgiList *list);

// Add element at the end of the list
int list_push_back(AgiList *list, void *data);

// Add element at the beginning of the list
int list_push_front(AgiList *list, void *data);

// Remove and return the element at the beginning of the list
void *list_pop_front(AgiList *list);

// Remove and return the element at the end of the list
void *list_pop_back(AgiList *list);

// Destroy the entire list (frees nodes, but not user data)
void list_destroy(AgiList *list);

// Optional: debug function for printing list of integers
void list_print_int(const AgiList *list);

#endif // AgiList_H
