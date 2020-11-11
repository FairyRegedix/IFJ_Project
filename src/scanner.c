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

switch(c){
  case EOF:
  *type = TOKEN_EOL;
  return;

  case '+' :
  *type = TOKEN_ADD;
  return;

  case '-' :
  *type = TOKEN_SUB;
  return;

  case '*' :
  *type = TOKEN_MUL;
  return;

  case '(' :
  *type = TOKEN_LBRACKET;
  return;

  case ')' :
  *type = TOKEN_RBRACKET;
  return;

  case '{' :
  *type = TOKEN_LCURLY;
  return;

  case '}' :
  *type = TOKEN_RCURLY;
  return;

  case ',' :
  *type = TOKEN_COMMA;
  return;

  case '.' :
  *type = TOKEN_DOT;
  return;

 case ';' :
  *type = TOKEN_SEMICOLON;
  return;

  //komentare
  case '/':
  c = fgetc(f);
  //riadkovy Komentar
  if(c == '/'){
    while(c != EOL){
      c = fgetc(f);
    }
    return token;
  }
  //blokovy komentar
  else if (c == '*'){
    c = fgetc(f);

  }
}
