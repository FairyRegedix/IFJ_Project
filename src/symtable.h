
#ifndef IFJ_PROJECT_SYMTAB_H
#define IFJ_PROJECT_SYMTAB_H

//#include <stdlib.h>
//#include <stdbool.h>
//#include "str.h"
#include "scanner.h"
#define ST_SIZE 133



typedef enum data_type{
    type_int = TOKEN_INT,
    type_float = TOKEN_FLOAT64,
    type_str = TOKEN_STR,
    type_bool = TOKEN_BOOL,
} data_type;

typedef enum item_type{
    type_function,
    type_variable,
} item_type;

struct item;

typedef struct function_t{
    string params;
    string param_types;
    string ret_types;
} function_t; //function signature

typedef struct variable_t{
    data_type value_type; //data type of the variable

    union{
        int int_value; //also for bool value
        double float_value;
        string string_value;
    }value;
} variable_t; //variable signature

typedef struct item_data{
    item_type type; // function | var ?
    int scope;
    bool defined;
    bool built_in;
    union{
        function_t function;
        variable_t variable;
    }as;
}item_data;


typedef struct item{
    string key;  //identifier
    item_data data; //another information about symbol
    struct item* next;  //pointer to the next item in the list (chaining collisions)
}st_item;

typedef st_item* symbol_table[ST_SIZE];

typedef struct stack{
    symbol_table local_table;
    struct stack* parent;
}st_stack_t;


/* djb2 hashing algorithm
 * @param   s   : string to be hashed
 * @return  returns hash(index to symbol table)*/
unsigned long hash(char* s);

/*
 * Initializes a symbol table.
 * @param   st  :pointer to a symbol table to be initialized
 * */
void st_init(symbol_table* st);



/* Allocates and initializes dynamic memory for item and returns pointer to that initialized memory.
 * Helper function for st_item_init
 * For more info see st_item_init
 * @param   key         : pointer to a string (variable identifier)
 * @param   type        : item_type of the item for which the space is allocated
 * @param   error_code  : pointer to an error_code variable,
 *                        if everything is okay error_code = SUCCESS,
 *                        else error_code = ERROR_TRANS (internal error)
 * @return  pointer to a st_item*/
st_item* st_item_alloc(const string* key, item_type type, int* error_code);

/*
 * Initializes a new symbol table item.
 * @pre     Item pointer has to be a valid pointer to memory.
 * @post    Memory space for some item attributes will be allocated.
 * @param   item :pointer to item
 * @param   key  :id of function/variable
 * @param   type :type_function or type_variable?
 * @return  SUCCESS or ERROR_TRANS(internal error)
 * */
int st_item_init(st_item* item, const string* key, item_type type);

/*
 *@param    st  : pointer to a symbol table
 *@param    key : pointer to a key of an item to lookup
 *@return   returns pointer to an existing item or NULL otherwise
 * */
st_item* st_get_item(symbol_table *st, const string* key);

/*
 *@param    st  : pointer to a symbol table
 *@param    key : pointer to a key of an item to lookup
 *@return   true - found, false - not found
 * */
bool st_search(symbol_table *st, const string* key);

/*  deallocates dynamic memory of item
 * @param   item    : pointer to a symbol table item
 * */
void st_item_free(st_item* item);

/* Inserts new item to the symbol table
 * if item with the same key is already in the table it will be updated
 * @param   st          : pointer to a symbol table
 * @param   key         : pointer to a string (variable identifier)
 * @param   type        : item_type of the item to insert
 * @param   error_code  : pointer to a variable to pass error code if any internal error occurred
 * @return  pointer to a st_item or NULL
 * */
st_item* st_insert(symbol_table* st, const string* key, const item_type type, int* error_code);

/* Deletes an item with the given key from the symbol table
 * if an item with the given key is not found nothing happens
 * @param   st  : pointer to a symbol table
 * @param   key : pointer to a string
 * @return  true - item found and deleted, false - item not found nothing happens
 * */
bool st_del_item(symbol_table* st, const string *key);

/* Frees all dynamic memory used by a symbol table.
 * @param   st  : pointer to a symbol table
 * */
void st_dispose(symbol_table* st);

int enter_scope(st_stack_t** s, int *n);
int leave_scope(st_stack_t** s, int *n);
st_item* stack_lookup(st_stack_t* s, const string* key);
#endif //IFJ_PROJECT_SYMTAB_H
