#ifndef MESINKARAKTER_H
#define MESINKARAKTER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../Boolean/Boolean.h"

#define CHAR_MARK ';'
#define BLANK ' '
#define NEWLINE '\n'

#define MODE_INPUT 0
#define MODE_CSV_FILE 1
#define MODE_CONF_FILE 2
#define MODE_CSV_BUFFER 3
#define MODE_JSON_FILE 4

extern char currentChar;
extern boolean EOP;
extern FILE *pita;
extern int MODE;

// INPUT
void STARTINPUT();
void ADVINPUT();

// CSV
void STARTCSV(const char *filename);
void ADVCSV();

// CONF
void STARTCONF(const char *filename);
void ADVCONF();

// CSV buffer mode
void STARTCSV_BUFFER(uint8_t *buffer, int len);
void ADVCSV_BUFFER();

// JSON
extern char jsonChar;
extern boolean JSON_EOF;
void STARTJSON(const char *filename);
void ADVJSON();
void IgnoreWSJSON();
int CopyJSONString(char *out, int maxLen);

// General
void CLOSEPITA();

// Helper
void IgnoreBlanks();
void IgnoreNewline();

#endif