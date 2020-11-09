#include "str.h"

typedef enum{
    TOKEN_ERROR = -1,
    //Keywords
    TOKEN_BOOL = 1,
    TOKEN_TRUE = 2,
    TOKEN_FALSE = 3,
    TOKEN_ELSE = 4,
    TOKEN_FLOAT64 = 5,
    TOKEN_FOR = 6,
    TOKEN_FUNC = 7,
    TOKEN_IF = 8,
    TOKEN_INT = 9,
    TOKEN_PACKAGE = 10,
    TOKEN_RETURN = 11,
    TOKEN_STRING = 12,
    //-----------

    TOKEN_UNDERSCORE = 13,
    TOKEN_ID = 14,
    TOKEN_RBRACKET = 15, // (
    TOKEN_LBRACKET = 16, // )
    TOKEN_RCURLY = 17, // }
    TOKEN_LCURLY = 18, // {
    TOKEN_COMMA = 19,
    TOKEN_SEMICOLON = 20,
    TOKEN_EOL = 21, // eol
    TOKEN_EOF = 22, // eof
    TOKEN_DEFINITION = 23,
    TOKEN_ASSIGN = 24,
    TOKEN_INTEGER = 25, // int
    TOKEN_FLOAT = 26, // float
    TOKEN_STR = 27, // str

    TOKEN_NOT = 28, // !
    TOKEN_AND = 29, // &
    TOKEN_OR = 30, // ||
    TOKEN_ADD = 31, // +
    TOKEN_SUB = 32, // -
    TOKEN_MUL = 33, // *
    TOKEN_DIV = 34, // /
    TOKEN_EQL = 35, // =
    TOKEN_NEQ = 36, // !=
    TOKEN_LT = 37, // <
    TOKEN_GT = 38, // >
    TOKEN_LTE = 39, // <=
    TOKEN_GTE = 40, // >=
} token_type;

void getToken(FILE *f, int *type, string *actual_value);
void getNextToken();

typedef struct{
    token_type type;
    string actual_value; //storing int,float,string value
    int lineno; //line number
    int pos; //position of the first char of the token
}token;


#define IS_KEYWORD(token)  ((token).type == TOKEN_BOOL || \
                            (token).type == TOKEN_TRUE || \
                            (token).type == TOKEN_FALSE || \
                            (token).type == TOKEN_ELSE || \
                            (token).type == TOKEN_FLOAT64 || \
                            (token).type == TOKEN_FOR || \
                            (token).type == TOKEN_FUNC || \
                            (token).type == TOKEN_IF || \
                            (token).type == TOKEN_INT || \
                            (token).type == TOKEN_PACKAGE || \
                            (token).type == TOKEN_RETURN || \
                            (token).type == TOKEN_STRING  || )
