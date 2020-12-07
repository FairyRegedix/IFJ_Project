/**
 * IFJ Projekt 2020
 *
 * Implementation of linked list ADT for strings
 *
 * @author <xhrmor00> Richard Hrmo
 */

#ifndef IFJ_PROJECT_INSTRUCTION_LIST_H
#define IFJ_PROJECT_INSTRUCTION_LIST_H


/**
 * struct of item in StringList
 */
typedef struct StringElement{
    struct StringElement *ptr;
    char* data;
} *StringElementPtr;

/**
 * struct of Stringlist
 */
typedef struct 
{
    StringElementPtr Actual;
    StringElementPtr First;
    StringElementPtr Last;
} StringList;

/**
 * Initialization of Stringlist
 * 
 * @param StringList pointer to StringList
 */
void InitListString(StringList *);

/**
 * Termination of Stringlist
 * 
 * @param StringList pointer to StringList
 */
void DisposeListString(StringList *);

/**
 * Inserting String to the first position of Stringlist
 * 
 * @param StringList pointer to StringList
 * @param char String of data to insert
 */
void InsertFirstString(StringList *, char *);

/**
 * Inserting String to the last position of Stringlist
 * 
 * @param StringList pointer to StringList
 * @param char String of data to insert
 */
void InsertLastString(StringList *List, char *val);

/**
 * Termination of first String in Stringlist
 * 
 * @param StringList pointer to StringList
 */
void DeleteFirstString(StringList *);

#endif //IFJ_PROJECT_INSTRUCTION_LIST_H