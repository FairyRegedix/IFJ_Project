//Scanner


#include "error.h"
#include "libraries.h"
#include "scanner.h"
#include "str.c"
#include "str.h"

// pomocna
bool end = false;
int symbol;
char znak;

/*int getNextToken(token_t *token){

}*/

void getToken(int *type, string *actual_value){
  while(true){
    int c = fgetc(stdin);

    //vynecha biele znaky
    symbol = isspace(c);
    if (symbol != 0){
      if(c == EOF){
        *type = TOKEN_EOF;
        printf("[EOF]");
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
      break;

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
      break;

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
      break;


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
      break;


      case '!' :
      c = fgetc(stdin);
      // !=
      if (c == '='){
        *type = TOKEN_NEQ;
        printf("[!=]");
        break;
      }
      else if (c == EOF){
        *type = ERROR_LEX;
        printf("ERROR");
        break;
      }
      else if (c == EOL){
        *type = ERROR_LEX;
        printf("ERROR");
        break;
      }
      else{
        ungetc(c,stdin);
        *type = ERROR_LEX;
        printf("ERROR");
        break;
      }
      break;

      case '"' :
      //string
      while(true){
        c = fgetc(stdin);
        if (c == EOF){
          *type = ERROR_LEX;
          printf("ERROR");
          break;
        }
        else if (c == EOL){
          *type = ERROR_LEX;
          printf("ERROR");
          break;
        }
        else if (c == '"'){
          *type = TOKEN_STRING;
          printf("[STRING]");
          break;
        }
      }
      break;



      case ':' :
      c = fgetc(stdin);
      // :=
      if (c == '='){
        *type = TOKEN_DEFINITION;
        printf("[:=]");
        break;
      }
      else if (c == EOF){
        *type = ERROR_LEX;
        printf("ERROR_LEX");
        break;
      }
      else if (c == EOL){
        *type = ERROR_LEX;
        printf("ERROR_LEX");
        break;
      }
      else{
        ungetc(c,stdin);
        *type = ERROR_LEX;
        printf("ERROR");
        break;
        }
      break;




      default :
      // ID
      if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_'){
        str_add_char(actual_value,c);
        c = fgetc(stdin);
        if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9'){
        //  str_add_char(actual_value,c);
          c = fgetc(stdin);
          while (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9'){
          //  str_add_char(actual_value,c);
            c = fgetc(stdin);
            continue;
          }
          /*isKeyword(type,string *actual_value);

          if (isKeyword == -2){
            *type = TOKEN_ID;
          }
          else{
            return *type;
          }
*/

          *type = TOKEN_ID;
          ungetc(c,stdin);
          printf("[ID]");

          break;
        }
        // jeden symbol
        else
        {
          *type = TOKEN_ID;
          printf("[ID]");
          ungetc(c, stdin);
          break;
        }
        //miesto pre string na isKeywords
      }


      //cisla
      int x = 1;
      if(c >= '0' && c <= '9'){
        c = fgetc(stdin);
        if (c >= '1' && c <= '9'){
          while (c >= '0' && c <= '9'){
            c = fgetc(stdin);
            //desatinná časť
            if (c == '.'){
              c = fgetc(stdin);
              if (c >= '1' && c <= '9'){
                  while (c >= '0' && c <= '9'){
                    c = fgetc(stdin);
                    continue;
                  }
                  *type = TOKEN_FLOAT;
                  ungetc(c,stdin);
                  printf("[FLOAT]");
                  x = 0;
                  break;
              }
            }

            continue;
          }
          if(x==1){
            *type = TOKEN_INTEGER;
            ungetc(c,stdin);
            printf("[INT]");
            break;
          }
        }
        else if(c == '.'){
          c = fgetc(stdin);
          if (c >= '1' && c <= '9'){
              while (c >= '0' && c <= '9'){
                c = fgetc(stdin);
                if (c == '.'){
                  *type = ERROR_LEX;
                  break;
                }
                continue;
              }
              *type = TOKEN_FLOAT;
              ungetc(c,stdin);
              printf("[FLOAT]");
              break;
          }
        }
        else{
          *type = TOKEN_INTEGER;
          printf("[INT]");
          ungetc(c, stdin);
          break;
        }
      }

    }// end of switch

  }// end of while

}// end of getToken


//check the keywords
void isKeyword(int *type, char *tmp){
  if (strcmp(tmp, "if") == 0){
    *type = TOKEN_IF;
  }
  else if (strcmp(tmp, "else") == 0){
    *type = TOKEN_ELSE;
  }
  else if (strcmp(tmp, "package") == 0){
    *type = TOKEN_PACKAGE;
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
/*  else{
    return -2;
  }*/

}// end of isKeyword

void main(){
  int *type;
  string *yeet;
  getToken(type, yeet);
  return;
}
