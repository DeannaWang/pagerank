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
#include "Tree.h"
#include <math.h>

typedef enum {RED, BLACK} Color;
typedef enum {LEFT, RIGHT} Direction;

typedef struct _Pair {
    void* left;
    void* right;
    int flag;
} Pair;

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

RBTree insertLeftRBTree (RBTree t, void* key, int keySize, void* value, int valueSize, int (*cmpFunc)(void*, void*)) {
    if (t == NULL) {
        return newRBTree(key, keySize, value, valueSize, cmpFunc);
    }
    // Lay down the node. Use the cmpFunc of root instead of the function arguement. 
    RBTree new = newRBTree(key, keySize, value, valueSize, t->cmpFunc); 
    new->color = RED;
    new->parent = NULL;
	new->left = t;
	return new;
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

int height (RBTree t) {
    if (t == NULL) {
	return -1;
    }
    int leftHeight = height(t->left);
    int rightHeight = height(t->right);
    return leftHeight > rightHeight ? leftHeight + 1: rightHeight + 1;
}

int inPair (RBTree t, void* key, Pair p) {
    if (t == NULL) {
	return 0;
    }
    if ((p.left == NULL && p.right == NULL) ||
	    (p.left == NULL && t->cmpFunc(p.right, key) > 0) ||
	    (p.right == NULL && t->cmpFunc(p.left, key) < 0) ||
	    (t->cmpFunc(p.left, key) < 0 && t->cmpFunc(p.right, key) > 0)) {
	return 1;
    }
    return 0;
}

Pair _isBST (RBTree t) {
    Pair p;
    p.left = NULL;
    p.right = NULL;
    p.flag = 1;
    if (t == NULL) {
	return p;
    }
    Pair leftPair = _isBST(t->left);
    if (leftPair.flag == 0) {
	p.flag = 0;
	return p;
    }
    Pair rightPair = _isBST(t->right);
    if (rightPair.flag == 0) {
	p.flag = 0;
	return p;
    }
    p.left = leftPair.right;
    p.right = rightPair.left;
    if (!inPair(t, t->key, p)) {
	p.flag = 0;
	return p;
    }
    p.left = leftPair.left == NULL ? t->key : leftPair.left;
    p.right = rightPair.right == NULL ? t->key : rightPair.right;
    return p;
}

int isBST (RBTree t) {
    return _isBST(t).flag;
}

void* _isBST2 (RBTree t, void* key, int start) {
    if (!start && key == NULL) {
        return NULL;
    }
    if (t == NULL) {
        return key;
    }
    key = _isBST2(t->left, key, start);
    if (!start && key == NULL) {
        return NULL;
    } else if (key == NULL) {
        key = t->key;
    } else if (t->cmpFunc(t->key, key) > 0) {
        key = t->key;
    } else {
        return NULL;
    }
    key = _isBST2(t->right, key, 0);
    return key;
}

int isBST2 (RBTree t) {
    if (_isBST2(t, NULL, 1) == NULL) {
        return 0;
    }
    return 1;
}

int _isBST3 (RBTree t, void* key, int start) {
    if (t == NULL) {
        return 1;
    }
    if (_isBST3(t->left, key, start) == 0) {
        return 0;
    }
    if (start || t->cmpFunc(t->key, key) > 0) {
        key = t->key;
    } else {
        return 0;
    }
    return _isBST3(t->right, key, 0);
}

int isBST3 (RBTree t) {
    return _isBST3(t, NULL, 1);
}

RBTree getEnds (RBTree t, Direction d) {
    if (t == NULL) {
	return t;
    }
    if (d == LEFT) {
		while(t->left != NULL) {
			t = t->left;
		}
	    return t;
    } else {
		while(t->right != NULL) {
			t = t->right;
		}
	    return t;
    }
}

void disturb (RBTree t) {
    RBTree small = getEnds(t, LEFT);
    RBTree large = getEnds(t, RIGHT);
	printf("small: %p, large: %p\n", small, large);
    if (small != NULL && large != NULL) {
		printf("small: %d, large: %d\n", *(int*)(small->key), *(int*)(large->key));
		RBTree tmp = small->key;
		small->key = large->key;
		large->key = tmp;
    }
}

int _isAVL (RBTree t) {
	if (t == NULL) {
		return -1;
	}
	int lh = _isAVL(t->left);
	int rh = _isAVL(t->right);
	if (lh == -2 || rh == -2 || abs(lh - rh) > 1) {
		return -2;
	}
	return lh > rh ? lh + 1 : rh + 1;
}

int isAVL (RBTree t) {
	if (_isAVL(t) == -2) {
		return 0;
	}
	return 1;
}
