#ifndef CONTENT_MODERATION_H
#define CONTENT_MODERATION_H

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include "../../adt/MesinKata/MesinKata.h"
#include "../../adt/MesinKarakter/MesinKarakter.h"

#define MAX_BLACKLIST 256
#define MAX_WORD_LEN 200
#define MAX_FOUND 64

typedef struct
{
    char words[MAX_BLACKLIST][MAX_WORD_LEN]; // daftar kata terlarang
    int count;                               // jumlah kata terlarang
} Blacklist;

boolean content_moderation_init(const char *pathConf); // inisialisasi moderasi konten

extern Blacklist GLOBAL_BLACKLIST; // blacklist global

int SaveBlacklistJSON(const char *filepath); // simpan blacklist ke file JSON

int LoadBlacklistJSON(const char *filepath); // load blacklist dari file JSON

int CheckBlacklistedContent(const char *text, char foundWords[][MAX_WORD_LEN], int *foundCount);
// cek apakah teks mengandung kata terlarang

void ToLowercaseInplace(char *s); // ubah string ke lowercase

#endif
