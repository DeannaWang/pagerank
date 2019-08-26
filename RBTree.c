/* 
    Implementation of red black tree ADT.
    A dictionary for any-type key-value pairs. Keys are in order and unique. 
    Acknowledge: algorithm of inserting nodes is from Introduction to Algorithms. 
    For COMP9024 assignment2.
    Written by Xiyan Wang.
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "RBTree.h"

typedef enum {RED, BLACK} Color;
typedef enum {LEFT, RIGHT} Direction;

typedef struct RBNode {
    void* key;
    void* value;
    Color color;
    int (*cmpFunc)(void*, void*); // Used to compare the keys.
    RBTree left;
    RBTree right;
    RBTree parent;
} RBNode;

// Create a new tree node. A compare function is needed to compare the keys. 
// Private function. A new tree can only be created by inserting to a NULL pointer. 
static RBTree newRBTree (void* key, int keySize, void* value, int valueSize, int (*cmpFunc)(void*, void*)) {
    RBTree new = malloc(sizeof(RBNode));
    assert(new != NULL);
    new->key = malloc(keySize);
    assert(new->key != NULL);
    memcpy(new->key, key, keySize);
    new->value = NULL;
    if (value != NULL) {
        new->value = malloc(valueSize);
        assert(new->value != NULL);
        memcpy(new->value, value, valueSize);
    }
    new->color = BLACK;
    new->cmpFunc = cmpFunc;
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;
    return new;
}

// Get the left child of tree. 
RBTree leftRBTree (RBTree t) {
    assert(t != NULL);
    return t->left;
}

// Get the right child of tree. 
RBTree rightRBTree (RBTree t) {
    assert(t != NULL);
    return t->right;
}

// Whether the tree node is red. 
static int isRed (RBTree t) {
    if (t == NULL) {
        return 0;
    }
    return t->color == RED;
}

// Find the node. If not found, return the parent. Return NULL only if tree is empty.
static RBTree searchRBTree (RBTree t, void* key) {
    if (t == NULL) {
        return NULL;
    }
    if (t->cmpFunc(key, t->key) < 0) {
        // If not found, return the parent. 
        if (t->left == NULL) {
            return t;
        }
        return searchRBTree (t->left, key);
    } else if (t->cmpFunc(key, t->key) > 0) {
        // If not found, return the parent. 
        if (t->right == NULL) {
            return t;
        }
        return searchRBTree (t->right, key);
    } else {
        // If found, return the corresponding node. 
        return t;
    }
}

// Whether the key exists. 
int keyExistsRBTree (RBTree t, void* key) {
    if (t == NULL) {
        return 0;
    }
    RBTree res = searchRBTree(t, key);
    if (t->cmpFunc(res->key, key) == 0) {
        return 1;
    }
    return 0;
}

// Get the node corresponding to the key. 
void* getTreeByKeyRBTree (RBTree t, void* key) {
    RBTree res = searchRBTree(t, key);
    if (res == NULL) {
        return NULL;
    } else if (t->cmpFunc(res->key, key) == 0) {
        // If found
        return res;
    }
    return NULL;
}

// Get function of value. 
void* getValueRBTree (RBTree t) {
    if (t == NULL) {
        return NULL;
    }
    return t->value;
}

// Set function of value. 
void setValueRBTree (RBTree t, void* value) {
    if (t == NULL) {
        return;
    }
    t->value = value;
}

// Get function of key. 
void* getKeyRBTree (RBTree t) {
    if (t == NULL) {
        return NULL;
    }
    return t->key;
}

// Tree rotation. 
static RBTree rotateRBTree (RBTree t, Direction d) {
    if (t == NULL) {
        return NULL;
    }
    RBTree res = NULL;
    if (d == LEFT) { // Left rotation. 
        if (t->right == NULL) {
            return t;
        }
        res = t->right;
        res->parent = t->parent;
        if (res->parent != NULL) {
            if (t == t->parent->left) { // t is a left child
                res->parent->left = res;
            } else { // t is a right child
                res->parent->right = res;
            }
        }
        t->right = res->left;
        if (t->right != NULL) {
            t->right->parent = t;
        }
        res->left = t;
        res->left->parent = res;
    } else { // Right rotation. 
        if (t->left == NULL) {
            return t;
        }
        res = t->left;
        res->parent = t->parent;
        if (res->parent != NULL) {
            if (t == t->parent->left) { // t is a left child
                res->parent->left = res;
            } else { // t is a right child
                res->parent->right = res;
            }
        }
        t->left = res->right;
        if (t->left != NULL) {
            t->left->parent = t;
        }
        res->right = t;
        res->right->parent = res;
    }
    return res;
}

// Balance the tree after inserting a node. Algorithm is from Introduction to Algorithms. 
static RBTree adjustRBTree (RBTree t, RBTree cur, RBTree father) {
    while (isRed(father)) { // O(log N)
        RBTree grandpa = father->parent;
        if (father == grandpa->left) {
            RBTree uncle = grandpa->right;
            if (isRed(uncle)) {
                father->color = BLACK;
                uncle->color = BLACK;
                grandpa->color = RED;
                cur = grandpa;
            } else if (cur == father->right) {
                rotateRBTree(father, LEFT);
                cur = father;
            } else {
                father->color = BLACK;
                grandpa->color = RED;
                RBTree ggrandpa = grandpa->parent;
                RBTree afterRotate = rotateRBTree(grandpa, RIGHT);
                if (ggrandpa == NULL) {
                    t = afterRotate;
                }
            }
        } else {
            RBTree uncle = grandpa->left;
            if (isRed(uncle)) {
                father->color = BLACK;
                uncle->color = BLACK;
                grandpa->color = RED;
                cur = grandpa;
            } else if (cur == father->left) {
                rotateRBTree(father, RIGHT);
                cur = father;
            } else {
                father->color = BLACK;
                grandpa->color = RED;
                RBTree ggrandpa = grandpa->parent;
                RBTree afterRotate = rotateRBTree(grandpa, LEFT);
                if (ggrandpa == NULL) {
                    t = afterRotate;
                }
            }
        }
        father = cur->parent;
        if (father == NULL) {
            cur->color = BLACK;
            t = cur;
        }
    }
    return t;
}

// Insert a key-value pair to the tree. If t is NULL, a new tree will be created.
// The arguement cmpFunc is only effective when creating a new tree, 
// otherwise the compare function of the root node will be used instead. 
// Time complexity: O(log N)
RBTree insertRBTree (RBTree t, void* key, int keySize, void* value, int valueSize, int (*cmpFunc)(void*, void*)) {
    // If t is NULL, create a new tree.
    if (t == NULL) {
        return newRBTree(key, keySize, value, valueSize, cmpFunc);
    }
    // Find the position to put the node. 
    RBTree pos = searchRBTree(t, key); // O(log N)
    int cmp = t->cmpFunc(key, pos->key);
    // If the key already exists, do nothing. 
    if (cmp == 0) {
        return t;
    }
    // Lay down the node. Use the cmpFunc of root instead of the function arguement. 
    RBTree new = newRBTree(key, keySize, value, valueSize, t->cmpFunc); 
    new->color = RED;
    new->parent = pos;
    if (cmp < 0) {
        pos->left = new;
    } else {
        pos->right = new;
    }
    // Balance the tree. 
    return adjustRBTree(t, new, pos); // O(log N)
}

// Print the tree in ascending key order to a file. 
// A print function is needed to customize the format. 
void printRBTree (RBTree t, FILE* f, void (*printFunc)(FILE*, RBTree)) {
    if (t == NULL) {
        return;
    }
    printRBTree(t->left, f, printFunc);
    printFunc(f, t);
    printRBTree(t->right, f, printFunc);
}

// Free the memory. 
// Special free functions are needed to free the memory of special types of key or value.
// If key or value can be simply freed, use NULL for freeKeyFunc or freeValueFunc.
void freeRBTree (RBTree t, void (*freeKeyFunc)(void*), void (*freeValueFunc)(void*)) {
    if (t == NULL) {
        return;
    }
    freeRBTree(t->left, freeKeyFunc, freeValueFunc);
    freeRBTree(t->right, freeKeyFunc, freeValueFunc);
    if (freeKeyFunc != NULL) {
        freeKeyFunc(t->key);
    } else {
        free(t->key);
    }
    if (freeValueFunc != NULL) {
        freeValueFunc(t->value);
    } else {
        free(t->value);
    }
    free(t);
}