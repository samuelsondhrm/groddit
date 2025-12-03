#include "Trending.h"

static const char *STOPWORDS[] = {
    "the", "and", "for", "are", "but", "not", "you", "all", "can", "her",
    "was", "one", "our", "out", "day", "get", "has", "him", "his", "how",
    "man", "new", "now", "old", "see", "two", "way", "who", "boy", "did",
    "its", "let", "put", "say", "she", "too", "use", "this", "that", "with",
    "have", "from", "they", "will", "what", "your", "more", "when", "into",
    "than", "them", "been", "their", "would", "about", "could", "other",

    "dan", "atau", "yang", "dari", "dengan", "pada", "adalah", "ini", "itu",
    "ada", "oleh", "akan", "untuk", "juga", "telah", "sudah", "dapat", "sebagai",
    "dalam", "satu", "dua", "kami", "kita", "anda", "mereka", "dia", "saya",
    "kamu", "nya", "apa", "siapa", "dimana", "kapan", "kenapa", "bagaimana",
    "bila", "jika", "maka", "karena", "oleh", "sampai", "hingga", "antara",
    "tanpa", "bagi", "para", "tiap", "sekitar", "hanya", "masih", "pernah",
    "paling", "sangat", "lebih", "sama", "bisa", "mau", "baru", "lain"
};
static const int STOPWORDS_COUNT = 112;

boolean isStopword(const char *word) {
    for (int i = 0; i < STOPWORDS_COUNT; i++) {
        if (strCmp(word, STOPWORDS[i]) == 0) {
            return true;
        }
    }
    return false;
}

boolean normalizeKeyword(const char *word, char *out) {
    int j = 0;
    for (int i = 0; word[i] != '\0' && j < NMax - 1; i++) {
        if (isalpha(word[i])) {
            out[j++] = tolower(word[i]);
        }
    }
    out[j] = '\0';
    
    if (j < 3 || isStopword(out)) {
        return false;
    }
    return true;
}

void extractKeywords(const char *text, KeywordFreq **keywordArray, 
                     int *keywordCount, int *capacity, const char *postTitle) {
    char buffer[NMax];
    int bufIdx = 0;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i]) || isdigit(text[i])) {
            if (bufIdx < NMax - 1) {
                buffer[bufIdx++] = text[i];
            }
        } else {
            if (bufIdx > 0) {
                buffer[bufIdx] = '\0';
                
                char normalized[NMax];
                if (normalizeKeyword(buffer, normalized)) {
                    int found = -1;
                    for (int j = 0; j < *keywordCount; j++) {
                        if (strCmp((*keywordArray)[j].keyword, normalized) == 0) {
                            found = j;
                            break;
                        }
                    }
                    
                    if (found != -1) {
                        (*keywordArray)[found].frequency++;
                    } else {
                        if (*keywordCount >= *capacity) {
                            *capacity = (*capacity == 0) ? 10 : (*capacity * 2);
                            *keywordArray = (KeywordFreq *)realloc(*keywordArray, 
                                                                   (*capacity) * sizeof(KeywordFreq));
                        }
                        
                        strcpy((*keywordArray)[*keywordCount].keyword, normalized);
                        (*keywordArray)[*keywordCount].frequency = 1;
                        strcpy((*keywordArray)[*keywordCount].hottestPostTitle, postTitle);
                        (*keywordCount)++;
                    }
                }
                
                bufIdx = 0;
            }
        }
    }
    
    if (bufIdx > 0) {
        buffer[bufIdx] = '\0';
        char normalized[NMax];
        if (normalizeKeyword(buffer, normalized)) {
            int found = -1;
            for (int j = 0; j < *keywordCount; j++) {
                if (strCmp((*keywordArray)[j].keyword, normalized) == 0) {
                    found = j;
                    break;
                }
            }
            
            if (found != -1) {
                (*keywordArray)[found].frequency++;
            } else {
                if (*keywordCount >= *capacity) {
                    *capacity = (*capacity == 0) ? 10 : (*capacity * 2);
                    *keywordArray = (KeywordFreq *)realloc(*keywordArray, 
                                                           (*capacity) * sizeof(KeywordFreq));
                }
                
                strcpy((*keywordArray)[*keywordCount].keyword, normalized);
                (*keywordArray)[*keywordCount].frequency = 1;
                strcpy((*keywordArray)[*keywordCount].hottestPostTitle, postTitle);
                (*keywordCount)++;
            }
        }
    }
}

void displayTrendingResults(const char *subgrodditName, int timeValue, 
                           const char *timeUnit, KeywordFreq *topKeywords, 
                           int topCount) {
    printf("\n");
    printf("TRENDING IN %s (Last %d %s)", subgrodditName, timeValue, timeUnit);
    
    int headerLen = 26 + strlen(subgrodditName) + 
                    (timeValue >= 10 ? 2 : 1) + strlen(timeUnit);
    for (int i = headerLen; i < 63; i++) {
        printf(" ");
    }
    printf("\n");
    
    if (topCount == 0) {
        printf("Tidak ada data trending untuk periode ini.\n");
        return;
    }
    
    for (int i = 0; i < topCount && i < 5; i++) {
        int pad = 15-(int)strlen(topKeywords[i].keyword);
        if (pad<1) pad = 1;
        printf("%d. \"%s\" %*s- %d mentions\n",
            i + 1,
            topKeywords[i].keyword,
            pad,
            "",
            topKeywords[i].frequency);

    }
    
    if (topCount > 0) {
        printf("Hottest topic: \"%s\" in \"%s\"\n", 
               topKeywords[0].keyword, topKeywords[0].hottestPostTitle);
    }
}

void commandTrending() {
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printf("Format perintah TRENDING salah. Gunakan 'TRENDING <subgroddit> <waktu> <hour/minute/second>;' atau 'TRENDING <subgroddit> ALL;'.\n");
        return;
    }
    
    char subgrodditName[256];
    wordToString(subgrodditName, currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printf("Format perintah TRENDING salah. Gunakan 'TRENDING <subgroddit> <waktu> <hour/minute/second>;' atau 'TRENDING <subgroddit> ALL;'.\n");
        return;
    }
    
    char timeOrAll[256];
    wordToString(timeOrAll, currentWord);
    
    boolean useAllTime = false;
    int timeValue = 0;
    char timeUnit[256] = "";
    
    if (strCmp(timeOrAll, "ALL") == 0) {
        useAllTime = true;
        
        ADVWORD_INPUT();
        if (currentWord.Length != 0) {
            while (currentWord.Length != 0) {
                ADVWORD_INPUT();
            }
            printf("Format perintah TRENDING salah. Gunakan 'TRENDING <subgroddit> ALL;'.\n");
            return;
        }
    } else {
        timeValue = wordToInt(currentWord);
        if (timeValue <= 0) {
            printf("Nilai waktu harus berupa angka positif.\n");
            return;
        }
        
        ADVWORD_INPUT();
        if (currentWord.Length == 0) {
            printf("Format perintah TRENDING salah. Gunakan 'TRENDING <subgroddit> <waktu> <hour/minute/second>;'.\n");
            return;
        }
        
        wordToString(timeUnit, currentWord);
        
        ADVWORD_INPUT();
        if (currentWord.Length != 0) {
            while (currentWord.Length != 0) {
                ADVWORD_INPUT();
            }
            printf("Format perintah TRENDING salah. Gunakan 'TRENDING <subgroddit> <waktu> <hour/minute/second>;'.\n");
            return;
        }
        
        if (strCmp(timeUnit, "hour") != 0 && 
            strCmp(timeUnit, "minute") != 0 && 
            strCmp(timeUnit, "second") != 0) {
            printf("Unit waktu tidak valid! Gunakan hour, minute, atau second.\n");
            return;
        }
    }
    
    int subIdx = findSubgrodditIndexByName(subgrodditName);
    if (subIdx == -1) {
        printf("Subgroddit %s belum ditemukan!\n", subgrodditName);
        printf("Gunakan perintah CREATE_SUBGRODDIT untuk membuatnya terlebih dahulu.\n");
        return;
    }
    
    Node *subNode = SUBGRODDITS.head;
    int idx = 0;
    while (subNode != NULL && idx < subIdx) {
        subNode = subNode->next;
        idx++;
    }
    if (subNode == NULL || subNode->element.type != TYPE_SUBGRODDIT) {
        printf("Error: Subgroddit tidak ditemukan.\n");
        return;
    }
    SubGroddit *sub = &subNode->element.data.subgroddit;
    
    time_t threshold = 0;
    
    if (!useAllTime) {
        time_t now = time(NULL);
        threshold = now;
        
        if (strCmp(timeUnit, "hour") == 0) {
            threshold -= (timeValue * 3600);
        } else if (strCmp(timeUnit, "minute") == 0) {
            threshold -= (timeValue * 60);
        } else if (strCmp(timeUnit, "second") == 0) {
            threshold -= timeValue;
        }
    }
    
    KeywordFreq *keywordArray = NULL;
    int keywordCount = 0;
    int capacity = 0;
    
    Node *p = POSTS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_POST) {
            Post *post = &p->element.data.post;
            
            boolean inSubgroddit = (compareWord(post->subgroddit_id, sub->subgroddit_id) == 1);
            boolean inTimeRange = useAllTime || (post->created_at >= threshold);
            
            if (inSubgroddit && inTimeRange) {
                char titleStr[NMax];
                char contentStr[NMax];
                wordToString(titleStr, post->title);
                wordToString(contentStr, post->content);

                extractKeywords(titleStr, &keywordArray, &keywordCount, &capacity, titleStr);
                extractKeywords(contentStr, &keywordArray, &keywordCount, &capacity, titleStr);
                
                char postIdStr[NMax];
                wordToString(postIdStr, post->post_id);
                
                for (int i = 0; i < COMMENT_COUNT; i++) {
                    char commentPostId[NMax];
                    wordToString(commentPostId, COMMENTS[i].post_id);
                    
                    if (strCmp(commentPostId, postIdStr) == 0) {
                        char commentContent[NMax];
                        wordToString(commentContent, COMMENTS[i].content);
                        extractKeywords(commentContent, &keywordArray, &keywordCount, 
                                      &capacity, titleStr);
                    }
                }
            }
        }
        p = p->next;
    }
    
    if (keywordCount == 0) {
        printf("\n");
        if (useAllTime) {
            printf("TRENDING IN %s (All Time)", subgrodditName);
            int headerLen = 28 + strlen(subgrodditName);
            for (int i = headerLen; i < 63; i++) {
                printf(" ");
            }
        } else {
            printf("TRENDING IN %s (Last %d %s)", subgrodditName, timeValue, timeUnit);
            int headerLen = 26 + strlen(subgrodditName) + 
                            (timeValue >= 10 ? 2 : 1) + strlen(timeUnit);
            for (int i = headerLen; i < 63; i++) {
                printf(" ");
            }
        }
        printf("\n");
        printf("Tidak ada data trending untuk periode ini.\n");
        if (keywordArray != NULL) {
            free(keywordArray);
        }
        return;
    }
    
    Heap maxHeap;
    createHeap(&maxHeap, keywordCount, true);
    
    for (int i = 0; i < keywordCount; i++) {
        HeapElType elem;
        elem.key = keywordArray[i].frequency;
        elem.dataIdx = i;
        heapPush(&maxHeap, elem);
    }
    
    KeywordFreq topKeywords[5];
    int topCount = 0;
    
    while (!isHeapEmpty(maxHeap) && topCount < 5) {
        HeapElType elem;
        heapPop(&maxHeap, &elem);
        topKeywords[topCount] = keywordArray[elem.dataIdx];
        topCount++;
    }
    
    if (useAllTime) {
        printf("TRENDING IN %s (All Time)", subgrodditName);
        int headerLen = 28 + strlen(subgrodditName);
        for (int i = headerLen; i < 63; i++) {
            printf(" ");
        }
        printf("\n");
        for (int i = 0; i < topCount && i < 5; i++) {
            int pad = 15-(int)strlen(topKeywords[i].keyword);
            if (pad<1) pad = 1;
            printf("%d. \"%s\" %*s- %d mentions\n",
                i+1,
                topKeywords[i].keyword,
                pad,
                "",
                topKeywords[i].frequency);
        }
        if (topCount > 0) {
            printf("Hottest topic: \"%s\" in \"%s\"\n", 
                   topKeywords[0].keyword, topKeywords[0].hottestPostTitle);
        }
    } else {
        displayTrendingResults(subgrodditName, timeValue, timeUnit, topKeywords, topCount);
    }
    
    deallocateHeap(&maxHeap);
    if (keywordArray != NULL) {
        free(keywordArray);
    }
}