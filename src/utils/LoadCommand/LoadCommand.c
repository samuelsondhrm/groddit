#include "LoadCommand.h"

void performLoad(const char *folder)
{
    printf("Anda akan melakukan pemuatan dari %s.\n\n", folder);
    printf("Mohon tunggu...\n");

    // Waktu jeda 3 detik
    for (int i = 1; i <= 3; i++)
    {
        printf("%d...\n", i);
        sleep(1);
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

    printf("Pemuatan selesai!\n\n");
}

void commandLoad()
{
    // Mengecek apakah user sedang login atau tidak
    if (CURRENT_USER_INDEX != -1)
    {
        printf("Anda harus keluar terlebih dahulu untuk melakukan perintah ini.\n\n");
        return;
    }

    int folderValid = 0;
    char folder[50];

    while (!folderValid)
    {
        printf("Masukkan nama folder yang hendak dimuat.\n");
        fflush(stdout);

        STARTWORD_INPUT();

        if (currentWord.Length == 0)
        {
            printf("[Error] Nama folder tidak boleh kosong!\n");
            continue;
        }

        wordToString(folder, currentWord);

        if (!isFolderValid(folder))
        {
            printf("Tidak ada folder yang dimaksud!\n");
            continue;
        }

        folderValid = 1;
    }

    printf("\n");
    performLoad(folder);
}
