#include <stdio.h>
#include "../../adt/header/mesinkata.h"
#include "load-config.h"
#include "../../adt/header/adt-sederhana.h"
#include "global-data.h"

// Konversi word ke string
int wordToString(char *dest, Word w);

// Membentuk path
void buildPath(char *out, const char *folder, const char *file);

// Inisialisasi
void initialize();
