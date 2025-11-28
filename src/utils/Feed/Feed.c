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

/* Cetak header feed */
static void printFeedHeader(const char *username, boolean isLatest) {
    printf("═════════════════════════════════════════\n");
    printf("📰  Feed: %s (sorted by %s)\n",
           username,
           isLatest ? "LATEST - newest first"
                    : "NEWEST - oldest first");
    printf("═════════════════════════════════════════\n");
}

/* Cetak footer feed */
static void printFeedFooter(void) {
    printf("Gunakan VIEW_POST <ID> untuk melihat detail postingan.\n");
    printf("═════════════════════════════════════════\n");
}

/* Implementasi Utama */

void commandShowFeed() {
    if (!isLoggedIn()) {
        // Bersihin pita
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat mengakses Groddit.\n");
        return;
    }

    // Baca mode: LATEST/NEWEST
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printf("Format perintah SHOW_FEED salah. Gunakan 'SHOW_FEED LATEST [LIMIT];' atau 'SHOW_FEED NEWEST [LIMIT];'\n");
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
        printf("Mode feed tidak dikenal. Gunakan 'LATEST' atau 'NEWEST'.\n");
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
            printf("LIMIT harus berupa bilangan bulat positif.\n");
            return;
        }

        // Error validation jika ada argumen tambahan setelah LIMIT
        ADVWORD_INPUT();
        if (currentWord.Length != 0) {
            while (currentWord.Length != 0) {
                ADVWORD_INPUT();
            }
            printf("Format perintah SHOW_FEED salah. Gunakan 'SHOW_FEED %s [LIMIT];'\n", modeStr);
            return;
        }
    }
    else {
        // Tidak ada LIMIT, biarkan limit = -1
    }

    if (POST_COUNT <= 0) {
        // Case tidak ada post pada followed accounts.
        char currName[256];
        wordToString(currName, USERS[CURRENT_USER_INDEX].username);
        printFeedHeader(currName, isLatest);
        printf("Feed kosong. Belum ada post di Groddit.\n");
        printf("═════════════════════════════════════════\n");
        return;
    }

    boolean *followed = NULL;
    int followedCount = buildFollowedArray(&followed);
    if (followedCount < 0) {
        printf("Terjadi kesalahan pada data sosial.\n");
        return;
    }

    char currName[256];
    wordToString(currName, USERS[CURRENT_USER_INDEX].username);

    if (followedCount == 0) {
        // Case tidak ada yang difollow oleh CURRENT_USER
        printFeedHeader(currName, isLatest);
        printf("Feed kosong. Kamu belum mengikuti siapa pun.\n");
        printf("═════════════════════════════════════════\n");
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
        printFeedHeader(currName, isLatest);
        printf("Feed kosong. Belum ada postingan dari akun yang kamu ikuti.\n");
        printf("═════════════════════════════════════════\n");
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

    printFeedHeader(currName, isLatest);

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

        wordToString(postId, post->post_id);
        wordToString(titleStr, post->title);
        timeToStr(timeStr, post->created_at);
        wordToString(authorName, USERS[authorIdx].username);

        printf("%d. [%s] %s (%s) - oleh %s\n",
               printed + 1,
               postId,
               titleStr,
               timeStr,
               authorName);

        printed++;
    }

    printFeedFooter();

    // dealokasi seluruh resource
    deallocateHeap(&H);
    free(followed);
    free(candidPosts);
    free(candidAuthors);
}
