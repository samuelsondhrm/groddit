#include <stdio.h>
#include "../../adt/header/mesinkata.h"
#include "../../adt/header/adt-sederhana.h"
#include "header/initialize.h"
#include "header/load-config.h"
#include "header/global-data.h"

void buildPath(char *out, const char *folder, const char *file)
{
    int p = 0;
    const char *root = "config/";

    int i = 0;
    while (root[i] != '\0')
        out[p++] = root[i++];

    i = 0;
    while (folder[i] != '\0')
        out[p++] = folder[i++];

    out[p++] = '/';

    i = 0;
    while (file[i] != '\0')
        out[p++] = file[i++];

    out[p] = '\0';
}

void initialize()
{
    printf("Selamat datang di Groddit!\n\n");
    printf("Masukkan folder konfigurasi untuk dimuat: ");

    STARTWORD();
    IgnoreNewline();
    IgnoreBlanks();

    printf("\n");

    char folder[50];
    wordToString(folder, currentWord);

    // Comments
    char pathComments[150];
    buildPath(pathComments, folder, "comment.csv");

    COMMENT_COUNT = loadComments(pathComments, &COMMENTS, &COMMENT_CAPACITY);

    if (COMMENT_COUNT == -1)
    {
        printf("[Gagal] Memuat komentars dari %s\n", pathComments);
    }
    else
    {
        printf("[OK] Memuat %d komentar\n", COMMENT_COUNT);
    }
}
