
#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#endif //IFJ_PARSER_H

#include "scanner.h"
#include "symtable.h"


typedef struct parser_info{
    int error_code;

    symbol_table_t st; //global symbol table  - only functions
    st_stack_t* local_st; //stack of symbol tables for currently proccessed function, current scope always first
    token_t* token; //active token
    token_list_t token_list; //list of tokens
    st_item* in_function; //pointer to the function which definition we are currently parsing
    int scope; //current scope number

    string left_side_vars_types; // v1,v2,...,v_n = ...
    string right_side_exp_types; // ... = exp1,exp2,...,exp_n | call params types
    st_item* function_called; // ... = f()

    //possibly more stuff coming

}parser_info;
bool is_data_type(token_type type);
int set_data_type(parser_info *p, token_t *token, data_type *type, bool throw_error_on_default);
int add_symbol(symbol_table_t* table,token_t* token , item_type type);
int fill_st_with_builtin(symbol_table_t* st);
int check_main(symbol_table_t* st);
int check_types(string* s1, string* s2);
int get_next_token(parser_info* p);
int first_pass(parser_info* p);
int parser();
