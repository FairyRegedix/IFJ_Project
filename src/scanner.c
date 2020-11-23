//Scanner


#include "error.h"
#include "libraries.h"
#include "scanner.h"
#include "str.c"
#include "str.h"



// pomocna
int symbol;
FILE *f; //source of input
/*int getNextToken(token_t *token){

}*/


int print_token(token_t *token);


//check the keywords
void isKeyword(int *type, char *tmp) {
    if (strcmp(tmp, "if") == 0) {
        *type = TOKEN_IF;
    } else if (strcmp(tmp, "else") == 0) {
        *type = TOKEN_ELSE;
    } else if (strcmp(tmp, "package") == 0) {
        *type = TOKEN_PACKAGE;

    } else if (strcmp(tmp, "return") == 0) {
        *type = TOKEN_RETURN;
    } else if (strcmp(tmp, "for") == 0) {
        *type = TOKEN_FOR;
    } else if (strcmp(tmp, "func") == 0) {
        *type = TOKEN_FUNC;

    } else if (strcmp(tmp, "print") == 0) {
        *type = TOKEN_PRINT;
    } else if (strcmp(tmp, "inputi") == 0) {
        *type = TOKEN_INPUTI;
    } else if (strcmp(tmp, "inputs") == 0) {
        *type = TOKEN_INPUTS;
    } else if (strcmp(tmp, "len") == 0) {
        *type = TOKEN_LEN;
    } else if (strcmp(tmp, "substr") == 0) {
        *type = TOKEN_SUBSTR;
    } else if (strcmp(tmp, "ord") == 0) {
        *type = TOKEN_ORD;
    } else if (strcmp(tmp, "chr") == 0) {
        *type = TOKEN_CHR;
    } else if (strcmp(tmp, "inputb") == 0) {
        *type = TOKEN_INPUTB;
    } else if (strcmp(tmp, "inputf") == 0) {
        *type = TOKEN_INPUTF;
    } else if (strcmp(tmp, "bool") == 0) {
        *type = TOKEN_BOOL;
    } else if (strcmp(tmp, "true") == 0) {
        *type = TOKEN_TRUE;
    } else if (strcmp(tmp, "false") == 0) {
        *type = TOKEN_FALSE;
    } else if (strcmp(tmp, "int") == 0) {
        *type = TOKEN_INT;
    } else if (strcmp(tmp, "float64") == 0) {
        *type = TOKEN_FLOAT64;
    } else if (strcmp(tmp, "string") == 0) {
        *type = TOKEN_STRING;
    } else if (strcmp(tmp, "_") == 0) {
        *type = TOKEN_UNDERSCORE;
    }
    //miesto na dalsie mozne keywordy

}// end of isKeyword

int print_token(token_t *token) {
    switch (token->type) {
        case TOKEN_ERROR:
            printf("[ERROR]\n");
            break;
        case TOKEN_BOOL:
            printf("[BOOL]\n");
            break;
        case TOKEN_TRUE:
            printf("[true]\n");
            break;
        case TOKEN_FALSE:
            printf("[false]\n");
            break;
        case TOKEN_ELSE:
            printf("[else]\n");
            break;
        case TOKEN_FLOAT64:
            printf("[float64]\n");
            break;
        case TOKEN_FOR:
            printf("[for]\n");
            break;
        case TOKEN_FUNC:
            printf("[func]\n");
            break;
        case TOKEN_IF:
            printf("[if]\n");
            break;
        case TOKEN_INT:
            printf("[int]\n");
            break;
        case TOKEN_PACKAGE:
            printf("[package]\n");
            break;
        case TOKEN_RETURN:
            printf("[return]\n");
            break;
        case TOKEN_STRING:
            printf("[string]\n");
            break;
        case TOKEN_UNDERSCORE:
            printf("[_]\n");
            break;
        case TOKEN_PRINT:
            printf("[print]\n");
            break;
        case TOKEN_INPUTI:
            break;
        case TOKEN_INPUTS:
            break;
        case TOKEN_LEN:
            break;
        case TOKEN_SUBSTR:
            break;
        case TOKEN_ORD:
            break;
        case TOKEN_CHR:
            break;
        case TOKEN_INPUTB:
            break;
        case TOKEN_INPUTF:
            break;
        case TOKEN_ID:
            printf("[ID] \"%s\"\n", token->actual_value.str);
            break;
        case TOKEN_RBRACKET:
            printf("[)]\n");
            break;
        case TOKEN_LBRACKET:
            printf("[(]\n");
            break;
        case TOKEN_RCURLY:
            printf("[}]\n");
            break;
        case TOKEN_LCURLY:
            printf("[{]\n");
            break;
        case TOKEN_COMMA:
            printf("[,]\n");
            break;
        case TOKEN_SEMICOLON:
            printf("[;]\n");
            break;
        case TOKEN_EOL:
            printf("[EOL]\n");
            break;
        case TOKEN_EOF:
            printf("[EOF]\n");
            break;
        case TOKEN_DEFINITION:
            printf("[:=]\n");
            break;
        case TOKEN_ASSIGN:
            printf("[=]\n");
            break;
        case TOKEN_INTEGER:
            printf("[%s]\n",token->actual_value.str);
            break;
        case TOKEN_FLOAT:
            printf("[%s]\n",token->actual_value.str);
            break;
        case TOKEN_STR:
            printf("[\"%s\"]\n", token->actual_value.str);
            break;
        case TOKEN_NOT:
            printf("[!]\n");
            break;
        case TOKEN_AND:
            printf("[&&]\n");
            break;
        case TOKEN_OR:
            printf("[|]\n");
            break;
        case TOKEN_ADD:
            printf("[+]\n");
            break;
        case TOKEN_SUB:
            printf("[-]\n");
            break;
        case TOKEN_MUL:
            printf("[*]\n");
            break;
        case TOKEN_DIV:
            printf("[/]\n");
            break;
        case TOKEN_EQL:
            printf("[==]\n");
            break;
        case TOKEN_NEQ:
            printf("[!=]\n");
            break;
        case TOKEN_LT:
            printf("[<]\n");
            break;
        case TOKEN_GT:
            printf("[>]\n");
            break;
        case TOKEN_LTE:
            printf("[<=]\n");
            break;
        case TOKEN_GTE:
            printf("[>=]\n");
            break;
    }
    return 0;
}

int get_next_token(token_t *token){
    int error_code;
    str_reinit(&token->actual_value);
    f = stdin;

    char c = (char) getc(f);

    if(c == EOF){
        token->type = TOKEN_EOF;
        return SUCCESS;
    }

    while (isspace(c)){
        c = (char) getc(f);
        if(c == EOL){
            token->type = TOKEN_EOL;
            return SUCCESS;
        }

    }
    switch (c) {
            // +
        case '+' :
            token->type = TOKEN_ADD;
            break;
            // -
        case '-' :
            token->type = TOKEN_SUB;
            break;
            // *
        case '*' :
            token->type = TOKEN_MUL;
            break;
            // (
        case '(' :
            token->type = TOKEN_LBRACKET;
            break;
            // )
        case ')' :
            token->type = TOKEN_RBRACKET;
            break;
            // {
        case '{' :
            token->type = TOKEN_LCURLY;
            break;
            // }
        case '}' :
            token->type = TOKEN_RCURLY;
            break;
            // ,
        case ',' :
            token->type = TOKEN_COMMA;
            break;
            // ;
        case ';' :
            token->type = TOKEN_SEMICOLON;
            break;

        case '>' :
            c = (char) fgetc(f);
            // >=
            if (c == '=') {
                token->type = TOKEN_GTE;
                break;
            }
                // >
            else {
                ungetc(c, f);
                token->type = TOKEN_GT;
                break;
            }

        case '<' :
            c = (char) fgetc(f);
            // <=
            if (c == '=') {
                token->type = TOKEN_LTE;

                break;
            }
                // <
            else {
                ungetc(c, f);
                token->type = TOKEN_LT;

                break;
            }

        case '=' :
            c = (char) fgetc(f);
            // ==
            if (c == '=') {
                token->type = TOKEN_EQL;
                break;
            }
                // =
            else {
                ungetc(c, f);
                token->type = TOKEN_ASSIGN;
                break;
            }
        case ':':
            c = (char) fgetc(f);
            // :=
            if (c == '=') {
                token->type = TOKEN_DEFINITION;
                break;
            }
            else
                return ERROR_LEX;

        case '!' :
            c = (char) fgetc(f);
            // !=
            if (c == '=') {
                token->type = TOKEN_NEQ;
            } else {
                ungetc(c, f);
                token->type = TOKEN_NOT;
            }
            break;

        case '/':
            c = (char) fgetc(f);
            if (c == '/') {
                for (; (c != EOL && c != EOF); c = (char) fgetc(f)) { ; }
            }
            else if (c == '*'){
                while (true) {
                    c = (char) fgetc(f);
                    if (c == '*'){
                        c = (char) fgetc(f);
                        if (c == '/')
                            break;
                        else if (c == EOF)
                            return ERROR_LEX;
                        else {;}
                    }
                    else if (c == EOF)
                        return ERROR_LEX;
                    else{;}
                }
            }
            else{
                ungetc(c, f);
                token->type = TOKEN_DIV;
                break;
            }
            break;

        case '"':
            c = (char)getc(f);
            while(true){
                if(c == 92){// backslash "\"
                    int decimal = 0; //stores decimal value of hex escape squence found in string
                    c = (char)getc(f);
                    switch(c){
                        case '\\':
                            CHECK(str_concat(&token->actual_value, "\\092", 4), SUCCESS);
                            break;
                        case '"':
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                            break;
                        case 't':
                            CHECK(str_concat(&token->actual_value, "\\009", 4), SUCCESS);
                            break;
                        case 'n':
                            CHECK(str_concat(&token->actual_value, "\\010", 4), SUCCESS);
                            break;
                        case 'x':
                            CHECK(str_add_char(&token->actual_value, '\\'), SUCCESS);
                            for(int pow = 16; pow >= 1 ; pow = pow/16){
                                c = (char)getc(f);
                                if(isalpha(c)){
                                    if(tolower(c) > 102)
                                        return ERROR_LEX;
                                    decimal += (c - 97 + 10)*pow;
                                }
                                else if(isdigit(c)){
                                    decimal += (c - 48)*pow;
                                }
                                else
                                    return ERROR_LEX;
                            }
                            for(int pow = 100,tmp=decimal; pow >= 1 ; pow = pow/10){
                                tmp = tmp/pow;
                                CHECK(str_add_char(&token->actual_value, tmp + 48), SUCCESS);
                                decimal = decimal - tmp*pow;
                                tmp = decimal; //tmp set remainder
                            }
                            break;
                        default:
                            return ERROR_LEX;
                    }
                }
                else if(c == '#'){
                    CHECK(str_concat(&token->actual_value, "\\035", 4), SUCCESS);
                }
                else if(c == '"'){
                    token->type = TOKEN_STR;
                    break;
                }
                else if(c > 32){
                    CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                }
                else if(c == ' '){
                    CHECK(str_concat(&token->actual_value, "\\032", 4), SUCCESS);
                }
                else
                    return ERROR_LEX;
                c = (char)getc(f);
            }
            break;
        default:
            if(isalpha(c) || c == '_') {
                while (isalnum(c) || c == '_') {
                    CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                    c = (char)getc(f);
                }
                token->type = TOKEN_ID;
                ungetc(c,f);
            }
            else if(isdigit(c)){
                bool state_float = false;
                bool state_exponent = false;
                if(c == '0')
                    return ERROR_LEX;

                CHECK(str_add_char(&token->actual_value, c), SUCCESS);

                while(true){
                    c = (char)getc(f);
                    if(c == '.'){//state_float
                        if(state_float || state_exponent)
                            break;
                        state_float = true;
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        c = (char)getc(f);
                        if(isdigit(c)){
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        }
                        else
                            return ERROR_LEX;
                    }
                    else if(tolower(c) == 'e'){//state_exponent
                        if(state_exponent)
                            return ERROR_LEX;
                        state_exponent = true;
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        c = (char)getc(f);
                        if(c == '+' || c == '-'){
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                            c = (char)getc(f);
                        }
                        if(!isdigit(c))
                            return ERROR_LEX;
                        while(c == '0'){
                            c = (char)getc(f);
                        }
                        ungetc(c,f);
                    }
                    else if(isdigit(c)){
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                    }
                    else
                        break;
                }
                if(state_float || state_exponent)
                    token->type = TOKEN_FLOAT;
                else
                    token->type = TOKEN_INTEGER;
                ungetc(c,f);
            }
            else
                return ERROR_LEX;
            break;
    }
    isKeyword(&token->type, token->actual_value.str);
    return SUCCESS;
}


int copy_token(token_t *t1, token_t *t2){
    int error_code;
    t1->type = t2->type;
    t1->lineno = t2->lineno;
    t1->pos = t2->pos;
    CHECK(str_copy(&t1->actual_value, &t2->actual_value), SUCCESS);
    return SUCCESS;
}

int token_init(token_t* token){
    int ret;
    token->type = TOKEN_ERROR;
    token->lineno = -1;
    token->pos = -1;
    if((ret = str_init(&token->actual_value)) == SUCCESS)
        return SUCCESS;
    else
        return ret;
}


int main(){ // testing
    token_t token;
    token_init(&token);
    while(token.type != TOKEN_EOF){
        if(get_next_token(&token) == ERROR_LEX)
            printf("Lex error.\n");
        else
            print_token(&token);
    }
    str_free(&token.actual_value);
    return 0;
}
