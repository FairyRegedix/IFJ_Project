//Scanner



#include "error.h"
#include "libraries.h"
#include "scanner.h"

// pomocna
bool end = false;
int symbol;

int getNextToken(token_t *token){

}

void getToken(int *type, string *actual_value){
  while(true){
    char c = fgetc(stdin);

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
      printf("[EOF]");
      break;
      // EOL
      case EOL:
      *type = TOKEN_EOL;
      printf("[EOL]");
      break;
      // +
      case '+' :
      *type = TOKEN_ADD;
      printf("[+]");
      break;
      // -
      case '-' :
      *type = TOKEN_SUB;
      printf("[-]");
      break;
      // *
      case '*' :
      *type = TOKEN_MUL;
      printf("[*]");
      break;
      // (
      case '(' :
      *type = TOKEN_LBRACKET;
      printf("[(]");
      break;
      // )
      case ')' :
      *type = TOKEN_RBRACKET;
      printf("[)]");
      break;
      // {
      case '{' :
      *type = TOKEN_LCURLY;
      printf("[{]");
      break;
      // }
      case '}' :
      *type = TOKEN_RCURLY;
      printf("[}]");
      break;
      // ,
      case ',' :
      *type = TOKEN_COMMA;
      printf("[,]");
      break;
      // .
      case '.' :
      *type = TOKEN_DOT;
      printf("[.]");
      break;
      // ;
     case ';' :
      *type = TOKEN_SEMICOLON;
      printf("[;]");
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
        break;
      }
      // /
      else{
        ungetc(c,stdin);
        *type = TOKEN_DIV;
        printf("[/]");
        break;
      }

      case '>' :
      c = fgetc(stdin);
      // >=
      if (c == '='){
        *type = TOKEN_GTE;
        printf("[>=]");
        break;
      }
      // >
      else{
        ungetc(c,stdin);
        *type = TOKEN_GT;
        printf("[>]");
        break;
      }

      case '<' :
      c = fgetc(stdin);
      // <=
      if (c == '='){
        *type = TOKEN_LTE;
        printf("[<=]");
        break;
      }
      // <
      else{
        ungetc(c,stdin);
        *type = TOKEN_LT;
        printf("[<]");
        break;
      }


      case '=' :
      c = fgetc(stdin);
      // ==
      if (c == '='){
        *type = TOKEN_EQL;
        printf("[==]");
        break;
      }
      // =
      else {
        ungetc(c,stdin);
        *type = TOKEN_ASSIGN;
        printf("[=]");
        break;

      }


      case '!' :
      c = fgetc(stdin);
      // !=
      if (c == '='){
        *type = TOKEN_NEQ;
        printf("[!=]");
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
              printf("[STRING]");
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
          printf("[:=]");
          break;
        }
        else{
          return ERROR_LEX;
        }
      }

    }// end of switch

  }// end of while

}// end of getToken


//check the keywords
int isKeyword(int *type, char *tmp){
  if (strcmp(tmp, "if") == 0){
    *type = TOKEN_IF;   
  }
  else if (strcmp(tmp, "else") == 0){
    *type = TOKEN_ELSE;
  }
  else if (strcmp(tmp, "package") == 0){
    *type = TOKEN_PACKAGE;
    printf("[package]");
  }
  else if (strcmp(tmp, "return") == 0){
    *type = TOKEN_RETURN;
  }
  else if (strcmp(tmp, "for") == 0){
    *type = TOKEN_FOR;
  }
  else if (strcmp(tmp, "func") == 0){
    *type = TOKEN_FUNC;
  }
  else if (strcmp(tmp, "print") == 0){
    *type = TOKEN_PRINT;
  }
  else if (strcmp(tmp, "while") == 0){
    *type = TOKEN_WHILE;
  }
  else if (strcmp(tmp, "inputi") == 0){
    *type = TOKEN_INPUTI;
  }
  else if (strcmp(tmp, "inputs") == 0){
    *type = TOKEN_INPUTS;
  }
  else if (strcmp(tmp, "len") == 0){
    *type = TOKEN_LEN;
  }
  else if (strcmp(tmp, "substr") == 0){
    *type = TOKEN_SUBSTR;
  }
  else if (strcmp(tmp, "ord") == 0){
    *type = TOKEN_ORD;
  }
  else if (strcmp(tmp, "chr") == 0){
    *type = TOKEN_CHR;
  }
  else if (strcmp(tmp, "inputb") == 0){
    *type = TOKEN_INPUTB;
  }
  else if (strcmp(tmp, "inputf") == 0){
    *type = TOKEN_INPUTF;
  }
  else if (strcmp(tmp, "bool") == 0){
    *type = TOKEN_BOOL;
  }
  else if (strcmp(tmp, "true") == 0){
    *type = TOKEN_TRUE;
  }
  else if (strcmp(tmp, "false") == 0){
    *type = TOKEN_FALSE;
  }
  else if (strcmp(tmp, "int") == 0){
    *type = TOKEN_INT;
  }
  else if (strcmp(tmp, "float64") == 0){
    *type = TOKEN_FLOAT64;
  }
  else if (strcmp(tmp, "string") == 0){
    *type = TOKEN_STRING;
  }
  else if (strcmp(tmp, "_") == 0){
    *type = TOKEN_UNDERSCORE;
  }
  //miesto na dalsie mozne keywordy

}// end of isKeyword



main(){
  void getToken(int *type, string *actual_value);
  return;
}
