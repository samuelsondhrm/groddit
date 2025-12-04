#ifndef MESINKATA_H
#define MESINKATA_H

#include "../Boolean/Boolean.h"
#include "../MesinKarakter/MesinKarakter.h"
#include <string.h>
#include <stdlib.h>

#define NMax 200

typedef struct
{
    char TabWord[NMax];
    int Length;
} Word;

extern Word currentWord;
extern boolean EndWordInput;

void STARTWORD_INPUT();
void ADVWORD_INPUT();
void CopyWordInput();

extern boolean EndWordCSV;

void STARTWORD_CSV(const char *filename);
void STARTWORD_CSV_BUFFER(uint8_t *buffer, int len);
void ADVWORD_CSV();
void CopyFieldCSV();

extern boolean EndWordCONF;

void STARTWORD_CONF(const char *filename);
void ADVWORD_CONF();
void CopyTokenCONF();
void IgnoreSpacesCONF();

#endif