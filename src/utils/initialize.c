#include <stdio.h>
#include "../../adt/header/mesinkata.h"
#include "../../adt/header/adt-sederhana.h"
#include "header/initialize.h"
#include "header/load-config.h"
#include "header/global-data.h"

int wordToString(char *dest, Word w)
{
    for (int i = 0; i < w.Length; i++)
    {
        dest[i] = w.TabWord[i];
    }
    dest[w.Length] = '\0';
    return w.Length;
}

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

    printf("\n");

    char folder[50];
    wordToString(folder, currentWord);

    char pathComments[150];
    buildPath(pathComments, folder, "comment.csv");

    COMMENT_COUNT = loadComments(pathComments, &COMMENTS, &COMMENT_CAPACITY);

    if (COMMENT_COUNT == -1)
    {
        printf("Gagal memuat file konfigurasi!\n\n");
    }
    else
    {
        printf("File konfigurasi berhasil dimuat!\n");
    }

    printf(">> ");
}
