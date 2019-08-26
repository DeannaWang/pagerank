/* 
    Interface to file reading functons.
    If startLine is NULL, search from the beginning of file. 
    If endLine is NULL or endLine not found, search to the end of file. 
    Result saved in res. 
    In getTokens, the arguement res passed to the function should be an existing list.
    In getTokenSet, res can be NULL.
    Both should be freed outside of the function.
    Written by Xiyan Wang for COMP9024 assignment2.
*/

#ifndef Token_H
#define Token_H

#include "DLList.h"
#include "RBTree.h"

#define MAX_LINE_TOKEN 1000
#define MAX_STRING_TOKEN 100
#define MAX_LONGLINE_TOKEN 10000

// Insert a string to a DLList. 
void insertDLListStr (DLList L, char* key);

// Read the content between startLine and endLine from a file named filename, 
// splite the content with any character from sep, and put the fragments into the list res. 
// Return the number of tokens found. 
int getTokens (char* filename, char* startLine, char* endLine, DLList res, char* sep);

// Read the content between startLine and endLine from a file named filename, 
// splite the content with any character from sep, and put the fragments into the RBTree res. 
// An optional string value can be added to each element. 
RBTree getTokensSet (char* filename, char* startLine, char* endLine, RBTree res, char* value, char* sep);

// Read a file by line and generate a tree, using the first token seperated by inLineSep as key, 
// and the rest tokens which generate a DLList as value.
RBTree getTokenSetByLine (char* filename, RBTree res, char* inLineSep);

#endif