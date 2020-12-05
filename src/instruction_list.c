
#include "instruction_list.h"
#include "error.h"

void InitListString(StringList *List){
    List->First = NULL;
    List->Actual = NULL;
    List->Last = NULL;
}

void DisposeListString(StringList *List){
    if(List->First != NULL)
    {
        while(List->First->ptr != NULL)
        {
            StringElementPtr elemPtr;
            elemPtr = List->First->ptr;
            List->First->ptr = elemPtr->ptr;
            free(elemPtr->data);
            free(elemPtr);
        }
        free(List->First->data);
        free(List->First);

        List->First = NULL;
        List->Actual = NULL;
        List->Last = NULL;
    }
}

void InsertFirstString(StringList *List, char *val){
    StringElementPtr newElemPtr = malloc(sizeof(struct StringElement));

    if(newElemPtr == NULL){
        return;
    }else{
        newElemPtr->data = val;
        newElemPtr->ptr = List->First;
        List->First = newElemPtr;
        if(List->Last == NULL){
            List->Last = List->First;
        }
    }
}

void InsertLastString(StringList *List, char *val){
    StringElementPtr newElemPtr = malloc(sizeof(struct StringElement));

    if(newElemPtr == NULL){
        return;
    }else
    {
        newElemPtr->data = val;
        newElemPtr->ptr = NULL;
        if(List->Last != NULL)
            List->Last->ptr = newElemPtr;
        List->Last = newElemPtr;
        if(List->First == NULL){
            List->First = List->Last;
        }
    }
    
}

void DeleteFirstString(StringList *List){
    StringElementPtr elemPtr;

    if(List->First != NULL){
        if(List->Actual == List->First)
            List->Actual = NULL;

        elemPtr = List->First->ptr;
        free(List->First->data);
        free(List->First);
        List->First = elemPtr;

        if(List->First == NULL)
            List->Last = NULL;
    }
}