#include "Feed.h"

/* Helper Internal */

// Cari index user berdasarkan user_id (Word), misal: "USER001"
static int findUserIndexByUserIdWord(Word userId) {
    char target[32];
    wordToString(target, userId);

    for (int i = 0; i < USER_COUNT; i++) {
        char curr[32];
        wordToString(curr, USERS[i].user_id);

        if (strCmp(curr, target) == 0) {
            return i;
        }
    }
    return -1;
}

/* Siapkan array boolean: followed[i] = true jika current user follow user i */
static int buildFollowedArray(boolean **followedOut) {
    if (USER_COUNT <= 0) {
        *followedOut = NULL;
        return 0;
    }

    boolean *followed = (boolean *) malloc(USER_COUNT * sizeof(boolean));
    if (!followed) {
        *followedOut = NULL;
        return -1;
    }

    for (int i = 0; i < USER_COUNT; i++) {
        followed[i] = false;
    }

    int count = 0;
    if (!isValidVertex(SOCIAL_GRAPH, CURRENT_USER_INDEX)) {
        free(followed);
        *followedOut = NULL;
        return -1;
    }

    AdjAddr p = SOCIAL_GRAPH.adj[CURRENT_USER_INDEX];
    while (p != NULL) {
        int v = p->v;
        if (v >= 0 && v < USER_COUNT && !followed[v]) {
            followed[v] = true;
            count++;
        }
        p = p->next;
    }

    *followedOut = followed;
    return count;
}

/* Implementasi Utama */

void commandShowFeed() {
    if (!isLoggedIn()) {
        // Bersihin pita
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Not logged in");
        printf("You must be logged in to view feed.\n");
        printf("\nUse %sLOGIN;%s to sign in to your account.\n", BOLD_WHITE, RESET);
        return;
    }

    // Baca mode: LATEST/NEWEST
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printError("Invalid command format");
        printf("Use: %sSHOW_FEED LATEST [LIMIT];%s or %sSHOW_FEED NEWEST [LIMIT];%s\n", 
               BOLD_WHITE, RESET, BOLD_WHITE, RESET);
        return;
    }

    char modeStr[32];
    wordToString(modeStr, currentWord);

    boolean isLatest;
    if (strCmp(modeStr, "LATEST") == 0) {
        isLatest = true;
    } else if (strCmp(modeStr, "NEWEST") == 0) {
        isLatest = false;
    } else {
        // Bersihin pita
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Invalid mode");
        printf("Mode %s%s%s not recognized. Use %sLATEST%s or %sNEWEST%s.\n", 
               BOLD_YELLOW, modeStr, RESET, BOLD_WHITE, RESET, BOLD_WHITE, RESET);
        return;
    }

    int limit = -1; // LIMIT = -1 : Baca semua feed dari following

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        limit = wordToInt(currentWord);
        if (limit <= 0) {
            // Bersihin pita
            while (currentWord.Length != 0) {
                ADVWORD_INPUT();
            }
            printError("Invalid LIMIT");
            printf("LIMIT must be a positive integer.\n");
            return;
        }

        // Error validation jika ada argumen tambahan setelah LIMIT
        ADVWORD_INPUT();
        if (currentWord.Length != 0) {
            while (currentWord.Length != 0) {
                ADVWORD_INPUT();
            }
            printError("Too many arguments");
            printf("Use: %sSHOW_FEED %s [LIMIT];%s\n", BOLD_WHITE, modeStr, RESET);
            return;
        }
    }
    else {
        // Tidak ada LIMIT, biarkan limit = -1
    }

    if (POST_COUNT <= 0) {
        // Case tidak ada post pada followed accounts.
        clearScreen();
        printBreadcrumb("Main Menu > Feed");
        printf("\n");
        printWarning("No posts available");
        printf("There are no posts yet in Groddit.\n");
        printf("\n%sTip:%s Use %sPOST;%s to create your first post!\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    boolean *followed = NULL;
    int followedCount = buildFollowedArray(&followed);
    if (followedCount < 0) {
        printError("Social graph error");
        printf("An error occurred while processing social data.\n");
        return;
    }

    char currName[256];
    wordToString(currName, USERS[CURRENT_USER_INDEX].username);

    if (followedCount == 0) {
        // Case tidak ada yang difollow oleh CURRENT_USER
        clearScreen();
        printBreadcrumb("Main Menu > Feed");
        printf("\n");
        printWarning("Empty feed");
        printf("You are not following anyone yet.\n");
        printf("\n%sTip:%s Use %sFOLLOW <username>;%s to follow users and see their posts.\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        if (followed) free(followed);
        return;
    }

    /* Kumpulkan semua post dari user yang di-follow:
       - candidPosts[i] = pointer ke Post
       - candidAuthors[i] = index author di USERS
    */
    Post **candidPosts = (Post **) malloc(POST_COUNT * sizeof(Post *));
    int *candidAuthors = (int *) malloc(POST_COUNT * sizeof(int));
    if (!candidPosts || !candidAuthors) {
        printf("Terjadi kesalahan memori saat menyiapkan feed.\n");
        if (followed) free(followed);
        if (candidPosts) free(candidPosts);
        if (candidAuthors) free(candidAuthors);
        return;
    }

    int candCount = 0;
    Node *pNode = POSTS.head;

    while (pNode != NULL) {
        if (pNode->element.type == TYPE_POST) {
            Post *post = &pNode->element.data.post;

            int authorIdx = findUserIndexByUserIdWord(post->author_id);
            if (authorIdx != -1 &&
                authorIdx < USER_COUNT &&
                followed[authorIdx]) {

                candidPosts[candCount]  = post;
                candidAuthors[candCount] = authorIdx;
                candCount++;
            }
        }
        pNode = pNode->next;
    }

    if (candCount == 0) {
        // Sudah follow orang, tapi belum ada post dari mereka
        clearScreen();
        printBreadcrumb("Main Menu > Feed");
        printf("\n");
        printWarning("No posts from followed users");
        printf("Users you follow haven't posted yet.\n");
        printf("\n%sTip:%s Check back later or follow more users!\n", 
               BOLD_CYAN, RESET);
        free(followed);
        free(candidPosts);
        free(candidAuthors);
        return;
    }

    /* Bangun heap:
       - key     = created_at (cast ke long)
       - dataIdx = index ke candidPosts[] dan candidAuthors[]
       - isMax   = true  untuk LATEST  (newest first)
                   false untuk NEWEST (oldest first)
    */
    Heap H;
    createHeap(&H, candCount, isLatest ? true : false);

    for (int i = 0; i < candCount; i++) {
        HeapElType el;
        el.key = (long) candidPosts[i]->created_at;
        el.dataIdx = i;
        heapPush(&H, el);
    }

    // Tentukan berapa banyak yang mau ditampilkan
    int maxToShow = candCount;
    if (limit > 0 && limit < candCount) {
        maxToShow = limit;
    }

    // Clear screen and show header
    clearScreen();
    printBreadcrumb("Main Menu > Feed");

    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s                    YOUR FEED : %s%-20s%s   %s\n", 
           DBOX_V, BOLD_WHITE, currName, RESET, DBOX_V);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    printf("%s%s %s Sorting:%s %s%s%s  %s%s Posts:%s %s%d%s  ", 
           BOX_V, BOLD_WHITE, "⚡", RESET, 
           isLatest ? BOLD_GREEN : BOLD_YELLOW, 
           isLatest ? "LATEST (newest first)" : "NEWEST (oldest first)", RESET,
           BOLD_WHITE, "", RESET, BOLD_CYAN, maxToShow, RESET);
    if (limit > 0) {
        printf("%s(limited to %d)%s", DIM, limit, RESET);
    }
    printf("\n");
    printSectionDivider();

    int printed = 0;
    while (printed < maxToShow && !isHeapEmpty(H)) {
        HeapElType top;
        heapPop(&H, &top);

        int idx = top.dataIdx;
        Post *post = candidPosts[idx];
        int authorIdx = candidAuthors[idx];

        char postId[32];
        char titleStr[256];
        char timeStr[256];
        char authorName[256];
        char subName[256];

        wordToString(postId, post->post_id);
        wordToString(titleStr, post->title);
        timeToStr(timeStr, post->created_at);
        wordToString(authorName, USERS[authorIdx].username);
        getSubgrodditName(post->subgroddit_id, subName);

        printf("%s\n", BOX_V);
        printf("%s %s%d.%s [%s%s%s] %s%s%s\n", 
               BOX_V, BOLD_CYAN, printed + 1, RESET,
               BOLD_MAGENTA, postId, RESET,
               BOLD_WHITE, titleStr, RESET);
        printf("%s    %s%s%s %s│%s by %s@%s%s %s│%s %s%s%s\n", 
               BOX_V, BOLD_YELLOW, subName, RESET, 
               DIM, RESET, BOLD_CYAN, authorName, RESET,
               DIM, RESET, DIM, timeStr, RESET);
        printf("%s    %s↑ %d%s %s%s%s %s↓ %d%s\n", 
               BOX_V, GREEN, post->upvotes, RESET,
               DIM, "│", RESET,
               RED, post->downvotes, RESET);

        printed++;
    }

    printf("%s\n", BOX_V);
    printSectionDivider();
    printf("\n");
    printInfo("Feed loaded successfully");
    printf("%sTip:%s Use %sVIEW_POST <ID>;%s to view post details and comments.\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET);

    // dealokasi seluruh resource
    deallocateHeap(&H);
    free(followed);
    free(candidPosts);
    free(candidAuthors);
}
