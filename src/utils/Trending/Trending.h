#ifndef TRENDING_H
#define TRENDING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "../../adt/MesinKata/MesinKata.h"
#include "../../adt/Heap/Heap.h"
#include "../../adt/ListBerkait/ListBerkait.h"
#include "../GlobalData/GlobalData.h"
#include "../Subgroddit/Subgroddit.h"
#include "../Helper/Helper.h"
#include "../Kreativitas/Kreativitas.h"

// struktur buat simpan keyword dan freq
typedef struct {
    char keyword[NMax];
    int frequency;
    char hottestPostTitle[NMax];
} KeywordFreq;

/**
 * command handler TRENDING
 * e.g.
 *      TRENDING r/programming 24 hour
 *      TRENDING r/programming 10 minute
 */
void commandTrending();

/**
 * helper buat normalize kata (to lowercase dll)
 * @param word  kata yg akan dinormalize
 * @param out   buffer output
 * @return      true jika kata valid (panjang >=3), false jika tidak
 */
boolean normalizeKeyword(const char *word, char *out);

/**
 * ekstrak semua kata dari teks dan calc freq
 * @param text          teks yg akan diproses
 * @param keyWordArray  array buat simpan keyword
 * @param keywordCOunt  jumlah keyword yg sudah ada
 * @param capacity      kapasitas array
 * @param postTitle     title post (buat tracking hottest topc)
 */
void extractKeywords(const char *text, KeywordFreq **keywordArray, int *keywordCount, int *capacity, const char *postTitle);

/**
 * display trending analysis result, beta ver, nanti aku atur lagi di kreativitas
 * @param subgrodditName    nama subgroddit
 * @param timeValue         
 * @param timeUnit          unit waktu hour/minute/second
 * @param topKeywords       array top keywords
 * @param topCount          jumlah top keywords
 */
void displayTrendingResults(const char *subgrodditName, int timeValue, const char *timeUnit, KeywordFreq *topKeywords, int topCount);

#endif