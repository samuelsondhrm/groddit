#include <stdio.h>
#include <stdlib.h>
#include "../../adt/header/MesinKata.h"
#include <time.h>

// Membaca field pada .csv
int readField(char *dest, int maxLen, FILE *f);

// Mengubah string menjadi integer
int toInt(const char *s);

// Menentukan panjang string
int strLength(const char *s);

// Membandingkan dua buah string apakah sama atau tidak
int strCmp(const char *s1, const char *s2);

// Menyalin suatu string statis
void copyString(char *dst, const char *src, int maxlen);

// Menyalin suatu string dinamis
void copyStringDynamic(char **dst, const char *src);

// Menjamin kapasitas array dinamis. Jika kurang, akan diperbesar 2x lipat
int ensureCapacity(void **arrPtr, int *capacity, size_t elemSize, int needed);

// Konversi word ke string
int wordToString(char *dest, Word w);

// Mengubah format time_t ke string
char *convertTimeToStr(time_t t);

// Mengubah format string ke time_t
time_t parseTime(const char *str);