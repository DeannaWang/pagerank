/* 
    Part A of COMP9024 assignment2. 
    Written by Xiyan Wang. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "Token.h"
#include "Graph.h"

typedef struct _page {
    // url, pagerank, and out degree of the url.
    char name[MAX_STRING_TOKEN];
    double rank[2];
    int outDegree;
} Page;

// Get the index of Page item whose name corresponding to a key in a Page array.
int getIndex (Page* pages, int num, char* key);
// Compare function used to sort the result. Arguments should be pointers to Page type.
int cmpFuncPage (void* k1, void* k2);
// Print function for a tree node, whose key is Page type. 
void printTreeFuncPageTree (FILE* f, RBTree t);

int main (int argc, char** argv) {
    // Get arguements
    if (argc != 4) {
        return 0;
    }
    double d = strtod(argv[1], NULL);
    double diffPR = strtod(argv[2], NULL);
    int maxIterations = atoi(argv[3]);

    // Get urls
    DLList urlList = newDLList(MAX_STRING_TOKEN);
    int urlNum = getTokens("collection.txt", NULL, NULL, urlList, " \r\n");
    if (urlNum < 2) {
        FILE* f;
        assert((f = fopen("pagerankList.txt", "w")) != NULL);
        resetDLList(urlList);
        char* curUrl = NULL;
        if ((curUrl = nextDLList(urlList)) != NULL) {
            fprintf(f, "%s, %d, %.7f\n", curUrl, 0, 1.0);
        }
        fclose(f);
        freeDLList(urlList, NULL);
        return EXIT_SUCCESS;
    }

    // Initialize page array
    Page* pages = malloc(urlNum * sizeof(Page));
    assert(pages);
    memset(pages, 0, urlNum * sizeof(Page));
    resetDLList(urlList);
    int i = 0;
    char* curUrl = NULL;
    while ((curUrl = nextDLList(urlList)) != NULL) {
        strcpy(pages[i].name, curUrl);
        // Initialize pagerank
        pages[i].rank[0] = 1.0 / urlNum;
        pages[i].rank[1] = (1 - d) / urlNum;
        i++;
    }
    freeDLList(urlList, NULL);

    // Initialize graph and insert edges, getting the outdegree of urls
    char filename[MAX_STRING_TOKEN];
    memset(filename, 0, MAX_STRING_TOKEN);
    Graph pageGraph = newGraph(urlNum);
    for (i = 0; i<urlNum; i++) {
        strcpy(filename, pages[i].name);
        strcpy(filename + strlen(pages[i].name), ".txt");
        DLList outUrlList = newDLList(MAX_STRING_TOKEN);
        getTokens(filename, "#start Section-1", "#end Section-1", outUrlList, " \r\n");
        resetDLList(outUrlList);
        int j = 0;
        char* outUrl = NULL;
        while ((outUrl = nextDLList(outUrlList)) != NULL) {
            Edge e;
            e.v = i;
            e.w = getIndex(pages, urlNum, outUrl);
            insertEdge(pageGraph, e);
            j++;
        }
        pages[i].outDegree = j;
        freeDLList(outUrlList, NULL);
    }

    // Calculate page rank
    int rankIndex = 1;
    double diff = 1;
    for (i=0; diff >= diffPR && i < maxIterations; i++) {
        diff = 0;
        int j;
        for (j=0; j<urlNum; j++) {
            DLList adjList = adjacentList(pageGraph, j);
            if (nitemsDLList(adjList) == 0) { // Edge: j -> all
                int k;
                for (k=0; k<urlNum; k++) {
                    if (k == j) {
                        continue;
                    }
                    pages[k].rank[rankIndex] += d * pages[j].rank[1 - rankIndex] / (urlNum - 1);
                }
            } else {
                resetDLList(adjList);
                int* k = NULL;
                while ((k = nextDLList(adjList)) != NULL) { // Edge: j -> k
                    pages[*k].rank[rankIndex] += d * pages[j].rank[1 - rankIndex] / pages[j].outDegree;
                }
            }
        }
        rankIndex = 1 - rankIndex;
        for (j=0; j<urlNum; j++) {
            diff += fabs(pages[j].rank[1 - rankIndex] - pages[j].rank[rankIndex]);
            pages[j].rank[rankIndex] = (1 - d) / urlNum;
        }
    }

    // Write the result
    if (rankIndex == 0) {
        for (i=0; i<urlNum; i++) {
            pages[i].rank[0] = pages[i].rank[1];
        }
    }
    RBTree outputTree = NULL;
    for (i=0; i<urlNum; i++) {
        outputTree = insertRBTree(outputTree, pages + i, sizeof(Page), NULL, 0, cmpFuncPage);
    }
    FILE* f;
    assert((f = fopen("pagerankList.txt", "w")) != NULL);
    printRBTree(outputTree, f, printTreeFuncPageTree);
    fclose(f);

    // Free the memory
    free(pages);
    freeRBTree(outputTree, NULL, NULL);
    freeGraph(pageGraph);

    return EXIT_SUCCESS;
}

int getIndex (Page* pages, int num, char* key) {
    int i;
    for (i=0; i<num; i++) {
        if (strcmp(pages[i].name, key) == 0) {
            return i;
        }
    }
    return -1;
}

int cmpFuncPage (void* k1, void* k2) {
    Page* p1 = (Page*)k1;
    Page* p2 = (Page*)k2;
    if (p1->rank[0] == p2->rank[0]) {
        return strcmp(p1->name, p2->name);
    }
    if (p1->rank[0] > p2->rank[0]) {
        return -1;
    } else {
        return 1;
    }
}

void printTreeFuncPageTree (FILE* f, RBTree t) {
    assert(t != NULL);
    Page* p = getKeyRBTree(t);
    fprintf(f, "%s, %d, %.7f\n", p->name, p->outDegree, p->rank[0]);
}