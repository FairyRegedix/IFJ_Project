//Scanner


#include "error.h"
#include "libraries.h"

// pomocna
bool end = false;
int symbol;

void getToken(FILE *f, int *type, string *actual_value){
  int token;
}

void getNextToken(){

}

//vynecha biele znaky
symbol = isspace(c);
if (symbol != 0){
  if(c == EOF){
    *type = TOKEN_EOF;
    return;
  }
}
while(true){
  c = fgetc(f);

  switch(c){
    // EOF
    case EOF:
    *type = TOKEN_EOL;
    break;
    // +
    case '+' :
    *type = TOKEN_ADD;
    break;
    // -
    case '-' :
    *type = TOKEN_SUB;
    break;
    // *
    case '*' :
    *type = TOKEN_MUL;
    break;
    // (
    case '(' :
    *type = TOKEN_LBRACKET;
    break;
    // )
    case ')' :
    *type = TOKEN_RBRACKET;
    break;
    // {
    case '{' :
    *type = TOKEN_LCURLY;
    break;
    // }
    case '}' :
    *type = TOKEN_RCURLY;
    break;
    // ,
    case ',' :
    *type = TOKEN_COMMA;
    break;
    // .
    case '.' :
    *type = TOKEN_DOT;
    break;
    // ;
   case ';' :
    *type = TOKEN_SEMICOLON;
    break;


    case '/':
    c = fgetc(f);
    //riadkovy Komentar
    if(c == '/'){
      while(c != EOL || c != EOF){
        c = fgetc(f);
      }
      break;
    }
    //blokovy komentar
    else if (c == '*'){
      while(c != EOF){
        c = fgetc(f);
        if (c == '*'){
          c = fgetc(f);
          if (c == '/'){
            break;
          }
        }
      }
    }
    // /
    else{
      ungetc(c,f);
      *type = TOKEN_DIV;
      break;
    }

    case '>' :
    c = fgetc(f);
    // >=
    if (c == '='){
      *type = TOKEN_GTE;
      break;
    }
    // >
    else{
      ungetc(c,f);
      *type = TOKEN_GT;
      break;
    }

    case '<' :
    c = fgetc(f);
    // <=
    if (c == '='){
      *type = TOKEN_LTE;
      break;
    }
    // <
    else{
      ungetc(c,f);
      *type = TOKEN_LT;
      break;
    }


    case '=' :
    c = fgetc(f);
    // ==
    if (c == '='){
      *type = TOKEN_EQL;
      break;
    }
    // =
    else {
      ungetc(c,f);
      *type = TOKEN_ASSIGN;
      break;

    }




}
