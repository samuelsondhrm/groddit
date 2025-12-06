#include "Social.h"

static int getUserIndexOrReport(const char *username) {
    int idx = findIdByUsername(username);
    if (idx == IDX_UNDEF) {
        printf("User dengan username \"%s\" tidak ditemukan.\n", username);
    }
    return idx;
}

static boolean wordEquals(Word a, Word b) {
    char sa[32], sb[32];
    wordToString_safe(sa, sizeof(sa), a);
    wordToString_safe(sb, sizeof(sb), b);
    return strcmp(sa, sb) == 0;
}

static void addSocialBuffer(int u, int v) {
    if (!ensureCapacity((void**)&SOCIALS, &SOCIAL_CAPACITY,
                        sizeof(Social), SOCIAL_COUNT + 1)) {
        printf("[Warning] Gagal memperbesar buffer SOCIALS.\n");
        return;
    }

    initSocial(&SOCIALS[SOCIAL_COUNT]);

    /* SOCIALS menyimpan user_id string, ambil dari USERS[idx].user_id (Word) */
    copyWord(&SOCIALS[SOCIAL_COUNT].user_id, USERS[u].user_id);
    copyWord(&SOCIALS[SOCIAL_COUNT].following_id, USERS[v].user_id);

    SOCIAL_COUNT++;
}

static void removeSocialBuffer(int u, int v) {
    for (int i = 0; i < SOCIAL_COUNT; i++) {
        if (wordEquals(SOCIALS[i].user_id, USERS[u].user_id) &&
            wordEquals(SOCIALS[i].following_id, USERS[v].user_id)) {

            /* geser kiri untuk menutup gap */
            for (int j = i; j < SOCIAL_COUNT - 1; j++) {
                SOCIALS[j] = SOCIALS[j + 1];
            }
            SOCIAL_COUNT--;
            return;
        }
    }
}


/* ============================================================================
 *                          Fitur FOLLOW / UNFOLLOW
 * ============================================================================
 */

void socialFollowUser(const char *targetUsername) {
    clearScreen();
    printBreadcrumb("Home > Follow User");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                          %sFOLLOW USER%s                              %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to follow users.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    int src = CURRENT_USER_INDEX;
    
    printf("\n");
    spinnerAnimation("Validating user", 6);
    
    int dst = getUserIndexOrReport(targetUsername);
    if (dst == IDX_UNDEF) {
        printf("\n");
        printError("User not found");
        printf("No user exists with username: %s@%s%s\n\n", BOLD_RED, targetUsername, RESET);
        printf("%sTip:%s Check the username spelling.\n", BOLD_CYAN, RESET);
        return;
    }

    if (src == dst) {
        printf("\n");
        printError("Invalid action");
        printf("You cannot follow yourself.\n");
        return;
    }

    if (!isValidVertex(SOCIAL_GRAPH, src) || !isValidVertex(SOCIAL_GRAPH, dst)) {
        printf("\n");
        printError("System error");
        printf("Social graph data corrupted.\n");
        return;
    }

    if (isAdjacent(SOCIAL_GRAPH, src, dst)) {
        printf("\n");
        printWarning("Already following");
        printf("You are already following %s@%s%s\n", BOLD_CYAN, findUsernameById(dst), RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Processing follow request", 8);

    /* 1) update graph */
    addEdge(&SOCIAL_GRAPH, src, dst);

    /* 2) update buffer SOCIALS */
    addSocialBuffer(src, dst);

    printf("\n");
    printSuccess("Follow successful");
    printf("You are now following %s@%s%s\n", BOLD_GREEN, findUsernameById(dst), RESET);
}

void socialUnfollowUser(const char *targetUsername) {
    clearScreen();
    printBreadcrumb("Home > Unfollow User");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                         %sUNFOLLOW USER%s                             %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to unfollow users.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    int src = CURRENT_USER_INDEX;
    
    printf("\n");
    spinnerAnimation("Validating user", 6);
    
    int dst = getUserIndexOrReport(targetUsername);
    if (dst == IDX_UNDEF) {
        printf("\n");
        printError("User not found");
        printf("No user exists with username: %s@%s%s\n\n", BOLD_RED, targetUsername, RESET);
        printf("%sTip:%s Check the username spelling.\n", BOLD_CYAN, RESET);
        return;
    }

    if (!isValidVertex(SOCIAL_GRAPH, src) || !isValidVertex(SOCIAL_GRAPH, dst)) {
        printf("\n");
        printError("System error");
        printf("Social graph data corrupted.\n");
        return;
    }

    if (!isAdjacent(SOCIAL_GRAPH, src, dst)) {
        printf("\n");
        printWarning("Not following");
        printf("You are not following %s@%s%s\n\n", BOLD_CYAN, findUsernameById(dst), RESET);
        printf("%sTip:%s Use %sFOLLOWING;%s to see who you follow.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Processing unfollow request", 8);

    /* 1) update graph */
    removeEdge(&SOCIAL_GRAPH, src, dst);

    /* 2) update buffer SOCIALS */
    removeSocialBuffer(src, dst);

    printf("\n");
    printSuccess("Unfollow successful");
    printf("You have unfollowed %s@%s%s\n", BOLD_YELLOW, findUsernameById(dst), RESET);
}


/* ============================================================================
 *                          Fitur FOLLOWING / FOLLOWERS
 * ============================================================================
 */

void socialShowFollowing(const char *username) {
    clearScreen();
    printBreadcrumb("Home > Following List");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                         %sFOLLOWING LIST%s                            %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    int idx;

    // Jika username NULL → gunakan current user
    if (username == NULL || username[0] == '\0') {
        if (!isLoggedIn()) {
            printError("Authentication required");
            printf("You must be logged in to view following list.\n\n");
            printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            return;
        }
        idx = CURRENT_USER_INDEX;
    } else {
        printf("\n");
        spinnerAnimation("Loading user profile", 6);
        idx = getUserIndexOrReport(username);
        if (idx == IDX_UNDEF) {
            printf("\n");
            printError("User not found");
            printf("No user exists with username: %s@%s%s\n", BOLD_RED, username, RESET);
            return;
        }
    }

    if (!isValidVertex(SOCIAL_GRAPH, idx)) {
        printError("System error");
        printf("Social graph data corrupted.\n");
        return;
    }

    printf("\n");
    printSectionHeader("", "ACCOUNTS FOLLOWED");
    printf("\n%s %sUser:%s %s@%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, findUsernameById(idx), RESET);
    printSectionDivider();
    printf("\n");

    AdjAddr p = SOCIAL_GRAPH.adj[idx];
    if (p == NULL) {
        printf("%s %s(Not following anyone)%s\n", BOX_V, DIM, RESET);
        printSectionDivider();
        return;
    }

    int count = 0;
    while (p != NULL) {
        count++;
        printf("%s %s%d.%s %s@%s%s\n", BOX_V, BOLD_WHITE, count, RESET, BOLD_CYAN, findUsernameById(p->v), RESET);
        p = p->next;
    }
    printSectionDivider();
    printf("\n");
    printInfo("Total following: " );
    printf("%s%d%s users\n", BOLD_WHITE, count, RESET);
}


void socialShowFollowers(const char *username) {
    clearScreen();
    printBreadcrumb("Home > Followers List");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                         %sFOLLOWERS LIST%s                            %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    int target;

    if (username == NULL || username[0] == '\0') {
        if (!isLoggedIn()) {
            printError("Authentication required");
            printf("You must be logged in to view followers list.\n\n");
            printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
            return;
        }
        target = CURRENT_USER_INDEX;
    } else {
        printf("\n");
        spinnerAnimation("Loading user profile", 6);
        target = getUserIndexOrReport(username);
        if (target == IDX_UNDEF) {
            printf("\n");
            printError("User not found");
            printf("No user exists with username: %s@%s%s\n", BOLD_RED, username, RESET);
            return;
        }
    }

    if (!isValidVertex(SOCIAL_GRAPH, target)) {
        printError("System error");
        printf("Social graph data corrupted.\n");
        return;
    }

    printf("\n");
    printSectionHeader("", "FOLLOWERS");
    printf("\n%s %sUser:%s %s@%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, findUsernameById(target), RESET);
    printSectionDivider();
    printf("\n");

    boolean ada = false;
    int count = 0;
    for (int u = 0; u < SOCIAL_GRAPH.nVertex; u++) {
        AdjAddr p = SOCIAL_GRAPH.adj[u];
        while (p != NULL) {
            if (p->v == target) {
                count++;
                printf("%s %s%d.%s %s@%s%s\n", BOX_V, BOLD_WHITE, count, RESET, BOLD_CYAN, findUsernameById(u), RESET);
                ada = true;
                break;
            }
            p = p->next;
        }
    }

    if (!ada) {
        printf("%s %s(No followers yet)%s\n", BOX_V, DIM, RESET);
    }
    printSectionDivider();
    printf("\n");
    printInfo("Total followers: ");
    printf("%s%d%s users\n", BOLD_WHITE, count, RESET);
}
