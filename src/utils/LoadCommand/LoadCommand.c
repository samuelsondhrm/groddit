#include "LoadCommand.h"

void performLoad(const char *folder)
{
    printSectionHeader("", "LOADING PROCESS");
    printf("\n%s %sSource folder:%s %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, folder, RESET);
    printSectionDivider();
    printf("\n");
    
    spinnerAnimation("Preparing to load data", 10);
    printf("\n");

    // Comments
    printf("%s %sLoading comments...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    char pathComments[150];
    buildPath(pathComments, folder, "comment.csv");
    COMMENT_COUNT = loadComments(pathComments, &COMMENTS, &COMMENT_CAPACITY);
    if (COMMENT_COUNT == -1)
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d comments%s\n", GREEN, COMMENT_COUNT, RESET);
    }

    // Posts
    printf("%s %sLoading posts...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    char pathPosts[150];
    buildPath(pathPosts, folder, "post.csv");
    POST_COUNT = loadPosts(pathPosts, &POSTS);
    if (POST_COUNT == -1)
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d posts%s\n", GREEN, POST_COUNT, RESET);
    }

    // Users
    printf("%s %sLoading users...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    char pathUsers[150];
    buildPath(pathUsers, folder, "user.csv");
    USER_COUNT = loadUsers(pathUsers, &USERS, &USER_CAPACITY);
    if (USER_COUNT == -1)
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d users%s\n", GREEN, USER_COUNT, RESET);
    }

    // SubGroddits
    printf("%s %sLoading subgroddits...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    char pathSubs[150];
    buildPath(pathSubs, folder, "subgroddit.csv");
    SUBGRODDIT_COUNT = loadSubGroddits(pathSubs, &SUBGRODDITS);
    if (SUBGRODDIT_COUNT == -1)
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d subgroddits%s\n", GREEN, SUBGRODDIT_COUNT, RESET);
    }

    // Socials
    printf("%s %sLoading social connections...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    char pathSocials[150];
    buildPath(pathSocials, folder, "social.csv");
    SOCIAL_COUNT = loadSocials(pathSocials, &SOCIALS, &SOCIAL_CAPACITY);
    if (SOCIAL_COUNT == -1)
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d connections%s\n", GREEN, SOCIAL_COUNT, RESET);
    }

    // Votings
    printf("%s %sLoading votes...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    char pathVotings[150];
    buildPath(pathVotings, folder, "voting.csv");
    VOTING_COUNT = loadVotings(pathVotings, &VOTINGS, &VOTING_CAPACITY);
    if (VOTING_COUNT == -1)
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }
    else
    {
        printf(" %s[OK] %d votes%s\n", GREEN, VOTING_COUNT, RESET);
    }

    printSectionDivider();
    printf("\n");
    printSuccess("Data loaded successfully!");
    printf("All data has been loaded from %s%s%s\n", BOLD_CYAN, folder, RESET);
}

void commandLoad()
{
    clearScreen();
    printBreadcrumb("Home > Load Data");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                           %sLOAD DATA%s                               %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    // Mengecek apakah user sedang login atau tidak
    if (CURRENT_USER_INDEX != -1)
    {
        printError("Cannot load while logged in");
        printf("You must logout first to perform this operation.\n\n");
        printf("%sTip:%s Use %sLOGOUT;%s to logout from current session.\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    int folderValid = 0;
    char folder[50];

    while (!folderValid)
    {
        printf("\n");
        printSectionHeader("", "FOLDER SELECTION");
        printf("\n%s %sEnter configuration folder name:%s\n", BOX_V, BOLD_WHITE, RESET);
        printSectionDivider();
        printInputPrompt("FOLDER NAME");
        fflush(stdout);

        STARTWORD_INPUT();
        printf("\n");

        if (currentWord.Length == 0)
        {
            printError("Empty folder name");
            printf("Folder name cannot be empty!\n\n");
            printf("%sAvailable folders:%s %sconfig-1, FileEnc, PwHashed, etc.%s\n", 
                   DIM, RESET, BOLD_WHITE, RESET);
            continue;
        }

        wordToString(folder, currentWord);

        if (!isFolderValid(folder))
        {
            printError("Folder not found");
            printf("Configuration folder \"%s%s%s\" does not exist!\n\n", 
                   BOLD_RED, folder, RESET);
            printf("%sTip:%s Check %sconfig/%s directory for available folders.\n", 
                   BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            continue;
        }

        folderValid = 1;
    }

    printf("\n");
    performLoad(folder);
}
