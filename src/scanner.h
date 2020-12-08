/**
 * IFJ Projekt 2020
 *
 * Implementation of scanner
 *
 * @author <xdemsk00> Patrik Demský
 * @author <xsabol03> František Sabol
 */


#ifndef IFJ_SCANNER_H
#define IFJ_SCANNER_H


#include "str.h"


#define CHECK(x, y) do{\
  error_code = (x);     \
  if (error_code != (y)) \
    return error_code;    \
}while (0)

#define EOL '\n'

//types of tokens
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
    TOKEN_UNDERSCORE = 13,
    TOKEN_PRINT = 44,
    TOKEN_INPUTI = 46,
    TOKEN_INPUTS = 47,
    TOKEN_LEN = 48,
    TOKEN_SUBSTR = 49,
    TOKEN_ORD = 50,
    TOKEN_CHR = 51,
    TOKEN_INPUTB = 52,
    TOKEN_INPUTF = 53,
    //-----------

    TOKEN_ID = 14, // main
    TOKEN_RBRACKET = 15, // )
    TOKEN_LBRACKET = 16, // (
    TOKEN_RCURLY = 17, // }
    TOKEN_LCURLY = 18, // {
    TOKEN_COMMA = 19, // ,
    TOKEN_SEMICOLON = 20, // ;
    TOKEN_EOL = 21, // eol
    TOKEN_EOF = 22, // eof
    TOKEN_DEFINITION = 23, // :=
    TOKEN_ASSIGN = 24, // =
    TOKEN_INTEGER = 25, // int literal
    TOKEN_FLOAT = 26, // float literal
    TOKEN_STR = 27, // str literal
    TOKEN_BOOLEAN = 28, //bool literal

    TOKEN_NOT = 29, // !
    TOKEN_AND = 30, // &&
    TOKEN_OR = 31, // ||
    TOKEN_ADD = 32, // +
    TOKEN_SUB = 33, // -
    TOKEN_MUL = 34, // *
    TOKEN_DIV = 35, // /
    TOKEN_EQL = 36, // ==
    TOKEN_NEQ = 37, // !=
    TOKEN_LT = 38, // <
    TOKEN_GT = 39, // >
    TOKEN_LTE = 40, // <=
    TOKEN_GTE = 41, // >=

} token_type;


//token
typedef struct token{
    token_type type; //type
    string actual_value; //storing int,float,string value
    int lineno; //line number
    struct token* next; //pointer to the next token in the list
    struct token* prev; //pointer to the previous token in the list
}token_t;

typedef struct token_list{
    token_t* act;
    token_t* last;
    token_t* first;
}token_list_t;


/*
 * Initializes a token.
 * @param  token  pointer to a token to be initialized.
 * */
int token_init(token_t *token);

/*
 * Initializes a token list.
 * @param  l  pointer to a list to be initialized.
 * */
void token_list_init(token_list_t* l);

/*
 * Inserts tokens to the list.
 * @param  l  pointer to a list.
 * @param  token  pointer to a token.
 * @return
 * */
int token_list_insert(token_list_t* l, token_t* token);

/*
 *
 * @param  l  pointer to a list.
 * @return
 * */
int token_list_next(token_list_t* l);

/* Frees all dynamic memory used by a token list.
 * @param  l  pointer to a token list.
 * */
void token_list_dispose(token_list_t* l);

/*
 * The main function of scanner, scans tokens.
 * Fuction is based on the graf in the documentation.
 * @param   token  pointer to a token.
 * @return  SUCESS if everything runs without problems,
 * otherwise return ERROR_LEX.
 * */
int getToken(token_t* token);


/* Fills token list with tokens recognized by scanner.
 * @param l : A pointer to a token list to fill with tokens*/
int scanner_fill_token_list(token_list_t* l);

/*
 * Auxiliary debugging function that prints the tokens.
 * @param  token  pointer to a token.
 * */
void print_token(token_t* token);

/* Converts token_type enum to string represention.
 * @param type : A token_type to convert to a string.*/
char* token_enum_to_str(token_type type);

/*
 * Check if the token type ID is the Keyword. If it is the Keyword
 * change the type.
 * @param  type  pointer to a type of the token.
 * @param  tmp  pointer to a temporary variable that is about to be checked.
 * */
void isKeyword(int *type, char *tmp);

#endif //IFJ_SCANNER_H
