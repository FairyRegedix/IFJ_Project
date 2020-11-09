//Scanner


#include "error.h"
#include "libraries.h"





switch(c){
  //Komentare
  case '/':
  c = fgetc(f);
  //riadkovy Komentar
  if(c == '/'){
    while(c != EOL){
      c = fgetc(f);
    }
  }
}
