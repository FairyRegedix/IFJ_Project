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


case STATE_START:
if (c = EOL){
  *type = TOKEN_EOL;
}
if (c = '+'){
  *type = TOKEN_ADD;
}
if (c = '-'){
  *type = TOKEN_SUB;
}
if (c = '*'){
  *type = TOKEN_MUL;
}
if (c = '/'){
  *type = TOKEN_DIV;
}
if (c = '='){
  *type = TOKEN_EQL;
}
if (c = '!''='){
  *type = TOKEN_NEQ;
}


switch(c){
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
