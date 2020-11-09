typedef enum{
    TOKEN_ERROR=-1,
    //Keywords
    TOKEN_BOOL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_ELSE,
    TOKEN_FLOAT64,
    TOKEN_FOR,
    TOKEN_FUNC,
    TOKEN_IF,
    TOKEN_INT,
    TOKEN_PACKAGE,
    TOKEN_RETURN,
    TOKEN_STRING,
    //-----------

    TOKEN_UNDERSCORE,
    TOKEN_ID,
    TOKEN_RBRACKET,
    TOKEN_LBRACKET,
    TOKEN_RCURLY,
    TOKEN_LCURLY,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_EOL,
    TOKEN_EOF,
    TOKEN_DEFINITION,
    TOKEN_ASSIGN,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STR,

    TOKEN_NOT,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_EQL,
    TOKEN_NEQ,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LTE,
    TOKEN_GTE
} token_type;

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
