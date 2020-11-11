// IFJ2020

#include <stdio.h>

#ifndef ERROR_H
#define ERROR_H

typedef enum error{
  SUCCESS = 0,  //Success
  ERROR_LEX = 1,  //chyba v rámci lexikálnej analýzy
  ERROR_SYN = 2,  //chyba v rámci syntaktickej analýzy
  ERROR_SEM_DEF = 3,  //sémantická chyba v programe - nedefinovaná funkcia/premenná
  ERROR_SEM_DAT = 4,  //sémantická chyba pri odvodzovaní dátového typu novej definovanej premennej
  ERROR_SEM_COMP = 5,  //sémantická chyba typovej kompatibility v aritmetických reťazových a relačných výrazoch
  ERROR_SEM_PAR = 6,  //sémantická chyba v programe - zlý počet/typ parametrov či návratových hodnôť u volania/návratu funkcie
  ERROR_SEM_OTHER = 7,  //ostatné sémantické chyby
  ERROR_NULL = 9,  //sémantická chyba delenia nulovou konštantou
  ERROR_STR  = 10, //chyba pri praci s retazcami
  ERROR_TRANS = 99,  //interná chyba prekladača (napr chyba alokácie, pamäti ...)
} ERROR;

#endif
