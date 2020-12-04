#ifndef IFJ_PROJECT_INSTRUCTION_LIST_H
#define IFJ_PROJECT_INSTRUCTION_LIST_H

#include "libraries.h"

typedef struct StringElement{
    struct StringElement *ptr;
    char* data;
} *StringElementPtr;

typedef struct 
{
    StringElementPtr Actual;
    StringElementPtr First;
} StringList;

void InitListString(StringList *);
void DisposeListString(StringList *);
void InsertFirstString(StringList *, char *);
void DeleteFirstString(StringList *);

typedef struct IntElement{
    struct IntElement *ptr;
    unsigned int data;
} *IntElementPtr;

typedef struct{
    IntElementPtr Actual;
    IntElementPtr First;    
} IntList;

void InitListInt(IntList *);
void DisposeListInt (IntList *);
void InsertFirstInt(IntList *, int);
void DeleteFirstInt(IntList *);

#endif //IFJ_PROJECT_INSTRUCTION_LIST_H