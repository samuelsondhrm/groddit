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

    // Users
    char pathUsers[150];
    buildPath(pathUsers, folder, "user.csv");

    USER_COUNT = loadUsers(pathUsers, &USERS, &USER_CAPACITY);

    if (USER_COUNT == -1)
    {
        printf("[Gagal] Memuat users dari %s\n", pathUsers);
    }
    else
    {
        printf("[OK] Memuat %d user\n", USER_COUNT);
    }

    // Posts
    char pathPosts[150];
    buildPath(pathPosts, folder, "post.csv");

    POST_COUNT = loadPosts(pathPosts, &POSTS, &POST_CAPACITY);

    if (POST_COUNT == -1)
    {
        printf("[Gagal] Memuat posts dari %s\n", pathPosts);
    }
    else
    {
        printf("[OK] Memuat %d post\n", POST_COUNT);
    }

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

    // Subgroddits
    char pathSubgroddits[150];
    buildPath(pathSubgroddits, folder, "subgroddit.csv");

    SUBGRODDIT_COUNT = loadSubgroddits(pathSubgroddits, &SUBGRODDITS, &SUBGRODDIT_CAPACITY);

    if (SUBGRODDIT_COUNT == -1)
    {
        printf("[Gagal] Memuat subgroddits dari %s\n", pathSubgroddits);
    }
    else
    {
        printf("[OK] Memuat %d subgroddits\n", SUBGRODDIT_COUNT);
    }

    // Socials
    char pathSocial[150];
    buildPath(pathSocial, folder, "social.csv");

    SOCIAL_COUNT = loadSocial(pathSocial, &SOCIALS, &SOCIAL_CAPACITY);

    if (SOCIAL_COUNT == -1)
    {
        printf("[Gagal] Memuat social dari %s\n", pathSocial);
    }
    else
    {
        printf("[OK] Memuat %d relasi social\n", SOCIAL_COUNT);
    }

    // Votings
    char pathVoting[150];
    buildPath(pathVoting, folder, "voting.csv");

    VOTING_COUNT = loadVoting(pathVoting, &VOTINGS, &VOTING_CAPACITY);

    if (VOTING_COUNT == -1)
    {
        printf("[Gagal] Memuat voting dari %s\n", pathVoting);
    }
    else
    {
        printf("[OK] Memuat %d voting\n", VOTING_COUNT);
    }

    printf("\nKonfigurasi selesai dimuat!\n");
    printf(">> ");
}
