#include <stdio.h>
#include <stdlib.h>

#include "AdvancedSearch.h"
#include "../../adt/Trie/Trie.h"
#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../Kreativitas/Kreativitas.h"
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
    clearScreen();
    printBreadcrumb("Home > Search User");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                          %sSEARCH USER%s                              %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printError("Invalid command format");
        printf("Prefix is required.\n\n");
        printf("%sUsage:%s %sSEARCH_USER <prefix>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        printf("%sExample:%s %sSEARCH_USER bob;%s\n", DIM, RESET, BOLD_WHITE, RESET);
        return;
    }

    Word prefixWord = currentWord;

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sSEARCH_USER <prefix>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Searching users", 8);

    Trie T;
    createTrie(&T);

    for (int i = 0; i < USER_COUNT; i++) {
        trieInsertWordLower(&T, USERS[i].username, i);
    }

    int outCount = 0;
    int *indices = trieSearchPrefixWordLower(&T, prefixWord, &outCount);

    char prefixStr[NMax + 1];
    wordToString(prefixStr, prefixWord);

    printf("\n");
    printSectionHeader("", "SEARCH RESULTS");
    printf("\n%s %sPrefix:%s %s\"%s\"%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, prefixStr, RESET);
    printSectionDivider();
    printf("\n");

    if (indices == NULL || outCount == 0) {
        printf("%s %s(No users found)%s\n", BOX_V, DIM, RESET);
        printSectionDivider();
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
        printf("%s %s%d.%s %s@%s%s\n", BOX_V, BOLD_WHITE, i + 1, RESET, BOLD_CYAN, username, RESET);
    }
    printSectionDivider();
    printf("\n");
    printInfo("Total results: ");
    printf("%s%d%s users\n", BOLD_WHITE, outCount, RESET);

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
    clearScreen();
    printBreadcrumb("Home > Search Subgroddit");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                       %sSEARCH SUBGRODDIT%s                          %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printError("Invalid command format");
        printf("Name prefix is required.\n\n");
        printf("%sUsage:%s %sSEARCH_SUBGRODDIT <prefix>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        printf("%sExample:%s %sSEARCH_SUBGRODDIT r/prog;%s\n", DIM, RESET, BOLD_WHITE, RESET);
        return;
    }

    Word prefixWord = currentWord;

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sSEARCH_SUBGRODDIT <prefix>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Searching subgroddits", 8);

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
                printf("\n");
                printError("Memory allocation failed");
                printf("Could not allocate memory for search.\n");
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

    printf("\n");
    printSectionHeader("", "SEARCH RESULTS");
    printf("\n%s %sName prefix:%s %s\"%s\"%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, prefixStr, RESET);
    printSectionDivider();
    printf("\n");

    if (indices == NULL || outCount == 0) {
        printf("%s %s(No subgroddits found)%s\n", BOX_V, DIM, RESET);
        printSectionDivider();
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
        printf("%s %s%d.%s %s%s%s\n", BOX_V, BOLD_WHITE, i + 1, RESET, BOLD_MAGENTA, name, RESET);
    }
    printSectionDivider();
    printf("\n");
    printInfo("Total results: ");
    printf("%s%d%s subgroddits\n", BOLD_WHITE, outCount, RESET);

    freeTrie(&T);
    free(subArr);
    free(indices);
}
