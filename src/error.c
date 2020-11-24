/*
 * */
#include "error.h"

int handle_error(ERROR error, char* msg){
    switch(error){
        case ERROR_LEX:
            fprintf(stderr,"Error: Lexical error: %s...\n", msg);
            break;
        case ERROR_SYN:
            fprintf(stderr,"Error: Syntax error: %s...\n", msg);
            break;
        case ERROR_SEM_DEF:
        case ERROR_SEM_DAT:
        case ERROR_SEM_COMP:
        case ERROR_SEM_PAR:
        case ERROR_SEM_OTHER:
            fprintf(stderr,"Error: Semantic error: %s...\n", msg);
            break;
        case ERROR_NULL:
            fprintf(stderr,"Error: Zero division: %s...\n", msg);
            break;
        case ERROR_TRANS:
            fprintf(stderr,"Error: Internal error occurred: %s...\n", msg);
            break;
        default:
            break;
    }
    return error;
}


