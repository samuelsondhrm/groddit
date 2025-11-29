#ifndef FOLLREC_H
#define FOLLREC_H

#include <stdio.h>
#include <stdlib.h>

#include "../../adt/Boolean/Boolean.h"
#include "../../adt/Graph/Graph.h"
#include "../../adt/Heap/Heap.h"
#include "../../adt/MesinKata/MesinKata.h"

#include "../GlobalData/GlobalData.h"
#include "../User/User.h"
#include "../Helper/Helper.h"

/* ============================================================================
 *                    FRIEND / FOLLOWING RECOMMENDATION
 * ============================================================================
 *
 * Perintah:
 *   FRIEND_RECOMMENDATION;
 *
 * Syarat:
 *   - User harus sudah LOGIN.
 *
 * Logika:
 *   - Lakukan BFS pada SOCIAL_GRAPH dari CURRENT_USER_INDEX
 *     dengan kedalaman maksimum MAX_DEPTH = 3.
 *   - Kandidat rekomendasi adalah semua user v yang:
 *       * depth[v] ∈ {2, 3}
 *       * v bukan current user
 *       * v belum di-follow oleh current user
 *   - Untuk tiap kandidat v, hitung mutual connections sebagai:
 *       | Following(current) ∩ Following(v) |
 *   - Semua kandidat dengan mutual > 0 dimasukkan ke max-heap,
 *     diurutkan menurun berdasarkan mutual.
 *   - Tampilkan maksimal 10 rekomendasi teratas.
 */
void commandFriendRecommendation();

#endif /* FOLLREC_H */
