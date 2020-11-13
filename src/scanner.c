//Scanner



#include "error.h"
#include "libraries.h"

// pomocna
bool end = false;
int symbol;

int getNextToken(token_t *token){

}

void getToken(int *type, string *actual_value){
  while(true){
    c = fgetc(stdin);

    //vynecha biele znaky
    symbol = isspace(c);
    if (symbol != 0){
      if(c == EOF){
        *type = TOKEN_EOF;
        return;
      }
    }

    switch(c){
      // EOF
      case EOF:
      *type = TOKEN_EOF;
      break;
      // EOL
      case EOL:
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
      c = fgetc(stdin);
      //riadkovy Komentar
      if(c == '/'){
        while(c != EOL || c != EOF){
          c = fgetc(stdin);
        }
        break;
      }
      //blokovy komentar
      else if (c == '*'){
        while(c != EOF){
          c = fgetc(stdin);
          if (c == '*'){
            c = fgetc(stdin);
            if (c == '/'){
              break;
            }
          }
        }
      }
      // /
      else{
        ungetc(c,stdin);
        *type = TOKEN_DIV;
        break;
      }

      case '>' :
      c = fgetc(stdin);
      // >=
      if (c == '='){
        *type = TOKEN_GTE;
        break;
      }
      // >
      else{
        ungetc(c,stdin);
        *type = TOKEN_GT;
        break;
      }

      case '<' :
      c = fgetc(stdin);
      // <=
      if (c == '='){
        *type = TOKEN_LTE;
        break;
      }
      // <
      else{
        ungetc(c,stdin);
        *type = TOKEN_LT;
        break;
      }


      case '=' :
      c = fgetc(stdin);
      // ==
      if (c == '='){
        *type = TOKEN_EQL;
        break;
      }
      // =
      else {
        ungetc(c,stdin);
        *type = TOKEN_ASSIGN;
        break;

      }


      case '!' :
      c = fgetc(stdin);
      // !=
      if (c == '='){
        *type = TOKEN_NEQ;
        break;
      }
      else{
        return ERROR_LEX;
      }

      case '"' :
      //string
      while(c != EOF || c != EOL){
            c = fgetc(stdin);
            if (c == '"'){
              *type = TOKEN_STRING;
              break;
            }
            return ERROR_LEX;
      }

      case ':' :
      while (c != EOF || c != EOL || c != '='){
        c = fgetc(stdin);
        // :=
        if (c == '='){
          *type = TOKEN_DEFINITION;
          break;
        }
        else{
          return ERROR_LEX;
        }
      }



      //check the keywords
      int isKeyword(char *tmp){
        if (strcmp(tmp, "if") == 0){
          *type = TOKEN_IF;
          break;
        }
        else if (strcmp(tmp, "else") == 0){
          *type = TOKEN_ELSE;
          break;
        }
        else if (strcmp(tmp, "package") == 0){
          *type = TOKEN_PACKAGE;
          break;
        }
        else if (strcmp(tmp, "return") == 0){
          *type = TOKEN_RETURN;
          break;
        }
        else if (strcmp(tmp, "for") == 0){
          *type = TOKEN_FOR;
          break;
        }
        else if (strcmp(tmp, "func") == 0){
          *type = TOKEN_FUNC;
          break;
        }
        else if (strcmp(tmp, "print") == 0){
          *type = TOKEN_PRINT;
          break;
        }
        else if (strcmp(tmp, "while") == 0){
          *type = TOKEN_WHILE;
          break;
        }
        else if (strcmp(tmp, "inputi") == 0){
          *type = TOKEN_INPUTI;
          break;
        }
        else if (strcmp(tmp, "inputs") == 0){
          *type = TOKEN_INPUTS;
          break;
        }
        else if (strcmp(tmp, "len") == 0){
          *type = TOKEN_LEN;
          break;
        }
        else if (strcmp(tmp, "substr") == 0){
          *type = TOKEN_SUBSTR;
          break;
        }
        else if (strcmp(tmp, "ord") == 0){
          *type = TOKEN_ORD;
          break;
        }
        else if (strcmp(tmp, "chr") == 0){
          *type = TOKEN_CHR;
          break;
        }
        else if (strcmp(tmp, "inputb") == 0){
          *type = TOKEN_INPUTB;
          break;
        }
        else if (strcmp(tmp, "inputf") == 0){
          *type = TOKEN_INPUTF;
          break;
        }
        else if (strcmp(tmp, "bool") == 0){
          *type = TOKEN_BOOL;
          break;
        }
        else if (strcmp(tmp, "true") == 0){
          *type = TOKEN_TRUE;
          break;
        }
        else if (strcmp(tmp, "false") == 0){
          *type = TOKEN_FALSE;
          break;
        }
        else if (strcmp(tmp, "int") == 0){
          *type = TOKEN_INT;
          break;
        }
        else if (strcmp(tmp, "float64") == 0){
          *type = TOKEN_FLOAT64;
          break;
        }
        else if (strcmp(tmp, "string") == 0){
          *type = TOKEN_STRING;
          break;
        }
        else if (strcmp(tmp, "_") == 0){
          *type = TOKEN_UNDERSCORE;
          break;
        }
        //miesto na dalsie mozne keywordy

      }// end of isKeyword

    }// end of switch

  }// end of while

}// end of getToken



main(){
  void getToken(int *type, string *actual_value);
  return;
}
