//Scanner


#include "error.h"
#include "libraries.h"
#include "scanner.h"
#include "parser.h"


#define MATCH_BASE(expr) do{                                               \
    if ((expr)) {                                                           \
        CHECK(str_add_char(&token->actual_value, c), SUCCESS);              \
    } else if (c == '_') {                                                  \
        ungetc(c, f);                                                       \
    } else                                                                  \
        return ERROR_LEX;                                                   \
    while (true) {                                                          \
        c = (char) getc(f);                                                 \
        if ((expr)) {                                                       \
            CHECK(str_add_char(&token->actual_value, c), SUCCESS);          \
            state_underscore = false;                                       \
        } else if (c == '_') {                                              \
            if (state_underscore)                                           \
                 return ERROR_LEX;                                          \
            state_underscore = true;                                        \
            c = (char) getc(f);                                             \
            if ((expr))                                                     \
                CHECK(str_add_char(&token->actual_value, c), SUCCESS);      \
            else                                                            \
                return ERROR_LEX;                                           \
        } else {                                                            \
            ungetc(c, f);                                                   \
            token->type = TOKEN_INTEGER;                                    \
            return SUCCESS;                                                 \
        }                                                                   \
    }                                                                       \
}while(0)








int getToken(token_t *token) {
    int error_code;
    char c;
    static FILE *f;
    f = stdin; //set stream to stdin
    str_reinit(&token->actual_value); //clear token string

    state_start:
    c = (char) fgetc(f);

    if (c == EOF) {
        token->type = TOKEN_EOF;
        return SUCCESS;
    }

    while (isspace(c)) {
        if (c == EOL) {
            token->type = TOKEN_EOL;
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

            if (isalpha(c) || c == '_') {//state id
                while (isalnum(c) || c == '_') {
                    CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                    c = (char) getc(f);
                }
                token->type = TOKEN_ID;
                ungetc(c, f);
            } else if (isdigit(c)) {//state number literal
                bool state_underscore = false;
                bool state_float = false;
                bool state_exponent = false;
                CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                if (c == '0') {
                    c = (char) getc(f);
                    if (c == '0')
                        return ERROR_LEX;
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
                    } else
                        ungetc(c, f);
                    //return SUCCESS;
                }

                while (true) {
                    c = (char) getc(f);
                    if (c == '.') {//state_float
                        if (state_float || state_exponent)
                            break;
                        state_float = true;
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        c = (char) getc(f);
                        if (isdigit(c)) {
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        } else
                            return ERROR_LEX;
                    } else if (tolower(c) == 'e') {//state_exponent
                        if (state_exponent)
                            return ERROR_LEX;
                        state_exponent = true;
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                        c = (char) getc(f);
                        if (c == '+' || c == '-') {
                            CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                            c = (char) getc(f);
                        }
                        if (!isdigit(c))
                            return ERROR_LEX;
                        while (c == '0') {
                            c = (char) getc(f);
                        }
                        ungetc(c, f);
                    } else if (isdigit(c)) {
                        CHECK(str_add_char(&token->actual_value, c), SUCCESS);
                    } else
                        break;
                }
                if (state_float || state_exponent)
                    token->type = TOKEN_FLOAT;
                else
                    token->type = TOKEN_INTEGER;
                ungetc(c, f);
            } else
                return ERROR_LEX;
            break;
    }
    isKeyword(&token->type, token->actual_value.str);
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
            printf("[EOF] ");
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


int token_init(token_t *token) {
    int ret;
    token->type = TOKEN_ERROR;
    token->lineno = 0;
    token->pos = 0;
    token->next = NULL;
    if ((ret = str_init(&token->actual_value)) == SUCCESS)
        return SUCCESS;
    else
        return ret;
}

int copy_token(token_t *t1, token_t *t2) {
    int error_code;
    t1->type = t2->type;
    t1->lineno = t2->lineno;
    t1->pos = t2->pos;
    CHECK(str_copy(&t1->actual_value, &t2->actual_value), SUCCESS);
    return SUCCESS;
}

void token_list_init(token_list_t *l) {
    l->act = NULL;
    l->head = NULL;
}

void token_list_first(token_list_t *l) {
    l->act = l->head;
}

int token_list_insert(token_list_t *l, token_t *token) {
    token_t *next_token;
    if (l == NULL)
        return ERROR_TRANS;
    else if (l->head == NULL)
        l->head = token;
    else {
        next_token = l->head;
        while (next_token->next != NULL) {
            next_token = next_token->next;
        }
        next_token->next = token;
    }
    return SUCCESS;
}

int token_list_next(token_list_t *l) {
    if (l->head == NULL)
        return ERROR_TRANS;
    else if (l->act == NULL)
        l->act = l->head;
    else
        l->act = l->act->next;
    return SUCCESS;
}


void token_list_dispose(token_list_t *l) {
    token_t *next_token;
    token_t *del_token;
    if (l != NULL && l->head != NULL) {
        next_token = l->head->next;
        str_free(&l->head->actual_value);
        free(l->head);
        while (next_token != NULL) {
            del_token = next_token;
            next_token = del_token->next;
            str_free(&del_token->actual_value);
            free(del_token);
        }
    }
}

//int main(){ // testing
//    token_t token;
//    token_init(&token);
//    while(token.type != TOKEN_EOF){
//        if(get_next_token(&token) == ERROR_LEX)
//            printf("Lex error.\n");
//        else
//            print_token(&token);
//    }
//    str_free(&token.actual_value);
//    return 0;
//}
