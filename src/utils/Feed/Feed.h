#ifndef FEED_H
#define FEED_H

#include <stdio.h>
#include <stdlib.h>

#include "../../adt/Boolean/Boolean.h"
#include "../../adt/Heap/Heap.h"
#include "../../adt/ListBerkait/ListBerkait.h"
#include "../../adt/MesinKata/MesinKata.h"

#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../User/User.h"

/**
 * Perintah utama fitur Feed:
 *
 * Sintaks:
 *   SHOW_FEED LATEST [LIMIT];
 *   SHOW_FEED NEWEST [LIMIT];
 *
 * - LATEST : post terbaru dulu (waktu menurun)
 * - NEWEST : post terlama dulu (waktu menaik)
 *
 * LIMIT opsional. Jika tidak diberikan, semua post yang memenuhi syarat ditampilkan.
 */
void commandShowFeed();

#endif /* FEED_H */
