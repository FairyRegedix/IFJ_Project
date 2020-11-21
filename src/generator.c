//generator



#include "generator.h"

string code;

/*int add_to_code(char *s){
    if (!(str_add_str(&code, s))){
        return false;
    }
}*/

int generate_header(){
   
    printf(".IFJcode20\n");

    printf("JUMP $$main\n");

    printf("LABEL $$main # main body\n");
    printf("PUSHFRAME\n");

}

int generate_end_of_main(){
    printf("POPFRAME\n");
    printf("CLEARS\n");
}



int codeGenerator(token_type type, token_t *token){
    
    generate_header();

    switch (type)
    {
    case TOKEN_ID:
        
        break;
    case TOKEN_ASSIGN: 

        break;
    case TOKEN_DEFINITION:

        break;
    case TOKEN_PRINT:

        break;
    case TOKEN_STRING:

        break;
    case TOKEN_LEN:

        break;
    case TOKEN_ORD:

        break;
    case TOKEN_CHR:

        break;
    case TOKEN_PACKAGE:

        break;
    case TOKEN_UNDERSCORE:

        break;
    case TOKEN_STR:

        break;
    case TOKEN_INTEGER:

        break;
    case TOKEN_INT:

        break;
    case TOKEN_FLOAT:

        break;
    case TOKEN_FLOAT64:

        break;
    case TOKEN_EOL:

        break;
    case TOKEN_EOF:
    
        break;
    case TOKEN_COMMA:

        break;
    case TOKEN_SEMICOLON:

        break;
    case TOKEN_LBRACKET:

        break;
    case TOKEN_RBRACKET:

        break;
    case TOKEN_LCURLY:

        break;
    case TOKEN_RCURLY:

        break;
    
    case TOKEN_ADD:
    case TOKEN_SUB:
    case TOKEN_MUL:
    case TOKEN_DIV:


        break;
    
    case TOKEN_EQL:
    case TOKEN_NEQ:
    case TOKEN_LTE:
    case TOKEN_GTE:
    case TOKEN_LT:
    case TOKEN_GT:

        break;
    
    case TOKEN_NOT:
    case TOKEN_AND:
    case TOKEN_OR:


        break;

    case TOKEN_IF:

        break;
    case TOKEN_ELSE:

        break;    
    case TOKEN_FOR:
        break;
    
    case TOKEN_FUNC:
        break;
        
    case TOKEN_INPUTB:

        break;
    case TOKEN_INPUTI:

        break;
    case TOKEN_INPUTS:

        break;
    case TOKEN_INPUTF:

        break;
    case TOKEN_SUBSTR:
        
        break;
    case TOKEN_RETURN:

        break;
    default:
        break;
    }

    generate_end_of_main();

}

void main(){
    printf("starting generator \n");
    str_init(&code);
    printf("initialization done \n");
    generate_header();
    printf("Generated string: %c \n", *(&code)->str);
    return;
}
