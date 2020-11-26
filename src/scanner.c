//Scanner


#include "error.h"
#include "libraries.h"
#include "scanner.h"
#include "parser.h"


#define MATCH_BASE(expr) do{                                                \
    while (true) {                                                          \
        if ((expr)) {                                                       \
            CHECK(str_add_char(&token->actual_value, c), SUCCESS);          \
            state_underscore = false;                                       \
        } else if (c == '_') {                                              \
            if (state_underscore)                                           \
                 return ERROR_LEX;                                          \
            state_underscore = true;                                        \
            c = (char) getc(f);                                             \
            CHECK(str_add_char(&token->actual_value, c), SUCCESS); \
            if ((expr))                                                    \
                state_underscore = false;                                   \
            else                                                            \
                return ERROR_LEX;                                           \
                \
        } else if(isalnum(c)) {                                             \
            CHECK(str_add_char(&token->actual_value, c), SUCCESS);   \
            return ERROR_LEX;\
        } else {                                                            \
            ungetc(c, f);                                                   \
            token->type = TOKEN_INTEGER;                                    \
            return SUCCESS;                                                 \
        }                                                                   \
        c = (char) getc(f);\
    }                                                                       \
}while(0)


int getToken(token_t *token) {
    int error_code;
    char c;
    static FILE *f;
    static int lineno = 1;
    f = stdin; //set stream to stdin
    //str_reinit(&token->actual_value); //clear token string

    state_start:
    token->lineno = lineno;
    c = (char) fgetc(f);

    if (c == EOF) {
        token->type = TOKEN_EOF;
        token->lineno = lineno;
        return SUCCESS;
    }

    while (isspace(c)) {
        if (c == EOL) {
            token->type = TOKEN_EOL;
            token->lineno = ++lineno;
            return SUCCESS;
        }
        c = (char) fgetc(f);
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
            } else
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

        case '&':
            c = (char) getc(f);
            if (c == '&') {
                token->type = TOKEN_AND;
            } else {
                return ERROR_LEX;
            }
            break;

        case '|':
            c = (char) getc(f);
            if (c == '|') {
                token->type = TOKEN_OR;
            } else {
                return ERROR_LEX;
            }
            break;

        case '/':
            c = (char) fgetc(f);
            if (c == '/') {
                for (; (c != EOL && c != EOF); c = (char) fgetc(f)) { ; }
            } else if (c == '*') {
                bool eol_in_comment = false;
                while (true) {
                    c = (char) fgetc(f);
                    if (c == '*') {
                        c = (char) fgetc(f);
                        if (c == '/')
                            break;
                        else if (c == EOF)
                            return ERROR_LEX;
                        else if (c == EOL)
                            eol_in_comment = true;
                        else { ; }
                    } else if (c == EOL)
                        eol_in_comment = true;
                    else if (c == EOF)
                        return ERROR_LEX;
                    else { ; }
                }
                if (eol_in_comment) {
                    token->type = TOKEN_EOL;
                    token->lineno = ++lineno;
                    return SUCCESS;
                }
            } else {
                ungetc(c, f);
                token->type = TOKEN_DIV;
                break;
            }
            goto state_start; //no lex errors go to start

        case '"': //state string literal
            c = (char) getc(f);
            while (true) {
                if (c == 92) {// backslash "\"
                    int decimal = 0; //stores decimal value of hex escape sequence found in string
                    c = (char) getc(f);
                    switch (c) {
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
                            for (int pow = 16; pow >= 1; pow = pow / 16) {
                                c = (char) getc(f);
                                if (isalpha(c)) {
                                    if (tolower(c) > 'f')
                                        return ERROR_LEX;
                                    decimal += (c - 97 + 10) * pow;
                                } else if (isdigit(c)) {
                                    decimal += (c - 48) * pow;
                                } else
                                    return ERROR_LEX;
                            }
                            //2 iterations as 2hex digits are expected after \x
                            for (int pow = 100, tmp = decimal; pow >= 1; pow = pow / 10) {
                                tmp = tmp / pow;
                                CHECK(str_add_char(&token->actual_value, tmp + 48), SUCCESS);
                                decimal = decimal % pow; //set remainder
                                tmp = decimal;
                            }
                            break;
                        default:
                            return ERROR_LEX;
                    }
                } else if (c == '#') {
                    CHECK(str_concat(&token->actual_value, "\\035", 4), SUCCESS);
                } else if (c == '"') {
                    token->type = TOKEN_STR;
                    break;
                } else if (c > 32) {
                    CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                } else if (c == ' ') {
                    CHECK(str_concat(&token->actual_value, "\\032", 4), SUCCESS);
                } else
                    return ERROR_LEX;
                c = (char) getc(f);
            }
            break;
        default:
            CHECK(str_add_char(&token->actual_value, c), SUCCESS);

            if (isalpha(c) || c == '_') {//state id
                c = (char) getc(f);

                while (isalnum(c) || c == '_') {
                    CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                    c = (char) getc(f);
                }
                token->type = TOKEN_ID;
                isKeyword(&token->type, token->actual_value.str);
                ungetc(c, f);

            } else if (isdigit(c)) {//state number literal
                bool state_underscore = false;
                bool state_float = false;
                bool state_exponent = false;

                if (c == '0') {
                    c = (char) getc(f);
                    if (isdigit(c)){
                        CHECK(str_add_char(&token->actual_value, tolower(c)), SUCCESS);
                        return ERROR_LEX;
                    }
                    else if (tolower(c) == 'b') {//binary state
                        CHECK(str_add_char(&token->actual_value, tolower(c)), SUCCESS);
                        c = (char) getc(f);
                        MATCH_BASE((c == '0' || c == '1'));

                    } else if (tolower(c) == 'o') {//octal state
                        CHECK(str_add_char(&token->actual_value, tolower(c)), SUCCESS);
                        c = (char) getc(f);
                        MATCH_BASE((c >= '0' && c < '8'));

                    } else if (tolower(c) == 'x') {//hexadecimal state
                        CHECK(str_add_char(&token->actual_value, tolower(c)), SUCCESS);
                        c = (char) getc(f);
                        MATCH_BASE(isxdigit(c));
                    } else if (c == '_'){
                        CHECK(str_add_char(&token->actual_value, tolower(c)), SUCCESS);
                        return ERROR_LEX;
                    } else
                        ungetc(c, f);
                }

                while (true) {
                    c = (char) getc(f);
                    if (c == '.') {//state_float
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        if (state_float || state_exponent)
                            return ERROR_LEX;
                        state_float = true;
                        c = (char) getc(f);
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        if (!isdigit(c))
                            return ERROR_LEX;
                    }
                    else if(c == '_') {
                        if (state_underscore)
                             return ERROR_LEX;
                        state_underscore = true;
                        c = (char) getc(f);
                        if (isdigit(c)){
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                            state_underscore = false;
                        }
                        else{
                            CHECK(str_add_char(&token->actual_value, '_'), SUCCESS);
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                            return ERROR_LEX;
                        }
                    }
                    else if (tolower(c) == 'e') {//state_exponent
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        if (state_exponent)
                            return ERROR_LEX;
                        state_exponent = true;
                        c = (char) getc(f);
                        if (c == '+' || c == '-') {
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                            c = (char) getc(f);
                        }
                        if (!isdigit(c)){
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                            return ERROR_LEX;
                        }
                        while (c == '0') {
                            c = (char) getc(f);
                        }
                        ungetc(c, f);
                    }
                    else if (isdigit(c)) {
                        state_underscore = false;
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                    }
                    else
                        break;
                }
                if (state_float || state_exponent)
                    token->type = TOKEN_FLOAT;
                else
                    token->type = TOKEN_INTEGER;
                ungetc(c, f);
            } else{
                return ERROR_LEX;
            }
            break;
    }
    return SUCCESS;
}

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
}// end of isKeyword

void print_token(token_t *token) {
    switch (token->type) {
        case TOKEN_ERROR:
            printf("[Error: %s] ", token->actual_value.str);
            break;
        case TOKEN_BOOL:
        case TOKEN_TRUE:
        case TOKEN_FALSE:
        case TOKEN_ELSE:
        case TOKEN_FLOAT64:
        case TOKEN_FOR:
        case TOKEN_FUNC:
        case TOKEN_IF:
        case TOKEN_INT:
        case TOKEN_PACKAGE:
        case TOKEN_RETURN:
        case TOKEN_STRING:
        case TOKEN_UNDERSCORE:
        case TOKEN_PRINT:
            printf("[%s] ", token->actual_value.str);
            break;
        case TOKEN_ID:
            printf("[ID: \"%s\"] ", token->actual_value.str);
            break;
        case TOKEN_RBRACKET:
            printf("[)] ");
            break;
        case TOKEN_LBRACKET:
            printf("[(] ");
            break;
        case TOKEN_RCURLY:
            printf("[}] ");
            break;
        case TOKEN_LCURLY:
            printf("[{] ");
            break;
        case TOKEN_COMMA:
            printf("[,] ");
            break;
        case TOKEN_SEMICOLON:
            printf("[;] ");
            break;
        case TOKEN_EOL:
            printf("[EOL]\n");
            break;
        case TOKEN_EOF:
            printf("[EOF]\n");
            break;
        case TOKEN_DEFINITION:
            printf("[:=] ");
            break;
        case TOKEN_ASSIGN:
            printf("[=] ");
            break;
        case TOKEN_INTEGER:
            printf("[INT: %s] ", token->actual_value.str);
            break;
        case TOKEN_FLOAT:
            printf("[FLOAT: %s] ", token->actual_value.str);
            break;
        case TOKEN_STR:
            printf("[STR:\"%s\"] ", token->actual_value.str);
            break;
        case TOKEN_NOT:
            printf("[!] ");
            break;
        case TOKEN_AND:
            printf("[&&] ");
            break;
        case TOKEN_OR:
            printf("[||] ");
            break;
        case TOKEN_ADD:
            printf("[+] ");
            break;
        case TOKEN_SUB:
            printf("[-] ");
            break;
        case TOKEN_MUL:
            printf("[*] ");
            break;
        case TOKEN_DIV:
            printf("[/] ");
            break;
        case TOKEN_EQL:
            printf("[==] ");
            break;
        case TOKEN_NEQ:
            printf("[!=] ");
            break;
        case TOKEN_LT:
            printf("[<] ");
            break;
        case TOKEN_GT:
            printf("[>] ");
            break;
        case TOKEN_LTE:
            printf("[<=] ");
            break;
        case TOKEN_GTE:
            printf("[>=] ");
            break;
        default:
            break;
    }
}

char* token_enum_to_str(token_type type){
    char* msg = "";
    switch(type){
        case TOKEN_ERROR:
            msg = "TOKEN_ERROR";
            break;
        case TOKEN_BOOL:
            msg = "TOKEN_BOOL";
            break;
        case TOKEN_TRUE:
            msg = "TOKEN_TRUE";
            break;
        case TOKEN_FALSE:
            msg = "TOKEN_FALSE";
            break;
        case TOKEN_ELSE:
            msg = "TOKEN_ELSE";
            break;
        case TOKEN_FLOAT64:
            msg = "TOKEN_FLOAT64";
            break;
        case TOKEN_FOR:
            msg = "TOKEN_FOR";
            break;
        case TOKEN_FUNC:
            msg = "TOKEN_FUNC";
            break;
        case TOKEN_IF:
            msg = "TOKEN_IF";
            break;
        case TOKEN_INT:
            msg = "TOKEN_INT";
            break;
        case TOKEN_PACKAGE:
            msg = "TOKEN_PACKAGE";
            break;
        case TOKEN_RETURN:
            msg = "TOKEN_RETURN";
            break;
        case TOKEN_STRING:
            msg = "TOKEN_STRING";
            break;
        case TOKEN_UNDERSCORE:
            msg = "TOKEN_UNDERSCORE";
            break;
        case TOKEN_PRINT:
            msg = "TOKEN_PRINT";
            break;
        case TOKEN_INPUTI:
            msg = "TOKEN_INPUTI";
            break;
        case TOKEN_INPUTS:
            msg = "TOKEN_INPUTS";
            break;
        case TOKEN_LEN:
            msg = "TOKEN_LEN";
            break;
        case TOKEN_SUBSTR:
            msg = "TOKEN_SUBSTR";
            break;
        case TOKEN_ORD:
            msg = "TOKEN_ORD";
            break;
        case TOKEN_CHR:
            msg = "TOKEN_CHR";
            break;
        case TOKEN_INPUTB:
            msg = "TOKEN_INPUTB";
            break;
        case TOKEN_INPUTF:
            msg = "TOKEN_INPUTF";
            break;
        case TOKEN_ID:
            msg = "TOKEN_ID";
            break;
        case TOKEN_RBRACKET:
            msg = "TOKEN_RBRACKET";
            break;
        case TOKEN_LBRACKET:
            msg = "TOKEN_LBRACKET";
            break;
        case TOKEN_RCURLY:
            msg = "TOKEN_RCURLY";
            break;
        case TOKEN_LCURLY:
            msg = "TOKEN_LCURLY";
            break;
        case TOKEN_COMMA:
            msg = "TOKEN_COMMA";
            break;
        case TOKEN_SEMICOLON:
            msg = "TOKEN_SEMICOLON";
            break;
        case TOKEN_EOL:
            msg = "TOKEN_EOL";
            break;
        case TOKEN_EOF:
            msg = "TOKEN_EOF";
            break;
        case TOKEN_DEFINITION:
            msg = "TOKEN_DEFINITION";
            break;
        case TOKEN_ASSIGN:
            msg = "TOKEN_ASSIGN";
            break;
        case TOKEN_INTEGER:
            msg = "TOKEN_INTEGER";
            break;
        case TOKEN_FLOAT:
            msg = "TOKEN_FLOAT";
            break;
        case TOKEN_STR:
            msg = "TOKEN_STR";
            break;
        case TOKEN_BOOLEAN:
            msg = "TOKEN_BOOLEAN";
            break;
        case TOKEN_NOT:
            msg = "TOKEN_NOT";
            break;
        case TOKEN_AND:
            msg = "TOKEN_AND";
            break;
        case TOKEN_OR:
            msg = "TOKEN_OR";
            break;
        case TOKEN_ADD:
            msg = "TOKEN_ADD";
            break;
        case TOKEN_SUB:
            msg = "TOKEN_SUB";
            break;
        case TOKEN_MUL:
            msg = "TOKEN_MUL";
            break;
        case TOKEN_DIV:
            msg = "TOKEN_DIV";
            break;
        case TOKEN_EQL:
            msg = "TOKEN_EQL";
            break;
        case TOKEN_NEQ:
            msg = "TOKEN_NEQ";
            break;
        case TOKEN_LT:
            msg = "TOKEN_LT";
            break;
        case TOKEN_GT:
            msg = "TOKEN_GT";
            break;
        case TOKEN_LTE:
            msg = "TOKEN_LTE";
            break;
        case TOKEN_GTE:
            msg = "TOKEN_GTE";
            break;
    }
    return msg;
};


int token_init(token_t *token) {
    int ret;
    token->type = TOKEN_ERROR;
    token->lineno = 0;
    token->next = NULL;
    token->prev = NULL;
    if ((ret = str_init(&token->actual_value)) == SUCCESS)
        return SUCCESS;
    else
        return ret;
}


void token_list_init(token_list_t *l) {
    l->act = NULL;
    l->first = NULL;
    l->last = NULL;
}

int token_list_insert(token_list_t *l, token_t *token) {
    token_t *next_token;
    if (l == NULL)
        return ERROR_TRANS;
    else if (l->first == NULL){
        l->first = token;
        l->last = token;
    }
    else {
        next_token = l->first;
        while (next_token->next != NULL) {
            next_token = next_token->next;
        }
        next_token->next = token;
        token->prev = next_token;
        l->last = token;
    }
    return SUCCESS;
}

int token_list_next(token_list_t *l) {
    if (l->first == NULL)
        return ERROR_TRANS;
    else if (l->act == NULL)//list is always active
        //TOKEN_EOF always at the end of the list as a guard
        l->act = l->first;
    else
        l->act = l->act->next;
    return SUCCESS;
}


void token_list_dispose(token_list_t *l) {
    token_t *next_token;
    token_t *del_token;
    if (l != NULL && l->first != NULL) {
        next_token = l->first->next;
        str_free(&l->first->actual_value);
        free(l->first);
        while (next_token != NULL) {
            del_token = next_token;
            next_token = del_token->next;
            str_free(&del_token->actual_value);
            free(del_token);
        }
    }
}

int scanner_fill_token_list(token_list_t* l){
    int error_code;
    token_t* token;
    do{
        if((token = malloc(sizeof(token_t))) == NULL)
            return handle_error(ERROR_TRANS, "%s\n", "memory allocation failed");
        token_init(token);
        token_list_insert(l, token);
        error_code = getToken(token);
        if(error_code == ERROR_LEX)
            return handle_error(error_code, "at line %i: incorrect lexeme \"%s\"\n",
                                token->lineno, token->actual_value.str);
        else if(error_code == SUCCESS)
            continue;
        else
            return handle_error(error_code, "error encountered in scanner\n");

    }while(token->type != TOKEN_EOF);

    token = l->first;
    while(token != NULL){//testing
        print_token(token);
        token=token->next;
    }
    return SUCCESS;
}
