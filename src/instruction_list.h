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
    StringElementPtr Last;
} StringList;

void InitListString(StringList *);
void DisposeListString(StringList *);
void InsertFirstString(StringList *, char *);
void InsertLastString(StringList *List, char *val);
void DeleteFirstString(StringList *);

#endif //IFJ_PROJECT_INSTRUCTION_LIST_H