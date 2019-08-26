/*
    Interface to adjacent list graph ADT.
    For COMP9024 assignment2.
    Written by Xiyan Wang.
*/

#ifndef Graph_H
#define Graph_H

#include "DLList.h"

typedef struct GraphRep* Graph;
typedef int Vertex;
typedef struct _edge {
    Vertex v;
    Vertex w;
} Edge;

Graph newGraph (int);
int isValid (Graph, Vertex);
int isAdjacent (Graph, Vertex, Vertex);
DLList adjacentList (Graph, Vertex);
void insertEdge (Graph, Edge);
void freeGraph (Graph);
int hasHamilton (Graph g, Vertex v, Vertex dest);
void hasHamiltonGraph (Graph g);

#endif
