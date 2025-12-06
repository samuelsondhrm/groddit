#include "FollRec.h"

#define MAX_REC_DEPTH 3
#define MAX_REC_RESULTS 10

// Helper internal

// Generate array followed[i] = true jika current user follow i
static void buildFollowedArray(boolean *followed, int n, int src) {
    for (int i = 0; i < n; i++) {
        followed[i] = false;
    }

    if (!isValidVertex(SOCIAL_GRAPH, src)) return;

    AdjAddr p = SOCIAL_GRAPH.adj[src];
    while (p != NULL) {
        int v = p->v;
        if (v >= 0 && v < n) {
            followed[v] = true;
        }
        p = p->next;
    }
}

// Cetak rekomendasi kosong
static void printNoRecommendation(const char *username) {
    printf("\n");
    printSectionHeader("", "RECOMMENDATIONS");
    printf("\n%s %sUser:%s %s@%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, username, RESET);
    printSectionDivider();
    printf("\n%s %s(No recommendations available)%s\n", BOX_V, DIM, RESET);
    printSectionDivider();
    printf("\n");
    printInfo("Follow more users to get recommendations");
}


// Implementasi utama

void commandFriendRecommendation() {
    clearScreen();
    printBreadcrumb("Home > Friend Recommendation");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                     %sFRIEND RECOMMENDATION%s                        %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    // Jika belum login, bersihkan kata dan beri pesan error
    if (!isLoggedIn()) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Authentication required");
        printf("You must be logged in to view recommendations.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    // Validasi untuk kasus keberadaan argumen setelah FRIEND_RECOMMENDATION
    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("This command takes no arguments.\n\n");
        printf("%sUsage:%s %sFRIEND_RECOMMENDATION;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Analyzing your network", 10);

    int n = USER_COUNT;
    int src = CURRENT_USER_INDEX;

    if (n <= 1 || src < 0 || src >= n || !isValidVertex(SOCIAL_GRAPH, src)) {
        char uname[256];
        wordToString(uname, USERS[src].username);
        printNoRecommendation(uname);
        return;
    }

    int *depth = (int *) malloc(n * sizeof(int));
    boolean *visited = (boolean *) malloc(n * sizeof(boolean));
    boolean *followed = (boolean *) malloc(n * sizeof(boolean));
    int *mutual = (int *) malloc(n * sizeof(int));
    int *queue = (int *) malloc(n * sizeof(int));

    if (!depth || !visited || !followed || !mutual || !queue) {
        printf("Terjadi kesalahan memori saat menyiapkan rekomendasi.\n");
        free(depth); free(visited); free(followed); free(mutual); free(queue);
        return;
    }

    for (int i = 0; i < n; i++) {
        depth[i]   = -1;
        visited[i] = false;
        mutual[i]  = 0;
    }

    buildFollowedArray(followed, n, src);

    // BFS Algorithm
    int head = 0, tail = 0;
    queue[tail++] = src;
    visited[src] = true;
    depth[src] = 0;

    while (head < tail) {
        int u = queue[head++];

        if (depth[u] >= MAX_REC_DEPTH) {
            continue;
        }

        if (!isValidVertex(SOCIAL_GRAPH, u)) continue;

        AdjAddr p = SOCIAL_GRAPH.adj[u];
        while (p != NULL) {
            int v = p->v;
            if (v >= 0 && v < n && !visited[v]) {
                visited[v] = true;
                depth[v] = depth[u] + 1;
                queue[tail++] = v;
            }
            p = p->next;
        }
    }

    // Hitung mutual untuk kandidat dengan depth = 2 sampai MAX_REC_DEPTH yang belum di-follow current user
    int candidateCount = 0;
    for (int v = 0; v < n; v++) {
        if (v == src) continue;
        if (depth[v] < 2 || depth[v] > MAX_REC_DEPTH) continue;
        if (followed[v]) continue;

        if (!isValidVertex(SOCIAL_GRAPH, v)) continue;

        int count = 0;
        AdjAddr p = SOCIAL_GRAPH.adj[v];
        while (p != NULL) {
            int w = p->v;
            if (w >= 0 && w < n && followed[w]) {
                count++;
            }
            p = p->next;
        }

        mutual[v] = count;
        if (count > 0) {
            candidateCount++;
        }
    }

    char currName[256];
    wordToString(currName, USERS[src].username);

    if (candidateCount == 0) {
        printNoRecommendation(currName);
        free(depth); free(visited); free(followed); free(mutual); free(queue);
        return;
    }

    // Bangun max-heap berdasarkan mutual connections
    Heap H;
    createHeap(&H, candidateCount, true);

    for (int v = 0; v < n; v++) {
        if (mutual[v] > 0 &&
            v != src &&
            depth[v] >= 2 &&
            depth[v] <= MAX_REC_DEPTH &&
            !followed[v]) {
                HeapElType el;
                el.key = (long) mutual[v];
                el.dataIdx = v;
                heapPush(&H, el);
            }
    }

    printf("\n");
    printSectionHeader("", "TOP RECOMMENDATIONS");
    printf("\n%s %sBased on mutual connections%s\n", BOX_V, DIM, RESET);
    printSectionDivider();
    printf("\n");

    int rank = 1;
    while (rank <= MAX_REC_RESULTS && !isHeapEmpty(H)) {
        HeapElType top;
        if (!heapPop(&H, &top)) break;

        int v = top.dataIdx;
        int m = (int) top.key;

        if (v < 0 || v >= n) continue;

        char uname[256];
        wordToString(uname, USERS[v].username);

        printf("%s %s%d.%s %s@%s%s %s(%d mutual)%s\n", 
               BOX_V, BOLD_WHITE, rank, RESET, BOLD_CYAN, uname, RESET, DIM, m, RESET);
        rank++;
    }
    printSectionDivider();
    printf("\n");
    printInfo("Recommendations based on BFS graph traversal");

    deallocateHeap(&H);
    free(depth); free(visited); free(followed); free(mutual); free(queue);
}
