
#include "instruction_list.h"
#include "error.h"

void InitListString(StringList *List){
    List->First = NULL;
    List->Actual = NULL;
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
    }
}

void DeleteFirstString(StringList *List){
    if(List->First != NULL){
        if(List->Actual == List->First)
            List->Actual = NULL;
    
        StringElementPtr elemPtr;
        elemPtr = List->First->ptr;
        free(List->First->data);
        free(List->First);
        List->First = elemPtr;
    }
}

void InitListInt(IntList *List){
    List->Actual = NULL;
    List->First = NULL;
}

void DisposeListInt(IntList *List){
    if(List->First != NULL)
    {
        while(List->First->ptr != NULL)
        {
            IntElementPtr elemPtr;
            elemPtr = List->First->ptr;
            List->First->ptr = elemPtr->ptr;
            free(elemPtr);
        }
        free(List->First);

        List->First = NULL;
        List->Actual = NULL;
    }
}

void InsertFirstInt(IntList *List, int ID){
    IntElementPtr newElemPtr = malloc(sizeof(struct IntElement));

    if(newElemPtr == NULL){
        return;
    }else{
        newElemPtr->data = ID;
        newElemPtr->ptr = List->First;
        List->First = newElemPtr;
    }
}

void DeleteFirstInt(IntList *List){
    if(List->First != NULL){
        if(List->Actual == List->First)
            List->Actual = NULL;
    
        IntElementPtr elemPtr;
        elemPtr = List->First->ptr;
        free(List->First);
        List->First = elemPtr;
    }
}