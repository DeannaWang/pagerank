/* 
    Implementation of file reading functons.
    If startLine is NULL, search from the beginning of file. 
    If endLine is NULL or endLine not found, search to the end of file. 
    Result saved in res. 
    In getTokens, the arguement res passed to the function should be an existing list.
    In getTokenSet, res can be NULL.
    Both should be freed outside of the function.
    Written by Xiyan Wang for COMP9024 assignment2.
*/

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include "Token.h"

// Insert a string to a DLList. 
void insertDLListStr (DLList L, char* key) {
    assert(L != NULL);
    char tmp[MAX_STRING_TOKEN];
    memset(tmp, 0, MAX_STRING_TOKEN);
    strcpy(tmp, key);
    insertDLList(L, tmp);
}

// Alphabetical order. 
static int cmpFuncStr (void* k1, void* k2) {
    return strcmp((char*)k1, (char*)k2);
}

// Read the content between startLine and endLine from a file named filename, 
// splite the content with any character from sep, and put the fragments into the list res. 
// Return the number of elements inserted. 
int getTokens (char* filename, char* startLine, char* endLine, DLList res, char* sep) {
    assert(res != NULL);
    FILE* f;
    if ((f = fopen(filename, "r")) == NULL) {
        return 0;
    }
    char line[MAX_LINE_TOKEN];
    int startLineLen = 0;
    if (startLine != NULL) {
        startLineLen = strlen(startLine);
    }
    int endLineLen = 0;
    if (endLine != NULL) {
        endLineLen = strlen(endLine);
    }
    int processing = 0;
    if (startLine == NULL) {
        processing = 1;
    }
    int num = 0;
    while (fgets(line, MAX_LINE_TOKEN, f) != NULL) {
        if (!processing) {
            if (startLineLen != 0 && memcmp(line, startLine, startLineLen) == 0) {
                processing = 1;
            }
            continue;
        }
        if (endLineLen != 0 && memcmp(line, endLine, endLineLen) == 0) {
            break;
        }
        char* token = strtok(line, sep);
        do {
            if (token == NULL) {
                continue;
            }
            insertDLListStr(res, token);
            num++;
        } while ((token = strtok(NULL, sep)) != NULL);
    }
    fclose(f);
    return num;
}

// Read the content between startLine and endLine from a file named filename, 
// splite the content with any character from sep, and put the fragments into the RBTree res. 
// An optional string value can be added to each element. 
RBTree getTokensSet (char* filename, char* startLine, char* endLine, RBTree res, char* value, char* sep) {
    FILE* f;
    if ((f = fopen(filename, "r")) == NULL) {
        return res;
    }
    char line[MAX_LINE_TOKEN];
    int startLineLen = 0;
    if (startLine != NULL) {
        startLineLen = strlen(startLine);
    }
    int endLineLen = 0;
    if (endLine != NULL) {
        endLineLen = strlen(endLine);
    }
    int processing = 0;
    if (startLine == NULL) {
        processing = 1;
    }
    while (fgets(line, MAX_LINE_TOKEN, f) != NULL) {
        if (!processing) {
            if (startLineLen != 0 && memcmp(line, startLine, startLineLen) == 0) {
                processing = 1;
            }
            continue;
        }
        if (endLineLen != 0 && memcmp(line, endLine, endLineLen) == 0) {
            break;
        }
        char* token = strtok(line, sep);
        do {
            if (token == NULL) {
                continue;
            }
            int i = 0;
            for (i=0; token[i]; i++) {
                token[i] = tolower((unsigned char)token[i]);
            }
            if (keyExistsRBTree(res, token)) {
                if (value != NULL) {
                    RBTree curTree = getTreeByKeyRBTree(res, token);
                    RBTree* pValueTree = getValueRBTree(curTree);
                    *pValueTree = insertRBTree(*pValueTree, value, MAX_STRING_TOKEN, NULL, 0, cmpFuncStr);
                    setValueRBTree(curTree, pValueTree);
                }
            } else {
                if (value != NULL) {
                    RBTree valueTree = insertRBTree(NULL, value, MAX_STRING_TOKEN, NULL, 0, cmpFuncStr);
                    res = insertRBTree(res, token, MAX_STRING_TOKEN, &valueTree, sizeof(RBTree), cmpFuncStr);
                } else {
                    res = insertRBTree(res, token, MAX_STRING_TOKEN, NULL, 0, cmpFuncStr);
                }
            }
        } while ((token = strtok(NULL, sep)) != NULL);
    }
    fclose(f);
    return res;
}

// Read a file by line and generate a tree, using the first token seperated by inLineSep as key, 
// and the rest tokens which generate a DLList as value.
RBTree getTokenSetByLine (char* filename, RBTree res, char* inLineSep) {
    FILE* f;
    if ((f = fopen(filename, "r")) == NULL) {
        return res;
    }
    char line[MAX_LONGLINE_TOKEN];
    while (fgets(line, MAX_LONGLINE_TOKEN, f) != NULL) {
        int len = strlen(line);
        if (len == 0) {
            continue;
        }
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        char* token = strtok(line, inLineSep);
        if (token == NULL) {
            continue;
        }
        DLList contentList = newDLList(MAX_STRING_TOKEN);
        res = insertRBTree(res, token, MAX_STRING_TOKEN, &contentList, sizeof(DLList), cmpFuncStr);
        while ((token = strtok(NULL, inLineSep)) != NULL) {
            insertDLListStr(contentList, token);
        }
    }
    fclose(f);
    return res;
}