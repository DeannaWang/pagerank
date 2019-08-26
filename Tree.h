/* 
    Interface to red black tree ADT.
    A dictionary for any-type key-value pairs. Keys are in order and unique. 
    For COMP9024 assignment2.
    Written by Xiyan Wang.
*/

#ifndef RBTREE_H
#define RBTREE_H

typedef struct RBNode* RBTree;

// Basic functions. 
RBTree leftRBTree (RBTree t);
RBTree rightRBTree (RBTree t);
int keyExistsRBTree (RBTree t, void* key);

// Gets and sets. 
void* getTreeByKeyRBTree (RBTree t, void* key);
void* getValueRBTree (RBTree t);
void setValueRBTree (RBTree t, void* value);
void* getKeyRBTree (RBTree t);
int isBST(RBTree t);
int isBST2(RBTree t);
int isBST3(RBTree t);
void disturb(RBTree t);
int isAVL(RBTree t);
// Insert a key-value pair to the tree. If t is NULL, a new tree will be created.
// The arguement cmpFunc is only effective when creating a new tree, 
// otherwise the compare function of the root node will be used instead. 
RBTree insertRBTree (RBTree t, void* key, int keySize, void* value, int valueSize, int (*cmpFunc)(void*, void*));
RBTree insertLeftRBTree (RBTree t, void* key, int keySize, void* value, int valueSize, int (*cmpFunc)(void*, void*));

// Print the tree in ascending key order to a file. 
// A print function is needed to customize the format. 
void printRBTree (RBTree t, FILE* f, void (*printFunc)(FILE*, RBTree));

// Free the memory. 
// Special free function are needed to free the memory of special type of key or value.
// If key or value can be simply freed, use NULL for freeKeyFunc or freeValueFunc.
void freeRBTree (RBTree t, void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*));

#endif
