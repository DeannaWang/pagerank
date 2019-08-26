/* 
    Part C of COMP9024 assignment2. 
    Written by Xiyan Wang. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "Token.h"

#define OUTPUT_NUMBER_SEARCHRESULT 30

typedef struct _searchRes {
    // url, pagerank, and the number of search items the page of url contains.
    char name[MAX_STRING_TOKEN];
    double rank;
    int itemNum;
} SearchRes;

// Compare function used to sort SearchRes type by name item. 
int cmpFuncUrl (void* k1, void* k2);
// Compare function used to sort SearchRes type for the result. 
// First itemNum descending, then rank descending, then name ascending.
int cmpFuncSearchRes (void* k1, void* k2);
// Build the result tree, using cmpFuncSearchRes.
RBTree buildResTree (RBTree t, RBTree res);
// Write the result to a certain file. 
int printResTree (FILE* f, RBTree t, int count);
// Free the value of invertedTree
void freeValueFuncInvertedTree (void* value);

int main (int argc, char** argv) {
    // Prepare items
    if (argc < 2) {
        return 0;
    }
    int i, j;
    for (i=1; i<argc; i++) {
        for (j=1; argv[i][j]; j++) {
            argv[i][j] = tolower((unsigned char)argv[i][j]);
        }
    }

    // Read pagerankList
    DLList pagerankList = newDLList(MAX_STRING_TOKEN);
    getTokens("pagerankList.txt", NULL, NULL, pagerankList, "\n");
    resetDLList(pagerankList);
    RBTree pagerankTree = NULL;
    char* line = NULL;
    while ((line = nextDLList(pagerankList)) != NULL) {
        SearchRes searchRes;
        memset(&searchRes, 0, sizeof(SearchRes));
        strcpy(searchRes.name, strtok(line, ", "));
        strtok(NULL, ", ");
        searchRes.rank = strtod(strtok(NULL, ", "), NULL);
        pagerankTree = insertRBTree(pagerankTree, &searchRes, sizeof(SearchRes), NULL, 0, cmpFuncUrl);
    }
    freeDLList(pagerankList, NULL);

    // Read invertedIndex and calculate itemNum of all the urls
    RBTree invertedTree = getTokenSetByLine("invertedIndex.txt", NULL, " ");
    for (i=1; i<argc; i++) {
        RBTree matchTree = getTreeByKeyRBTree(invertedTree, argv[i]); // O(log W)
        if (matchTree == NULL) {
            continue;
        }
        DLList* pUrlList = getValueRBTree(matchTree);
        resetDLList(*pUrlList);
        char* url = NULL;
        SearchRes searchRes;
        while ((url = nextDLList(*pUrlList)) != NULL) { // O(N)
            memset(searchRes.name, 0, MAX_STRING_TOKEN);
            strcpy(searchRes.name, url);
            matchTree = getTreeByKeyRBTree(pagerankTree, &searchRes); // O(log N)
            if (matchTree != NULL) {
                SearchRes* matchRes = getKeyRBTree(matchTree);
                matchRes->itemNum++;
            }
        }
    }
    freeRBTree(invertedTree, NULL, freeValueFuncInvertedTree);

    // Sort the result by using a tree
    RBTree resultTree = buildResTree(pagerankTree, NULL); // O(N * log N)

    // Write the result
    printResTree(NULL, resultTree, 0);

    // Free the memory
    freeRBTree(pagerankTree, NULL, NULL);
    freeRBTree(resultTree, NULL, NULL);

    return EXIT_SUCCESS;
}

int cmpFuncUrl (void* k1, void* k2) {
    SearchRes* s1 = (SearchRes*)k1;
    SearchRes* s2 = (SearchRes*)k2;
    return strcmp(s1->name, s2->name);
}

int cmpFuncSearchRes (void* k1, void* k2) {
    SearchRes* s1 = (SearchRes*)k1;
    SearchRes* s2 = (SearchRes*)k2;
    if (s1->itemNum != s2->itemNum) {
        return s2->itemNum - s1->itemNum;
    } else if (s1->rank < s2->rank) {
        return 1;
    } else if (s1->rank > s2->rank) {
        return -1;
    } else {
        return strcmp(s1->name, s2->name);
    }
}

RBTree buildResTree (RBTree t, RBTree res) {
    if (t == NULL) {
        return res;
    }
    res = buildResTree(leftRBTree(t), res);
    res = insertRBTree(res, getKeyRBTree(t), sizeof(SearchRes), NULL, 0, cmpFuncSearchRes);
    res = buildResTree(rightRBTree(t), res);
    return res;
}

int printResTree (FILE* f, RBTree t, int count) {
    if (count == OUTPUT_NUMBER_SEARCHRESULT) {
        return OUTPUT_NUMBER_SEARCHRESULT;
    }
    if (t == NULL) {
        return count;
    }
    count = printResTree(NULL, leftRBTree(t), count);
    if (count == OUTPUT_NUMBER_SEARCHRESULT) {
        return OUTPUT_NUMBER_SEARCHRESULT;
    }
    SearchRes* s = getKeyRBTree(t);
    if (s->itemNum > 0) {
        printf("%s\n", s->name);
        count++;
    } else {
        return OUTPUT_NUMBER_SEARCHRESULT;
    }
    count = printResTree(f, rightRBTree(t), count);
    return count;
}

void freeValueFuncInvertedTree (void* value) {
    DLList* pList = value;
    freeDLList(*pList, NULL);
    free(value);
}