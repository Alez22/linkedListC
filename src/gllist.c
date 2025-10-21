/*====================================================*/
//
//       	Author: Alessandro Giulian
//		Date: September 2025
/*====================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gllist.h"

// --------------------------------------------------
// Initialize list
// --------------------------------------------------
void list_init(gllist *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// -------------------------------------------------
// Push data at the tail
// Return 0 on success, -1 on failure
// -------------------------------------------------
int list_push_back(gllist *list, void *data) {

	Node *new_node = (Node *)malloc(sizeof(Node));
	if (!new_node) return -1; // allocation failed

    	new_node->data = data;
    	new_node->next = NULL;
    	new_node->prev = list->tail;

    	if (list->tail) {
        	list->tail->next = new_node;
    	} else {
        	list->head = new_node; // first node
    	}

    	list->tail = new_node;
    	list->size++;
    	return 0;
}

// -------------------------------------------------
// Add element at the beginning of the list
// Return 0 on success, -1 on failure
// -------------------------------------------------
int list_push_front(gllist *list, void *data) {
    // allocate new node
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return -1; // allocation failed
    }

    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = list->head;

    // if the list is not empty, update the current head
    if (list->head != NULL) {
        list->head->prev = new_node;
    } else {
        // if the list was empty, set tail as well
        list->tail = new_node;
    }

    // update head and size
    list->head = new_node;
    list->size++;

    return 0; // success
}

// -------------------------------------------------
// Remove and return the element at the beginning of the list
// Return pointer to user data, or NULL if the list is empty
// -------------------------------------------------
void *list_pop_front(gllist *list) {
    // check if the list is empty
    if (list->head == NULL) {
        return NULL;
    }

    // get the first node
    Node *old_head = list->head;
    void *data = old_head->data;

    // update head pointer
    list->head = old_head->next;

    if (list->head != NULL) {
        // if list is not empty after removal
        list->head->prev = NULL;
    } else {
        // if list becomes empty, update tail as well
        list->tail = NULL;
    }

    // free the old node
    free(old_head);

    // update size
    list->size--;

    // return user data
    return data;
}

// -------------------------------------------------
// Remove and return the element at the end of the list
// Return pointer to user data, or NULL if the list is empty
// -------------------------------------------------
void *list_pop_back(gllist *list) {
    // check if the list is empty
    if (list->tail == NULL) {
        return NULL;
    }

    // get the last node
    Node *old_tail = list->tail;
    void *data = old_tail->data;

    // update tail pointer
    list->tail = old_tail->prev;

    if (list->tail != NULL) {
        // if list is not empty after removal
        list->tail->next = NULL;
    } else {
        // if list becomes empty, update head as well
        list->head = NULL;
    }

    // free the old node
    free(old_tail);

    // update size
    list->size--;

    // return user data
    return data;
}

// -------------------------------------------------
// Destroy the entire list (frees all nodes, but not user data)
// After this call, the list is empty and can be reused
// -------------------------------------------------
void list_destroy(gllist *list) {
    Node *current = list->head;
    Node *next_node;

    // iterate through the list and free all nodes
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    // reset list structure
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// -------------------------------------------------
// Debug function: print all integers in the list
// Assumes that data pointers are of type (int *)
// -------------------------------------------------
void list_print_int(const gllist *list) {
    const Node *current = list->head;

    printf("List size: %zu\n", list->size);
    printf("[");

    while (current != NULL) {
        int *value = (int *)current->data;
        if (value != NULL) {
            printf("%d", *value);
        } else {
            printf("NULL");
        }

        if (current->next != NULL) {
            printf(", ");
        }

        current = current->next;
    }

    printf("]\n");
}
