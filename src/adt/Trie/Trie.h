#ifndef TRIE_H
#define TRIE_H

#include "../Boolean/Boolean.h"
#include "../MesinKata/MesinKata.h"

/* ============================================================================
 *                                   TRIE
 * ============================================================================
 * ADT Trie (prefix tree) untuk menyimpan string (ASCII 32..126) beserta index
 * entitas (misalnya index User / Post / SubGroddit).
 *
 * Setiap node:
 *  - isEnd : menandai akhir sebuah key valid.
 *  - index : index entitas terkait (>= 0), atau -1 jika tidak menyimpan index.
 *  - next  : array pointer anak berukuran 95 (karakter 32..126).
 * ==========================================================================*/

typedef struct TrieNode {
    boolean isEnd;       /* penanda akhir key */
    int index;           /* index entitas, -1 jika tidak ada */
    struct TrieNode **next; /* array anak, size = 95 (char 32..126) */
} TrieNode;

typedef struct {
    TrieNode *root;      /* akar trie */
} Trie;

/* ============================================================================
 *                            PRIMITIVE OPERATIONS
 * ==========================================================================*/

/**
 * @brief Menginisialisasi sebuah Trie kosong.
 *
 * @param T Pointer ke Trie
 */
void createTrie(Trie *T);

/**
 * @brief Menghapus seluruh node dalam Trie dan mengosongkannya.
 *
 * @param T Pointer ke Trie
 */
void freeTrie(Trie *T);

/**
 * @brief Menyisipkan sebuah key C-string ke dalam Trie.
 *
 * @param T     Pointer ke Trie
 * @param key   C-string (null-terminated) yang akan dimasukkan
 * @param index Index entitas yang dikaitkan dengan key
 */
void trieInsert(Trie *T, const char *key, int index);

/**
 * @brief Mencari semua entitas yang memiliki prefix tertentu (C-string).
 *
 * @param T         Pointer ke Trie
 * @param prefix    C-string prefix yang dicari
 * @param outCount  Output: banyaknya index hasil
 *
 * @return Array dinamis berisi index (panjang = *outCount), atau NULL jika
 *         tidak ada hasil / gagal alokasi. Caller bertanggung jawab memanggil
 *         free() pada array hasil.
 */
int *trieSearchPrefix(const Trie *T, const char *prefix, int *outCount);

/* ============================================================================
 *                               WORD UTILITIES
 * ==========================================================================*/

/**
 * @brief Mengonversi Word ke C-string lowercase (null-terminated).
 *
 * @param out Buffer keluaran (ukuran minimal NMax+1)
 * @param w   Word sumber
 */
void wordToLowercaseString(char *out, Word w);

/**
 * @brief Menyisipkan sebuah Word ke Trie (secara lowercase, case-insensitive).
 *
 * @param T     Pointer ke Trie
 * @param w     Word yang akan dimasukkan (akan di-lowercase)
 * @param index Index entitas yang dikaitkan dengan key
 */
void trieInsertWordLower(Trie *T, Word w, int index);

/**
 * @brief Mencari semua entitas yang memiliki prefix sesuai sebuah Word.
 *
 * Prefix akan diperlakukan secara case-insensitive dengan mengubah ke
 * lowercase terlebih dahulu, lalu menggunakan trieSearchPrefix.
 *
 * @param T          Pointer ke Trie
 * @param prefixWord Word prefix yang dicari
 * @param outCount   Output: banyaknya index hasil
 *
 * @return Array dinamis berisi index (panjang = *outCount), atau NULL jika
 *         tidak ada hasil / gagal alokasi. Caller bertanggung jawab memanggil
 *         free() pada array hasil.
 */
int *trieSearchPrefixWordLower(const Trie *T, Word prefixWord, int *outCount);

#endif
