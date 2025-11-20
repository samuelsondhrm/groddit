#ifndef MESINKARAKTER_H
#define MESINKARAKTER_H

#include <stdio.h>
#include "../Boolean/Boolean.h"

#define CHAR_MARK ';'
#define BLANK ' '
#define NEWLINE '\n'

#define MODE_INPUT 0
#define MODE_CSV 1

extern char currentChar;
extern boolean EOP;
extern FILE *pita;
extern int MODE;

// I
void STARTINPUT();
void ADVINPUT();

// CSV MODE
void STARTCSV(const char *filename);
void ADVCSV();

// General
void CLOSEPITA();

// Helper
void IgnoreBlanks();
void IgnoreNewline();

#endif
