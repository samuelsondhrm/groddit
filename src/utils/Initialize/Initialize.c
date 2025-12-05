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
    // system("clear");


    printf("Loading in...............");
    printf("\n");
    loadingBarSmooth(30, 50000);

    printf("\n\n");
    printBanner();
    printf("\n\n");
    printf("Selamat datang di Groddit: CREDIT!\n");

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
        printf("\nMemuat data dari folder '%s'..........\n", folder);
        // loadingBarSmooth(30, 10000);
        loadingBarSmooth(30, 50000);
        printf("Data berhasil dimuat dari folder '%s'!\n", folder);
        printf("\n");
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
    createGraph(&SOCIAL_GRAPH, USER_COUNT);
    
    char pathSocials[150];
    buildPath(pathSocials, folder, "social.csv");
    SOCIAL_COUNT = loadSocials(pathSocials, &SOCIALS, &SOCIAL_CAPACITY);
    if (SOCIAL_COUNT == -1) printf("[Gagal] Memuat socials dari %s\n", pathSocials);
    
    // Proses memasukkan edge di global SOCIAL_GRAPH
    for (int i = 0; i < SOCIAL_COUNT; i++) {
        char uId[32], vId[32];
        wordToString_safe(uId, sizeof(uId), SOCIALS[i].user_id);
        wordToString_safe(vId, sizeof(vId), SOCIALS[i].following_id);

        int u = findUserIndexById(uId);
        int v = findUserIndexById(vId);

        if (u != IDX_UNDEF && v != IDX_UNDEF && u != v) {
            addEdge(&SOCIAL_GRAPH, u, v);
        }
    }

    // Votings
    char pathVotings[150];
    buildPath(pathVotings, folder, "voting.csv");
    VOTING_COUNT = loadVotings(pathVotings, &VOTINGS, &VOTING_CAPACITY);
    if (VOTING_COUNT == -1)
        printf("[Gagal] Memuat votings dari %s\n", pathVotings);
}
