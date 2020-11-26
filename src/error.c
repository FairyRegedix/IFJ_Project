/*
 * */
#include <stdarg.h>
#include "error.h"

int handle_error(ERROR error, char* format, ...){
    va_list args;
    char* msg;
    va_start(args, format);

    switch(error){
        case ERROR_LEX:
            msg="Lexical error: ";
            break;
        case ERROR_SYN:
            msg="Syntax error: ";
            break;
        case ERROR_SEM_DEF:
        case ERROR_SEM_DAT:
        case ERROR_SEM_COMP:
        case ERROR_SEM_PAR:
        case ERROR_SEM_OTHER:
            msg="Semantic error: ";
            break;
        case ERROR_NULL:
            msg="Zero division: ";
            break;
        case ERROR_TRANS:
            msg="Internal error occurred: ";
            break;
        default:
            msg = "";
            break;
    }
    fprintf(stderr,"Error[%i]: %s", error, msg);
    vfprintf(stderr, format, args);

    va_end(args);

    return error;
}


