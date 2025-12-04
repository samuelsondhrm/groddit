#include <stdio.h>
#include <stdlib.h>

#include "AdvancedSearch.h"
#include "../../adt/Trie/Trie.h"
#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../../adt/ListBerkait/ListBerkait.h"

static void getSubgrodditNameById(Word subId, char *out) {
    out[0] = '\0';
    Node *p = SUBGRODDITS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_SUBGRODDIT) {
            SubGroddit *s = &p->element.data.subgroddit;
            if (compareWord(s->subgroddit_id, subId)) {
                wordToString(out, s->name);
                return;
            }
        }
        p = p->next;
    }
}

void commandSearchUser() {
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printf("Format perintah SEARCH_USER salah. Gunakan 'SEARCH_USER <prefix>;'\n");
        return;
    }

    Word prefixWord = currentWord;

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Format perintah SEARCH_USER salah. Gunakan 'SEARCH_USER <prefix>;'\n");
        return;
    }

    Trie T;
    createTrie(&T);

    for (int i = 0; i < USER_COUNT; i++) {
        trieInsertWordLower(&T, USERS[i].username, i);
    }

    int outCount = 0;
    int *indices = trieSearchPrefixWordLower(&T, prefixWord, &outCount);

    char prefixStr[NMax + 1];
    wordToString(prefixStr, prefixWord);

    printf("Menampilkan hasil pencarian user dengan prefix \"%s\"\n", prefixStr);

    if (indices == NULL || outCount == 0) {
        printf("(Tidak ada hasil)\n");
        freeTrie(&T);
        if (indices != NULL)
            free(indices);
        return;
    }

    for (int i = 0; i < outCount; i++) {
        int idx = indices[i];
        if (idx < 0 || idx >= USER_COUNT)
            continue;

        char username[256];
        wordToString(username, USERS[idx].username);
        printf("%d. %s\n", i + 1, username);
    }

    freeTrie(&T);
    free(indices);
}

void commandSearchPost() {
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printf("Format perintah SEARCH_POST salah. Gunakan 'SEARCH_POST <prefix>;'\n");
        return;
    }

    Word prefixWord = currentWord;

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Format perintah SEARCH_POST salah. Gunakan 'SEARCH_POST <prefix>;'\n");
        return;
    }

    Trie T;
    createTrie(&T);

    int arrCap = 0;
    int postCount = 0;
    Post **postArr = NULL;

    Node *p = POSTS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_POST) {
            if (!ensureCapacity((void **)&postArr, &arrCap, sizeof(Post *), postCount + 1)) {
                freeTrie(&T);
                free(postArr);
                printf("Gagal alokasi memori untuk Advanced Search post.\n");
                return;
            }

            Post *post = &p->element.data.post;
            postArr[postCount] = post;
            trieInsertWordLower(&T, post->title, postCount);
            postCount++;
        }
        p = p->next;
    }

    int outCount = 0;
    int *indices = trieSearchPrefixWordLower(&T, prefixWord, &outCount);

    char prefixStr[NMax + 1];
    wordToString(prefixStr, prefixWord);

    printf("Menampilkan hasil pencarian post dengan prefix \"%s\"\n", prefixStr);

    if (indices == NULL || outCount == 0) {
        printf("(Tidak ada hasil)\n");
        freeTrie(&T);
        free(postArr);
        if (indices != NULL)
            free(indices);
        return;
    }

    for (int i = 0; i < outCount; i++) {
        int idx = indices[i];
        if (idx < 0 || idx >= postCount)
            continue;

        Post *post = postArr[idx];

        char subName[256];
        getSubgrodditNameById(post->subgroddit_id, subName);
        char title[256];
        wordToString(title, post->title);

        printf("%d. [%s] %s\n", i + 1, subName, title);
    }

    freeTrie(&T);
    free(postArr);
    free(indices);
}

void commandSearchSubgroddit() {
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printf("Format perintah SEARCH_SUBGRODDIT salah. Gunakan 'SEARCH_SUBGRODDIT <prefix>;'\n");
        return;
    }

    Word prefixWord = currentWord;

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Format perintah SEARCH_SUBGRODDIT salah. Gunakan 'SEARCH_SUBGRODDIT <prefix>;'\n");
        return;
    }

    Trie T;
    createTrie(&T);

    int arrCap = 0;
    int subCount = 0;
    SubGroddit **subArr = NULL;

    Node *p = SUBGRODDITS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_SUBGRODDIT) {
            if (!ensureCapacity((void **)&subArr, &arrCap, sizeof(SubGroddit *), subCount + 1)) {
                freeTrie(&T);
                free(subArr);
                printf("Gagal alokasi memori untuk Advanced Search subgroddit.\n");
                return;
            }

            SubGroddit *sub = &p->element.data.subgroddit;
            subArr[subCount] = sub;
            trieInsertWordLower(&T, sub->name, subCount);
            subCount++;
        }
        p = p->next;
    }

    int outCount = 0;
    int *indices = trieSearchPrefixWordLower(&T, prefixWord, &outCount);

    char prefixStr[NMax + 1];
    wordToString(prefixStr, prefixWord);

    printf("Menampilkan hasil pencarian subgroddit untuk prefix \"%s\"\n", prefixStr);

    if (indices == NULL || outCount == 0) {
        printf("(Tidak ada hasil)\n");
        freeTrie(&T);
        free(subArr);
        if (indices != NULL)
            free(indices);
        return;
    }

    for (int i = 0; i < outCount; i++) {
        int idx = indices[i];
        if (idx < 0 || idx >= subCount)
            continue;

        SubGroddit *sub = subArr[idx];

        char name[256];
        wordToString(name, sub->name);
        printf("%d. %s\n", i + 1, name);
    }

    freeTrie(&T);
    free(subArr);
    free(indices);
}
