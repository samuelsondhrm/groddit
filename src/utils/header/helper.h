#include <stdio.h>
#include <stdlib.h>

// Membaca field pada .csv
int readField(char *dest, int maxLen, FILE *f);

// Mengubah string menjadi integer
int toInt(const char *s);

// Menyalin suatu string
void copyString(char *dst, const char *src, int maxlen);

// Menjamin kapasitas array dinamis. Jika kurang, akan diperbesar 2x lipat
int ensureCapacity(void **arrPtr, int *capacity, size_t elemSize, int needed);
