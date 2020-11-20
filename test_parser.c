//
// Created by Frantisek on 18.11.2020.
//

#include "error.h"
#include "parser.h"
#include <string.h>

extern int token_pos;

void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

token_type array[100] = {TOKEN_PACKAGE, TOKEN_ID, TOKEN_EOL,
                         TOKEN_FUNC, TOKEN_ID, TOKEN_LBRACKET, TOKEN_ID, TOKEN_INT, TOKEN_RBRACKET,TOKEN_LBRACKET, TOKEN_INT, TOKEN_COMMA, TOKEN_INT, TOKEN_RBRACKET, TOKEN_LCURLY, TOKEN_EOL,
                         TOKEN_ID, TOKEN_ASSIGN, TOKEN_ID ,TOKEN_EOL,
                         TOKEN_FOR, TOKEN_ID,TOKEN_DEFINITION,TOKEN_ERROR,TOKEN_SEMICOLON,TOKEN_ERROR, TOKEN_SEMICOLON,TOKEN_ID,TOKEN_ASSIGN,TOKEN_ID,TOKEN_LCURLY,TOKEN_EOL,
                         TOKEN_RCURLY,
                         TOKEN_EOL, TOKEN_EOL,
                         TOKEN_RCURLY, TOKEN_EOL, TOKEN_EOL,
                         TOKEN_FUNC, TOKEN_ID, TOKEN_LBRACKET,TOKEN_RBRACKET,TOKEN_LCURLY,TOKEN_EOL,
                         TOKEN_RCURLY, TOKEN_EOL,
                         TOKEN_EOF};


int get_next_token(token_t* token){

    token->type = array[token_pos++];
    if(array[token_pos-2] == TOKEN_PACKAGE)
        strcpy(token->attribute.str,"main");
    else
        rand_str(token->attribute.str,5);
    
    return SUCCESS;

}
int main(){
    printf("Done\n");
    int ret = parser();
    if(ret)
        printf("Parsing error_code = %i\n", ret);
    else
        printf("Parsing Successful!!");
    return 0;
}