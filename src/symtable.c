/**
 * IFJ Projekt 2020
 *
 * Implementation of a symbol table
 *
 * @author <xsabol03> František Sabol
 */

#include <stdlib.h>
#include "symtable.h"
#include "error.h"

//djb2 algorithm
unsigned long hash(char* str){

    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}


void st_init(symbol_table_t* st_ptr){
    if(st_ptr != NULL){
        for(int i = 0; i < ST_SIZE; i++){
            (*st_ptr)[i] = NULL;
        }
    }
    else
        return;
}

st_item *st_item_alloc(const string *key, item_type type) {
    st_item* item = malloc(sizeof(st_item));
    if(item == NULL){
        return NULL;
    }
    else{
        if(st_item_init(item,key,type) != SUCCESS){
            free(item);
            return NULL;
        }
        else
            return item;
    }
}

int st_item_init(st_item* item, const string* key, item_type type){
    int error_code;

    if((error_code = str_init(&item->key)) != SUCCESS){
            return error_code;
    }
    
    if((error_code = str_copy(&item->key,key)) != SUCCESS){
        str_free(&item->key);
        return error_code;
    }
    item->next = NULL;
    item->data.type = type;
    item->data.defined = false;

    if(type == type_function){//function
        if((error_code = str_init(&item->data.as.function.params)) != SUCCESS){
                str_free(&item->key);
                return error_code;
        }
        if((error_code = str_init(&item->data.as.function.ret_types)) != SUCCESS){
            str_free(&item->data.as.function.params);
            str_free(&item->key);
            return error_code;
        }
        if((error_code =  str_init(&item->data.as.function.param_types)) != SUCCESS){
            str_free(&item->data.as.function.params);
            str_free(&item->data.as.function.ret_types);
            str_free(&item->key);
            return error_code;
        }

    }
    else{//variable
        item->data.as.variable.value_type = -1;
    }
    
    return error_code;
}

void st_item_free(st_item* item){
    if(item == NULL) return;
    if(item->data.type == type_function){
        str_free(&item->data.as.function.params);
        str_free(&item->data.as.function.param_types);
        str_free(&item->data.as.function.ret_types);
    }
    else{
        ;
    }
    str_free(&item->key);
    free(item);
}

st_item* st_get_item(symbol_table_t *st, const string* key){
    unsigned long hash_code = hash(key->str) % ST_SIZE;
    st_item* item = (*st)[hash_code];
    while(item != NULL && str_cmp(&item->key,key)){
        item=item->next;
    }
    return item; //NULL or desired item
}


bool st_search(symbol_table_t *st, const string* key) {
    st_item* item = st_get_item(st,key);
    if(item != NULL)
        return true;
    else
        return false;
}

st_item *st_insert(symbol_table_t *st, const string *key, item_type type) {
    unsigned long hash_code = hash(key->str) % ST_SIZE;
    st_item* replace_item = (*st)[hash_code];
    st_item* prev_item = NULL;
    while(replace_item != NULL && str_cmp(key,&replace_item->key)){
        prev_item = replace_item;
        replace_item = replace_item->next;
    }
    if(replace_item != NULL){//item found
        return replace_item;
    }
    else{//item not found
        st_item* new_item = NULL;
        if((new_item = st_item_alloc(key, type)) == NULL){
            return NULL;
        }
        else if(prev_item != NULL){
            prev_item->next = new_item;
        }
        else
            (*st)[hash_code] = new_item;

        return new_item;
    }
}

bool st_del_item(symbol_table_t* st, const string *key){
    unsigned long hash_code = hash(key->str) % ST_SIZE;
    st_item* del_item;
    del_item = (*st)[hash_code];
    if(del_item == NULL){//item doesn't exist
        return false;
    }
    else{//collision or item found
        st_item* prev_item = NULL;
        while(del_item != NULL && str_cmp(key,&del_item->key)){
            prev_item = del_item;
            del_item = del_item->next;
        }
        if(del_item != NULL){//item found
            if(prev_item == NULL)//item found at the front
                (*st)[hash_code] = del_item->next;
            else                //item found somewhere in the chain
                prev_item->next = del_item->next;

            st_item_free(del_item);
            return true;
        }
        else{//item doesn't exist
            return false;
        }
    }
}

void st_dispose(symbol_table_t* st){
    st_item* item;
    st_item* del_item;
    for(int i = 0; i < ST_SIZE; i++){
        item = (*st)[i];
        while(item != NULL){
            del_item = item;
            item = item->next;
            st_item_free(del_item);
        }
    }
    //free(st); not on heap currently
}

int enter_scope(st_stack_t** s, int *n){
    st_stack_t* new;

    if(s == NULL)
        return ERROR_TRANS;

    if((new = malloc(sizeof(st_stack_t))) == NULL)
        return ERROR_TRANS;

    st_init(&new->local_table);
    new->parent = NULL;
    new->scope = *n;

    if(*s == NULL){//entered the first scope
       *s = new;
    }
    else{
        new->parent = *s;
        *s = new;
    }

    (*n)++;
    return SUCCESS;
}

int leave_scope(st_stack_t **s, int *n) {
    st_stack_t* del;
    if(s == NULL)
        return ERROR_TRANS;
    if(*s == NULL)
        return ERROR_TRANS;

    del = *s;
    *s = del->parent;
    st_dispose(&del->local_table);
    free(del);
    if(*s == NULL)
        *n = 0;
    return SUCCESS;
}

st_item* stack_lookup(st_stack_t* s, const string* key){
    st_item* item;
    st_stack_t* parent;

    if(s == NULL)
        return NULL;

    parent = s->parent;
    item = st_get_item(&s->local_table, key);
    while(parent != NULL && item == NULL){
        item = st_get_item(&parent->local_table, key);
        parent = parent->parent;
    }

    return item; //NULL if not found in any of the symbol tables
}




