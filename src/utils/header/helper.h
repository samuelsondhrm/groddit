#ifndef HELPER_H
#define HELPER_H

#include "../../adt/header/MesinKata.h"
#include <stdlib.h>
#include <time.h>

// Membandingkan dua Word
int compareWord(Word a, Word b);

// Mengonversi Word ke integer
int wordToInt(Word w);

// Menyalin Word
void copyWord(Word *dest, Word src);

// Memeriksa apakah Word kosong
int isWordEmpty(Word w);

// Mengonversi Word ke string
void wordToString(char *out, Word w);

// Mengonversi string ke Word
void stringToWord(Word *out, const char *str);

// Memastikan kapasitas array cukup, realloc jika perlu
int ensureCapacity(void **array, int *capacity, int elementSize, int needed);

// Membandingkan dua string
int strCmp(const char *a, const char *b);

// Mengubah format time_t ke string
void timeToStr(char *out, time_t t);

// Mengubah format string ke time_t
time_t parseTime(const char *str);

#endif
