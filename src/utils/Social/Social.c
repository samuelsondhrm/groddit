// #include "header/Social.h"

// /* Helper internal: ambil index user dari username dengan pesan error standar */
// static int getUserIndexOrReport(const char *username) {
//     int idx = findUserIndexByUsername(username);
//     if (idx == IDX_UNDEF) {
//         printf("User dengan username \"%s\" tidak ditemukan.\n", username);
//     }
//     return idx;
// }

// /* ============================================================================
//  *                          Fitur FOLLOW / UNFOLLOW
//  * ============================================================================
//  */

// void socialFollowUser(const char *targetUsername) {
//     if (!isLoggedIn()) {
//         printf("Anda belum login. Silakan login terlebih dahulu.\n");
//         return;
//     }

//     int src = CURRENT_USER_INDEX;
//     int dst = getUserIndexOrReport(targetUsername);
//     if (dst == IDX_UNDEF) {
//         return;
//     }

//     if (src == dst) {
//         printf("Anda tidak dapat mengikuti diri sendiri.\n");
//         return;
//     }

//     if (!isValidVertex(SOCIAL_GRAPH, src) || !isValidVertex(SOCIAL_GRAPH, dst)) {
//         printf("Terjadi kesalahan pada data sosial.\n");
//         return;
//     }

//     if (isAdjacent(SOCIAL_GRAPH, src, dst)) {
//         printf("Anda sudah mengikuti %s.\n", getUsernameByIndex(dst));
//         return;
//     }

//     addEdge(&SOCIAL_GRAPH, src, dst);
//     printf("Berhasil mengikuti %s.\n", getUsernameByIndex(dst));
// }

// void socialUnfollowUser(const char *targetUsername) {
//     if (!isLoggedIn()) {
//         printf("Anda belum login. Silakan login terlebih dahulu.\n");
//         return;
//     }

//     int src = CURRENT_USER_INDEX;
//     int dst = getUserIndexOrReport(targetUsername);
//     if (dst == IDX_UNDEF) {
//         return;
//     }

//     if (!isValidVertex(SOCIAL_GRAPH, src) || !isValidVertex(SOCIAL_GRAPH, dst)) {
//         printf("Terjadi kesalahan pada data sosial.\n");
//         return;
//     }

//     if (!isAdjacent(SOCIAL_GRAPH, src, dst)) {
//         printf("Anda belum mengikuti %s.\n", getUsernameByIndex(dst));
//         return;
//     }

//     removeEdge(&SOCIAL_GRAPH, src, dst);
//     printf("Berhasil berhenti mengikuti %s.\n", getUsernameByIndex(dst));
// }

// /* ============================================================================
//  *                          Fitur FOLLOWING / FOLLOWERS
//  * ============================================================================
//  */

// void socialShowFollowing(const char *username) {
//     int idx = getUserIndexOrReport(username);
//     if (idx == IDX_UNDEF) {
//         return;
//     }

//     if (!isValidVertex(SOCIAL_GRAPH, idx)) {
//         printf("Terjadi kesalahan pada data sosial.\n");
//         return;
//     }

//     printf("Daftar akun yang diikuti oleh %s:\n", getUsernameByIndex(idx));

//     AdjAddr p = SOCIAL_GRAPH.adj[idx];
//     if (p == NULL) {
//         printf("(Tidak mengikuti siapa pun)\n");
//         return;
//     }

//     while (p != NULL) {
//         int v = p->v;
//         if (isValidVertex(SOCIAL_GRAPH, v)) {
//             printf("- %s\n", getUsernameByIndex(v));
//         }
//         p = p->next;
//     }
// }

// void socialShowFollowers(const char *username) {
//     int target = getUserIndexOrReport(username);
//     if (target == IDX_UNDEF) {
//         return;
//     }

//     if (!isValidVertex(SOCIAL_GRAPH, target)) {
//         printf("Terjadi kesalahan pada data sosial.\n");
//         return;
//     }

//     printf("Daftar akun yang mengikuti %s:\n", getUsernameByIndex(target));

//     boolean ada = false;
//     for (int u = 0; u < SOCIAL_GRAPH.nVertex; u++) {
//         AdjAddr p = SOCIAL_GRAPH.adj[u];
//         while (p != NULL) {
//             if (p->v == target) {
//                 printf("- %s\n", getUsernameByIndex(u));
//                 ada = true;
//                 break;
//             }
//             p = p->next;
//         }
//     }

//     if (!ada) {
//         printf("(Belum ada yang mengikuti akun ini)\n");
//     }
// }
