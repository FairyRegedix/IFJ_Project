
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "str.h"


int str_init(string *s){
    if ((s->str = (char*) malloc(BLOCK_SIZE*sizeof(char))) == NULL)
        return ERROR_STR;
    s->str[0] = '\0';
    s->len = 0;
    s->size = BLOCK_SIZE;
    return SUCCESS;
}

void str_free(string *s){
    free(s->str);
    s->str = NULL; //not initialized state
    s->len = -1; //not initialized state
    s->size= 0; //not initialized state
}

void str_reinit(string *s){
    s->str[0] = '\0';
    s->len = 0;
}

int str_add_char(string *s1, char c){
    if (s1->len + 1 >= s1->size){
        if ((s1->str = (char*) realloc(s1->str, (s1->len + BLOCK_SIZE)*sizeof(char))) == NULL)
            return ERROR_STR;
        s1->size = s1->len + BLOCK_SIZE;
    }
    s1->str[s1->len] = c;
    s1->len++;
    s1->str[s1->len] = '\0';
    return SUCCESS;
}

string c_str_to_str(const char* s){
    char c;
    int i = 0;
    string str;
    str_init(&str);

    if(s == NULL)
        return str;

    while((c = s[i++]) != '\0'){
        if(str_add_char(&str,c) != SUCCESS){
            str_free(&str);
            return str;
        }
    }
    return str;
}

int str_copy(string *s1, const string *s2){
    if (s2->len >= s1->size){
        s1->str = (char*)realloc(s1->str, (s2->len + 1)*sizeof(char));
        if (s1->str == NULL)
            return ERROR_STR;
        s1->size = s2->len + 1;
    }
    strcpy(s1->str, s2->str);
    s1->len = s2->len;
    return SUCCESS;
}

int str_cmp(const string *s1, const string *s2){
    return strcmp(s1->str, s2->str);
}

int str_cmp_c_str(const string *s1, const char* s2){
    return strcmp(s1->str, s2);
}

char* str_to_c_str(const string *s){
    return s->str;
}

size_t str_len(const string *s){
    return s->len;
}

void InitList(StringList *List){
    List->First = NULL;
    List->Actual = NULL;
}

void DisposeList(StringList *List){
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

void InsertFirst(StringList *List, char *val){
    StringElementPtr newElemPtr = malloc(sizeof(struct StringElement));

    if(newElemPtr == NULL){
        return;
    }else{
        newElemPtr->data = val;
        newElemPtr->ptr = List->First;
        List->First = newElemPtr;
    }
}

void DeleteFirst(StringList *List){
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