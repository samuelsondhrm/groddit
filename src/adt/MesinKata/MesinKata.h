#ifndef MESINKATA_H
#define MESINKATA_H

#include "../MesinKarakter/MesinKarakter.h"

#define NMax 200
#define BLANK ' '

typedef struct
{
    char TabWord[NMax];
    int Length;
} Word;

extern Word currentWord;

// INPUT MODE
extern boolean EndWordInput;
void STARTWORD_INPUT();
void ADVWORD_INPUT();

// CSV MODE
extern boolean EndWordCSV;
void STARTWORD_CSV(const char *filename);
void ADVWORD_CSV();

void CopyFieldCSV();
void CopyWordInput();

#endif
