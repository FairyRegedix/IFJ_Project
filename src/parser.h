
#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#endif //IFJ_PARSER_H

#include "scanner.h"
#include "symtable.h"


typedef struct parser_info{
    int error_code;
    int internal_error;

    symbol_table st;
    symbol_table local_st;
    token_t token; //current token

    //possibly more stuff coming

}parser_info;

