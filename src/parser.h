
#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#endif //IFJ_PARSER_H

#include "scanner.h"
#include "symtable.h"


typedef struct parser_info{
    int error_code;
    int internal_error;
    int syntax_error_token_position;

    symbol_table st;
    symbol_table local_st;
    token_t token; //current token

    st_item* in_function; //pointer to the function which definition we are currently parsing
    int scope; //current scope
    //possibly more stuff coming

}parser_info;

int parser();
