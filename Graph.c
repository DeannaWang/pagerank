/* 
    Implementation of adjacent list graph ADT.
    For COMP9024 assignment2.
    Written by Xiyan Wang.
*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Graph.h"

typedef struct GraphRep {
    // The number of nodes in the list,
    // the fist node and the last node.
    int nV;
    int nE;
    DLList* edges;
} GraphRep;

// Create a new graph.
Graph newGraph (int nV) {
    Graph new = malloc(sizeof(GraphRep));
    assert(new);
    new->edges = malloc(nV * sizeof(DLList));
    assert(new->edges);
    int i;
    for (i=0; i<nV; i++) {
        new->edges[i] = newDLList(sizeof(int));
    }
    new->nV = nV;
    new->nE = 0;
    return new;
}

// Wheter the vertex is valid.
int isValid (Graph g, Vertex v) {
    assert(g != NULL);
    if (v >= 0 && v < g->nV) {
        return 1;
    }
    return 0;
}

// Whether edge v -> w exists.
int isAdjacent (Graph g, Vertex v, Vertex w) {
    if (!isValid(g, v) || !isValid(g, w)) {
        return 0;
    }
    return keyExistsDLList(g->edges[v], &w);
}

// Return the adjacent list of vertex v. 
DLList adjacentList (Graph g, Vertex v) {
    assert(g != NULL);
    return g->edges[v];
}

// Insert edge.
void insertEdge (Graph g, Edge e) {
    assert(isValid(g, e.v) && isValid(g, e.w));
    insertDLList(g->edges[e.v], &(e.w));
    insertDLList(g->edges[e.w], &(e.v));
    g->nE++;
}

// Free the memory.
void freeGraph (Graph g) {
    int i;
    for (i=0; i<g->nV; i++) {
        freeDLList(g->edges[i], NULL);
    }
    free(g);
}

int tryHamilton (Graph g, Vertex v, Vertex dest, int* visited, int d) {
    assert(g != NULL);
    if (d == 0 && dest == v) {
        return v;
    } else if (dest == v) {
        return -1;
    }
    DLList adjacentList = g->edges[v];
    resetDLList(adjacentList);
    Vertex* w = NULL;
    while ((w = nextDLList(adjacentList)) != NULL) {
        if (visited[*w] != -1) {
            continue;
        }
        visited[*w] = v;
        Vertex res = -1;
        if ((res = tryHamilton(g, *w, dest, visited, d - 1)) != -1){
            return res;
        }
        visited[*w] = -1;
    }
    return -1;
}

int hasHamilton (Graph g, Vertex v, Vertex dest) {
    assert(g != NULL && isValid(g, v) && isValid(g, dest));
    int* visited = malloc(sizeof(int) * g->nV);
    assert(visited != NULL);
    int i;
    for (i=0; i<g->nV; i++) {
        visited[i] = -1;
    }
    visited[v] = -2;
    int res = tryHamilton(g, v, dest, visited, g->nV - 1);
    if (res != -1) {
        Vertex curV = res;
        printf("Graph has hamilton path: \n%d", curV);
        while((curV = visited[curV]) != -2) {
            printf(" - %d", curV);
        }
        printf("\n");
    } else {
        printf("Graph does not have hamilton path.\n");
    }
    free(visited);
    return res == -1 ? 0 : 1;
}