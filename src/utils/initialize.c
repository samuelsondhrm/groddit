#include "header/initialize.h"

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
    fflush(stdout);

    MODE = MODE_INPUT;
    STARTWORD_INPUT();

    if (currentWord.Length == 0)
    {
        printf("[Error] Nama folder tidak boleh kosong!\n");
        printf("Masukkan folder konfigurasi untuk dimuat: ");
        fflush(stdout);
        STARTWORD_INPUT();
    }

    printf("\n");

    char folder[50];
    wordToString(folder, currentWord);

    // Comments
    char pathComments[150];
    buildPath(pathComments, folder, "comment.csv");

    COMMENT_COUNT = loadComments(pathComments, &COMMENTS, &COMMENT_CAPACITY);

    if (COMMENT_COUNT == -1)
    {
        printf("[Gagal] Memuat komentar dari %s\n", pathComments);
    }
    else
    {
        printf("[OK] Memuat %d komentar dari %s\n", COMMENT_COUNT, pathComments);
    }

    // Posts
    char pathPosts[150];
    buildPath(pathPosts, folder, "post.csv");
    POST_COUNT = loadPosts(pathPosts, &POSTS, &POST_CAPACITY);
    if (POST_COUNT == -1)
        printf("[Gagal] Memuat posts dari %s\n", pathPosts);
    else
        printf("[OK] Memuat %d posts dari %s\n", POST_COUNT, pathPosts);

    // Users
    char pathUsers[150];
    buildPath(pathUsers, folder, "user.csv");
    USER_COUNT = loadUsers(pathUsers, &USERS, &USER_CAPACITY);
    if (USER_COUNT == -1)
        printf("[Gagal] Memuat users dari %s\n", pathUsers);
    else
        printf("[OK] Memuat %d users dari %s\n", USER_COUNT, pathUsers);

    // SubGroddits
    char pathSubs[150];
    buildPath(pathSubs, folder, "subgroddit.csv");
    SUBGRODDIT_COUNT = loadSubGroddits(pathSubs, &SUBGRODDITS, &SUBGRODDIT_CAPACITY);
    if (SUBGRODDIT_COUNT == -1)
        printf("[Gagal] Memuat subgroddits dari %s\n", pathSubs);
    else
        printf("[OK] Memuat %d subgroddits dari %s\n", SUBGRODDIT_COUNT, pathSubs);

    // Socials
    char pathSocials[150];
    buildPath(pathSocials, folder, "social.csv");
    SOCIAL_COUNT = loadSocials(pathSocials, &SOCIALS, &SOCIAL_CAPACITY);
    if (SOCIAL_COUNT == -1)
        printf("[Gagal] Memuat socials dari %s\n", pathSocials);
    else
        printf("[OK] Memuat %d socials dari %s\n", SOCIAL_COUNT, pathSocials);

    // Votings
    char pathVotings[150];
    buildPath(pathVotings, folder, "voting.csv");
    VOTING_COUNT = loadVotings(pathVotings, &VOTINGS, &VOTING_CAPACITY);
    if (VOTING_COUNT == -1)
        printf("[Gagal] Memuat votings dari %s\n", pathVotings);
    else
        printf("[OK] Memuat %d votings dari %s\n", VOTING_COUNT, pathVotings);
}
