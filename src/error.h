/**
 * IFJ Projekt 2020
 *
 * Errors
 *
 * @author <xdemsk00> Patrik Demský
 * @author <xsabol03> František Sabol
 */

#ifndef IFJ_ERROR_H
#define IFJ_ERROR_H


typedef enum error{
  SUCCESS = 0,
  ERROR_LEX = 1,
  ERROR_SYN = 2,
  ERROR_SEM_DEF = 3,
  ERROR_SEM_DAT = 4,
  ERROR_SEM_COMP = 5,
  ERROR_SEM_PAR = 6,
  ERROR_SEM_OTHER = 7,
  ERROR_NULL = 9,
  ERROR_TRANS = 99,
} ERROR;

/*
 * Pretty print for errors.
 * */
int handle_error(ERROR error, char* format, ...);

#endif //IFJ_ERROR_H