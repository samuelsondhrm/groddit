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
    if (!isLoggedIn()) {
        printf("Anda belum login. Silakan login terlebih dahulu.\n");
        return;
    }

    int src = CURRENT_USER_INDEX;
    int dst = getUserIndexOrReport(targetUsername);
    if (dst == IDX_UNDEF) {
        return;
    }

    if (src == dst) {
        printf("Anda tidak dapat mengikuti diri sendiri.\n");
        return;
    }

    if (!isValidVertex(SOCIAL_GRAPH, src) || !isValidVertex(SOCIAL_GRAPH, dst)) {
        printf("Terjadi kesalahan pada data sosial.\n");
        return;
    }

    if (isAdjacent(SOCIAL_GRAPH, src, dst)) {
        printf("Anda sudah mengikuti %s.\n", findUsernameById(dst));
        return;
    }

    /* 1) update graph */
    addEdge(&SOCIAL_GRAPH, src, dst);

    /* 2) update buffer SOCIALS */
    addSocialBuffer(src, dst);

    printf("Berhasil mengikuti %s.\n", findUsernameById(dst));
}

void socialUnfollowUser(const char *targetUsername) {
    if (!isLoggedIn()) {
        printf("Anda belum login. Silakan login terlebih dahulu.\n");
        return;
    }

    int src = CURRENT_USER_INDEX;
    int dst = getUserIndexOrReport(targetUsername);
    if (dst == IDX_UNDEF) {
        return;
    }

    if (!isValidVertex(SOCIAL_GRAPH, src) || !isValidVertex(SOCIAL_GRAPH, dst)) {
        printf("Terjadi kesalahan pada data sosial.\n");
        return;
    }

    if (!isAdjacent(SOCIAL_GRAPH, src, dst)) {
        printf("Anda belum mengikuti %s.\n", findUsernameById(dst));
        return;
    }

    /* 1) update graph */
    removeEdge(&SOCIAL_GRAPH, src, dst);

    /* 2) update buffer SOCIALS */
    removeSocialBuffer(src, dst);

    printf("Berhasil berhenti mengikuti %s.\n", findUsernameById(dst));
}


/* ============================================================================
 *                          Fitur FOLLOWING / FOLLOWERS
 * ============================================================================
 */

void socialShowFollowing(const char *username) {
    int idx;

    // Jika username NULL → gunakan current user
    if (username == NULL || username[0] == '\0') {
        if (!isLoggedIn()) {
            printf("Anda belum login. Silakan login terlebih dahulu.\n");
            return;
        }
        idx = CURRENT_USER_INDEX;
    } else {
        idx = getUserIndexOrReport(username);
        if (idx == IDX_UNDEF) return;
    }

    if (!isValidVertex(SOCIAL_GRAPH, idx)) {
        printf("Terjadi kesalahan pada data sosial.\n");
        return;
    }

    printf("Daftar akun yang diikuti oleh %s:\n", findUsernameById(idx));

    AdjAddr p = SOCIAL_GRAPH.adj[idx];
    if (p == NULL) {
        printf("(Tidak mengikuti siapa pun)\n");
        return;
    }

    while (p != NULL) {
        printf("- %s\n", findUsernameById(p->v));
        p = p->next;
    }
}


void socialShowFollowers(const char *username) {
    int target;

    if (username == NULL || username[0] == '\0') {
        if (!isLoggedIn()) {
            printf("Anda belum login. Silakan login terlebih dahulu.\n");
            return;
        }
        target = CURRENT_USER_INDEX;
    } else {
        target = getUserIndexOrReport(username);
        if (target == IDX_UNDEF) return;
    }

    if (!isValidVertex(SOCIAL_GRAPH, target)) {
        printf("Terjadi kesalahan pada data sosial.\n");
        return;
    }

    printf("Daftar akun yang mengikuti %s:\n", findUsernameById(target));

    boolean ada = false;
    for (int u = 0; u < SOCIAL_GRAPH.nVertex; u++) {
        AdjAddr p = SOCIAL_GRAPH.adj[u];
        while (p != NULL) {
            if (p->v == target) {
                printf("- %s\n", findUsernameById(u));
                ada = true;
                break;
            }
            p = p->next;
        }
    }

    if (!ada) {
        printf("(Belum ada yang mengikuti akun ini)\n");
    }
}
