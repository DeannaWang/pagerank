/*
    Interface to double linked list ADT for any-type values.
    For COMP9024 assignment 2.
    Written by Xiyan Wang.
*/

#ifndef DLList_H
#define DLList_H

typedef struct DLListRep* DLList;

// Create and insert. 
DLList newDLList (int);
void insertDLList (DLList, void*);

// Basic functions. 
int keyExistsDLList (DLList, void*);
int nitemsDLList (DLList);

// Iteration.
void resetDLList (DLList);
void* nextDLList (DLList);
void* previousDLList (DLList);

// Free the memory.
// Special free function is needed to free the memory of special type of key.
// If key can be simply freed, use NULL for freeKeyFunc.
void freeDLList (DLList, void (*)(void*));

#endif
