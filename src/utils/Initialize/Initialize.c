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

    // Load security configuration
    char pathConf[150];
    buildPath(pathConf, folder, "security.conf");
    strcpy(global_security_conf_path, pathConf);

    if (!security_init(pathConf))
    {
        printf("[Peringatan] Gagal memuat konfigurasi keamanan dari %s. Menggunakan pengaturan default.\n", pathConf);
    }

    uint8_t *buf = NULL;
    size_t len = 0;
    boolean enc = false;

    // -------------------
    // COMMENTS (buffer)
    // -------------------
    char pathComments[150];
    buildPath(pathComments, folder, "comment.csv");

    buf = read_encrypted_file(pathComments, &len, &enc);
    if (!buf)
    {
        COMMENT_COUNT = 0;
    }
    else
    {
        COMMENT_COUNT = loadComments_Buffer(buf, len, &COMMENTS, &COMMENT_CAPACITY);
        free(buf);
        buf = NULL;
        len = 0;
        enc = false;
    }

    if (COMMENT_COUNT == -1)
    {
        printf("[Gagal] Memuat komentar dari %s\n", pathComments);
    }

    // -------------------
    // POSTS (buffer)
    // -------------------
    char pathPosts[150];
    buildPath(pathPosts, folder, "post.csv");

    buf = read_encrypted_file(pathPosts, &len, &enc);
    if (!buf)
    {
        POST_COUNT = 0;
    }
    else
    {
        POST_COUNT = loadPosts_Buffer(buf, len, &POSTS);
        free(buf);
        buf = NULL;
        len = 0;
        enc = false;
    }

    if (POST_COUNT == -1)
    {
        printf("[Gagal] Memuat posts dari %s\n", pathPosts);
    }

    // -------------------
    // USERS (buffer)
    // -------------------
    char pathUsers[150];
    buildPath(pathUsers, folder, "user.csv");

    buf = read_encrypted_file(pathUsers, &len, &enc);
    if (!buf)
    {
        USER_COUNT = 0;
    }
    else
    {
        USER_COUNT = loadUsers_Buffer(buf, len, &USERS, &USER_CAPACITY);
        free(buf);
        buf = NULL;
        len = 0;
        enc = false;
    }

    if (USER_COUNT == -1)
    {
        printf("[Gagal] Memuat users dari %s\n", pathUsers);
    }

    // -------------------
    // SUBGRODDITS (buffer)
    // -------------------
    char pathSubs[150];
    buildPath(pathSubs, folder, "subgroddit.csv");

    buf = read_encrypted_file(pathSubs, &len, &enc);
    if (!buf)
    {
        SUBGRODDIT_COUNT = 0;
    }
    else
    {
        SUBGRODDIT_COUNT = loadSubGroddits_Buffer(buf, len, &SUBGRODDITS);
        free(buf);
        buf = NULL;
        len = 0;
        enc = false;
    }

    if (SUBGRODDIT_COUNT == -1)
    {
        printf("[Gagal] Memuat subgroddits dari %s\n", pathSubs);
    }

    // -------------------
    // SOCIALS (buffer)
    // -------------------
    createGraph(&SOCIAL_GRAPH, USER_COUNT);

    char pathSocials[150];
    buildPath(pathSocials, folder, "social.csv");

    buf = read_encrypted_file(pathSocials, &len, &enc);
    if (!buf)
    {
        SOCIAL_COUNT = 0;
    }
    else
    {
        SOCIAL_COUNT = loadSocials_Buffer(buf, len, &SOCIALS, &SOCIAL_CAPACITY);
        free(buf);
        buf = NULL;
        len = 0;
        enc = false;
    }

    if (SOCIAL_COUNT == -1)
    {
        printf("[Gagal] Memuat socials dari %s\n", pathSocials);
    }

    for (int i = 0; i < SOCIAL_COUNT; i++)
    {
        char uId[32], vId[32];
        wordToString_safe(uId, sizeof(uId), SOCIALS[i].user_id);
        wordToString_safe(vId, sizeof(vId), SOCIALS[i].following_id);

        int u = findUserIndexById(uId);
        int v = findUserIndexById(vId);

        if (u != IDX_UNDEF && v != IDX_UNDEF && u != v)
        {
            addEdge(&SOCIAL_GRAPH, u, v);
        }
    }

    // -------------------
    // VOTINGS (buffer)
    // -------------------
    char pathVotings[150];
    buildPath(pathVotings, folder, "voting.csv");

    buf = read_encrypted_file(pathVotings, &len, &enc);
    if (!buf)
    {
        VOTING_COUNT = 0;
    }
    else
    {
        VOTING_COUNT = loadVotings_Buffer(buf, len, &VOTINGS, &VOTING_CAPACITY);
        free(buf);
        buf = NULL;
        len = 0;
        enc = false;
    }

    if (VOTING_COUNT == -1)
    {
        printf("[Gagal] Memuat votings dari %s\n", pathVotings);
    }

    // -------------------
    // CONTENT MODERATION
    // -------------------
    char pathBlacklistWords[150];
    buildPath(pathBlacklistWords, folder, "blacklisted_words.json");

    if (!content_moderation_init(pathBlacklistWords))
    {
        printf("[Warning] Content moderation tidak aktif.\n");
    }
}