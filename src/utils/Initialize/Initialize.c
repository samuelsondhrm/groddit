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
    // Clear screen and show welcome
    clearScreen();
    
    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s                      INITIALIZING GRODDIT SYSTEM                      %s\n", DBOX_V, DBOX_V);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    printf("\n");
    spinnerAnimation("Starting up system", 15);
    printf("\n");

    printBanner();
    printf("\n");
    
    printf("%s%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", 
           BOLD_WHITE, "   ", RESET);
    printf("%s              Welcome to %sGRODDIT%s - Social Media Platform              %s\n", 
           BOLD_WHITE, BOLD_CYAN, BOLD_WHITE, RESET);
    printf("%s%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", 
           BOLD_WHITE, "   ", RESET);

    char folder[50];
    int folderValid = 0;

    printf("\n");
    printSectionHeader("", "CONFIGURATION FOLDER");
    printf("\n");

    while (!folderValid)
    {
        printf("%s %sPlease enter configuration folder name:%s ", BOX_V, BOLD_WHITE, RESET);
        fflush(stdout);

        MODE = MODE_INPUT;
        STARTWORD_INPUT();

        if (currentWord.Length == 0)
        {
            printf("\n");
            printError("Empty folder name");
            printf("Folder name cannot be empty!\n");
            printf("\n");
            continue;
        }

        wordToString(folder, currentWord);

        if (!isFolderValid(folder))
        {
            printf("\n");
            printError("Folder not found");
            printf("Folder %s'%s'%s doesn't exist in config directory.\n", BOLD_YELLOW, folder, RESET);
            printf("%sTip:%s Check available folders in %sconfig/%s directory.\n\n", 
                   BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            continue;
        }

        folderValid = 1;
        printf("\n");
        printSuccess("Folder validated");
        printf("Loading configuration from %s'%s'%s\n", BOLD_CYAN, folder, RESET);
        printf("\n");
        
        loadingBarSmooth(30, 50000);
        printf("\n");
        printSuccess("Configuration loaded");
    }

    printSectionDivider();

    printf("\n");
    printSectionHeader("", "LOADING SECURITY CONFIGURATION");
    
    // Load security configuration
    char pathConf[150];
    buildPath(pathConf, folder, "security.conf");
    strcpy(global_security_conf_path, pathConf);

    printf("%s %sFile:%s %s%s%s\n", BOX_V, BOLD_WHITE, RESET, DIM, pathConf, RESET);
    spinnerAnimation("Processing security settings", 8);
    
    if (!security_init(pathConf))
    {
        printf("\n");
        printWarning("Security configuration failed");
        printf("Using default security settings.\n");
    }
    else
    {
        printf("\n");
        printSuccess("Security configured");
    }

    uint8_t *buf = NULL;
    size_t len = 0;
    boolean enc = false;

    printf("\n");
    printSectionDivider();
    printf("\n");
    printSectionHeader("", "LOADING DATA MODULES");
    printf("\n");

    // -------------------
    // COMMENTS (buffer)
    // -------------------
    printf("%s %s[1/7]%s Loading %sComments%s", BOX_V, BOLD_CYAN, RESET, BOLD_WHITE, RESET);
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
        printf(" %s[FAILED]%s\n", RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d records%s\n", GREEN, COMMENT_COUNT, RESET);
    }

    // -------------------
    // POSTS (buffer)
    // -------------------
    printf("%s %s[2/7]%s Loading %sPosts%s", BOX_V, BOLD_CYAN, RESET, BOLD_WHITE, RESET);
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
        printf(" %s[FAILED]%s\n", RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d records%s\n", GREEN, POST_COUNT, RESET);
    }

    // -------------------
    // USERS (buffer)
    // -------------------
    printf("%s %s[3/7]%s Loading %sUsers%s", BOX_V, BOLD_CYAN, RESET, BOLD_WHITE, RESET);
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
        printf(" %s[FAILED]%s\n", RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d records%s\n", GREEN, USER_COUNT, RESET);
    }

    // -------------------
    // SUBGRODDITS (buffer)
    // -------------------
    printf("%s %s[4/7]%s Loading %sSubgroddits%s", BOX_V, BOLD_CYAN, RESET, BOLD_WHITE, RESET);
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
        printf(" %s[FAILED]%s\n", RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d records%s\n", GREEN, SUBGRODDIT_COUNT, RESET);
    }

    // -------------------
    // SOCIALS (buffer)
    // -------------------
    printf("%s %s[5/7]%s Loading %sSocials%s", BOX_V, BOLD_CYAN, RESET, BOLD_WHITE, RESET);
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
        printf(" %s[FAILED]%s\n", RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d records%s\n", GREEN, SOCIAL_COUNT, RESET);
    }

    printf("%s %s       %s Building social graph...", BOX_V, DIM, RESET);
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
    printf(" %sDone%s\n", GREEN, RESET);

    // -------------------
    // VOTINGS (buffer)
    // -------------------
    printf("%s %s[6/7]%s Loading %sVotings%s", BOX_V, BOLD_CYAN, RESET, BOLD_WHITE, RESET);
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
        printf(" %s[FAILED]%s\n", RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d records%s\n", GREEN, VOTING_COUNT, RESET);
    }

    // -------------------
    // CONTENT MODERATION
    // -------------------
    printf("%s %s[7/7]%s Loading %sContent Moderation%s", BOX_V, BOLD_CYAN, RESET, BOLD_WHITE, RESET);
    char pathBlacklistWords[150];
    buildPath(pathBlacklistWords, folder, "blacklisted_words.json");

    if (!content_moderation_init(pathBlacklistWords))
    {
        printf(" %s⚠ Inactive%s\n", YELLOW, RESET);
    }
    else
    {
        printf(" %s[ACTIVE]%s\n", GREEN, RESET);
    }

    printf("\n");
    printSectionDivider();
    printf("\n");
    printSuccess("System initialized successfully");
    printf("All modules loaded and ready to use!\n");
    printf("\n%sPress ENTER to continue...%s", BOLD_WHITE, RESET);
    getchar();
}