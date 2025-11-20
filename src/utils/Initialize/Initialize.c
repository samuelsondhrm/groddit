#include "Initialize.h"
#include <sys/stat.h>

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

int isFolderValid(const char *folder)
{
    char path[150];
    buildPath(path, folder, "comment.csv");

    // Remove the filename to check if folder exists
    int i = 0;
    while (path[i] != '\0' && path[i] != '\n')
        i++;

    // Find last slash
    int lastSlash = i - 1;
    while (lastSlash >= 0 && path[lastSlash] != '/')
        lastSlash--;

    if (lastSlash >= 0)
    {
        path[lastSlash] = '\0';
        struct stat st;
        return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
    }
    return 0;
}

void initialize()
{
    system("clear");
    printf("Selamat datang di Groddit!\n\n");

    char folder[50];
    int folderValid = 0;

    while (!folderValid)
    {
        printf("Masukkan folder konfigurasi untuk dimuat: ");
        fflush(stdout);

        MODE = MODE_INPUT;
        STARTWORD_INPUT();

        if (currentWord.Length == 0)
        {
            printf("[Error] Nama folder tidak boleh kosong!\n\n");
            continue;
        }

        wordToString(folder, currentWord);

        if (!isFolderValid(folder))
        {
            printf("[Error] Folder '%s' tidak ditemukan. Silakan masukkan folder yang tepat.\n\n", folder);
            continue;
        }

        folderValid = 1;
    }

    printf("\n");

    // Comments
    char pathComments[150];
    buildPath(pathComments, folder, "comment.csv");

    COMMENT_COUNT = loadComments(pathComments, &COMMENTS, &COMMENT_CAPACITY);

    if (COMMENT_COUNT == -1)
    {
        printf("[Gagal] Memuat komentar dari %s\n", pathComments);
    }

    // Posts
    char pathPosts[150];
    buildPath(pathPosts, folder, "post.csv");
    POST_COUNT = loadPosts(pathPosts, &POSTS);
    if (POST_COUNT == -1)
        printf("[Gagal] Memuat posts dari %s\n", pathPosts);

    // Users
    char pathUsers[150];
    buildPath(pathUsers, folder, "user.csv");
    USER_COUNT = loadUsers(pathUsers, &USERS, &USER_CAPACITY);
    if (USER_COUNT == -1)
        printf("[Gagal] Memuat users dari %s\n", pathUsers);

    // SubGroddits
    char pathSubs[150];
    buildPath(pathSubs, folder, "subgroddit.csv");
    SUBGRODDIT_COUNT = loadSubGroddits(pathSubs, &SUBGRODDITS);
    if (SUBGRODDIT_COUNT == -1)
        printf("[Gagal] Memuat subgroddits dari %s\n", pathSubs);

    // Socials
    char pathSocials[150];
    buildPath(pathSocials, folder, "social.csv");
    SOCIAL_COUNT = loadSocials(pathSocials, &SOCIALS, &SOCIAL_CAPACITY);
    if (SOCIAL_COUNT == -1)
        printf("[Gagal] Memuat socials dari %s\n", pathSocials);

    // Votings
    char pathVotings[150];
    buildPath(pathVotings, folder, "voting.csv");
    VOTING_COUNT = loadVotings(pathVotings, &VOTINGS, &VOTING_CAPACITY);
    if (VOTING_COUNT == -1)
        printf("[Gagal] Memuat votings dari %s\n", pathVotings);
}
