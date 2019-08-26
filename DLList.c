/* 
    Implementation of double linked list ADT for any-type values.
    For COMP9024 assignment2.
    Written by Xiyan Wang.
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "DLList.h"

typedef struct Node {
    // The previous element, the next element,
    // and the key.
    void* key;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct DLListRep {
    // The number of nodes in the list, the size of key, 
    // the comparing function for key, the fist node and the last node.
    int nitems;
    int keySize;
    Node* first;
    Node* last;
    Node* curr;
} DLListRep;

// Create a new node.
static Node* newNode (void* key, int keySize) {
    Node* new = malloc(sizeof(Node));
    assert(new != NULL);
    new->key = malloc(keySize);
    assert(new->key != NULL);
    memcpy(new->key, key, keySize);
    new->prev = NULL;
    new->next = NULL;
    return new;
}

// Create a new list.
DLList newDLList (int keySize) {
    DLList new = malloc(sizeof(DLListRep));
    assert(new != NULL);
    new->nitems = 0;
    new->keySize = keySize;
    new->first = NULL;
    new->last = NULL;
    new->curr = NULL;
    return new;
}

// Whether the key exists.
int keyExistsDLList (DLList L, void* key) {
    if (L == NULL) {
        return 0;
    }
    Node* curNode = L->first;
    while (curNode != NULL) {
        if (memcmp(key, curNode->key, L->keySize) == 0) {
            return 1;
        }
    }
    return 0;
}

// Return the number of items. 
int nitemsDLList (DLList L) {
    assert(L != NULL);
    return L->nitems;
}

// Insert value to the end of list.
void insertDLList (DLList L, void* key) {
    assert(L != NULL);
    Node* new = newNode(key, L->keySize);
    assert(new != NULL);
    if (L->nitems == 0) {
        L->first = new;
        L->last = new;
    } else {
        L->last->next = new;
        L->last = new;
    }
    L->nitems++;
}

// Reset the cursor to the start of the list. 
// Must be set before using nextDLList of previousDLList. 
void resetDLList (DLList L) {
    if (L != NULL) {
        L->curr = L->first;
    }
}

// Return the current value and move the cursor to the next position.
void* nextDLList (DLList L) {
    if (L == NULL) {
        return NULL;
    }
    if (L->curr == NULL) {
        return NULL;
    }
    Node* curNode = L->curr;
    L->curr = L->curr->next;
    return curNode->key;
}

// // Return the current value and move the cursor to the next position.
void* previousDLList (DLList L) {
    if (L == NULL) {
        return NULL;
    }
    if (L->curr == NULL) {
        return NULL;
    }
    Node* curNode = L->curr;
    L->curr = L->curr->prev;
    return curNode->key;
}

// Free the memory.
// Special free function is needed to free the memory of special type of key.
// If key can be simply freed, use NULL for freeKeyFunc.
void freeDLList (DLList L, void (*freeKeyFunc)(void*)) {
    Node* curNode = L->first;
    while (curNode) {
        Node* nextNode = curNode->next;
        if (freeKeyFunc != NULL) {
            freeKeyFunc(curNode->key);
        } else {
            free(curNode->key);
        }
        free(curNode);
        curNode = nextNode;
    }
    free(L);
}
