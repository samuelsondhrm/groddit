#include "Trie.h"
#include <stdlib.h>
#include <ctype.h>

/* ============================================================================
 *                        Helper Internal: Node Trie
 * ==========================================================================*/

#define FIRST_CHAR 32
#define ALPHABET_SIZE 95   
TrieNode *newTrieNode() {
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    if (!node)
        return NULL;

    node->isEnd = false;
    node->index = -1;
    node->next = (TrieNode **) malloc(ALPHABET_SIZE * sizeof(TrieNode *));
    if (!node->next) {
        free(node);
        return NULL;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->next[i] = NULL;
    }

    return node;
}

// Dealokasi rekursif seluruh subtree TrieNode.
void freeTrieNode(TrieNode *node) {
    if (!node)
        return;

    if (node->next) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->next[i]) {
                freeTrieNode(node->next[i]);
            }
        }
        free(node->next);
    }

    free(node);
}

/* ============================================================================
 *                        Konstruktor / Destruktor Trie
 * ==========================================================================*/

void createTrie(Trie *T) {
    T->root = newTrieNode();
}

void freeTrie(Trie *T) {
    if (T->root) {
        freeTrieNode(T->root);
        T->root = NULL;
    }
}

/* ============================================================================
 *                          Utilitas Konversi Karakter
 * ==========================================================================*/

/* Mengubah karakter menjadi index 0..ALPHABET_SIZE-1, atau -1 jika di luar
 * rentang yang didukung (ASCII 32..126). */
int charToIndex(char c) {
    unsigned char uc = (unsigned char)c;
    if (uc < FIRST_CHAR || uc >= FIRST_CHAR + ALPHABET_SIZE)
        return -1;
    return (int)(uc - FIRST_CHAR);
}

/* ============================================================================
 *                              Operasi Utama
 * ==========================================================================*/

void trieInsert(Trie *T, const char *key, int index) {
    if (!T->root) {
        createTrie(T);
    }

    TrieNode *p = T->root;
    for (int i = 0; key[i] != '\0'; i++) {
        int ci = charToIndex(key[i]);
        if (ci < 0) {
            // Skip karakter di luar range
            continue;
        }

        if (!p->next[ci]) {
            p->next[ci] = newTrieNode();
            if (!p->next[ci]) {
                // Gagal malloc, hentikan penyisipan lebih lanjut
                return;
            }
        }

        p = p->next[ci];
    }

    p->isEnd = true;
    p->index = index;
}

/* Mengumpulkan seluruh index dari subtree yang berakar di node.
 * Hasil disimpan ke buffer dinamis (dengan kemungkinan realloc). */
void collectAllFrom(TrieNode *node, int **buffer, int *count, int *capacity) {
    if (!node)
        return;

    if (node->isEnd && node->index >= 0) {
        if (*count >= *capacity) {
            int newCap = (*capacity == 0 ? 8 : (*capacity * 2));
            int *newBuf = (int *) realloc(*buffer, newCap * sizeof(int));
            if (!newBuf) {
                // Jika gagal realloc, hentikan penambahan lebih lanjut
                return;
            }
            *buffer = newBuf;
            *capacity = newCap;
        }

        (*buffer)[*count] = node->index;
        (*count)++;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->next[i]) {
            collectAllFrom(node->next[i], buffer, count, capacity);
        }
    }
}

int *trieSearchPrefix(const Trie *T, const char *prefix, int *outCount) {
    *outCount = 0;
    if (!T->root)
        return NULL;

    TrieNode *p = T->root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int ci = charToIndex(prefix[i]);
        if (ci < 0) {
            // Skip karakter di luar range
            continue;
        }

        if (!p->next[ci]) {
            // Prefix tidak ditemukan
            return NULL;
        }

        p = p->next[ci];
    }

    int capacity = 8;
    int *result = (int *) malloc(capacity * sizeof(int));
    if (!result)
        return NULL;

    int count = 0;
    collectAllFrom(p, &result, &count, &capacity);

    *outCount = count;
    return result;
}

/* ============================================================================
 *                             Helper Berbasis Word
 * ==========================================================================*/

void wordToLowercaseString(char *out, Word w) {
    for (int i = 0; i < w.Length; i++) {
        unsigned char c = (unsigned char) w.TabWord[i];
        out[i] = (char) tolower(c);
    }
    out[w.Length] = '\0';
}

void trieInsertWordLower(Trie *T, Word w, int index) {
    char buf[NMax + 1];
    wordToLowercaseString(buf, w);
    trieInsert(T, buf, index);
}

int *trieSearchPrefixWordLower(const Trie *T, Word prefixWord, int *outCount) {
    char buf[NMax + 1];
    wordToLowercaseString(buf, prefixWord);
    return trieSearchPrefix(T, buf, outCount);
}
