/* 
    Part B of COMP9024 assignment2. 
    Written by Xiyan Wang. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Token.h"

// Print function for a tree node, whose key is a string and value is a RBTree of strings.
void printTreeFuncWordTree (FILE* f, RBTree t);
// Free function for the value of a RBTree whose value is a RBTree.
void freeValueFuncRBTree (void* value);

int main (int argc, char** argv) {
    // Get urls
    DLList urlList = newDLList(MAX_STRING_TOKEN);
    getTokens("collection.txt", NULL, NULL, urlList, " \r\n");

    // Get wordTree
    RBTree wordTree = NULL;
    char* curUrl = NULL;
    resetDLList(urlList);
    while ((curUrl = nextDLList(urlList)) != NULL) {
        char filename[MAX_STRING_TOKEN];
        memset(filename, 0, MAX_STRING_TOKEN);
        strcpy(filename, curUrl);
        strcpy(filename + strlen(curUrl), ".txt");
        wordTree = getTokensSet(filename, "#start Section-2", "#end Section-2", wordTree, curUrl, " .,;?\r\n");
    }

    // Write the result
    FILE* f;
    assert((f = fopen("invertedIndex.txt", "w")) != NULL);
    printRBTree(wordTree, f, printTreeFuncWordTree);
    fclose(f);
    
    // Free the memory
    freeDLList(urlList, NULL);
    freeRBTree(wordTree, NULL, freeValueFuncRBTree);

    return EXIT_SUCCESS;
}

void printTreeFuncWordTree (FILE* f, RBTree t) {
    char* key = getKeyRBTree(t);
    RBTree* value = getValueRBTree(t);
    fprintf(f, "%s ", key);
    if (value != NULL) {
        printRBTree(*value, f, printTreeFuncWordTree);
        fprintf(f, "\n");
    }
}

void freeValueFuncRBTree (void* value) {
    RBTree* pTree = value;
    freeRBTree(*pTree, NULL, NULL);
    free(value);
}