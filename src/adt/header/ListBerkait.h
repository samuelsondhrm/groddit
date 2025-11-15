#ifndef LISTBERKAIT_H
#define LISTBERKAIT_H

#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"
#include "adt-sederhana.h"

/* ============================================================================
 *                          Element Variant Type
 * ============================================================================
 * ListElement dapat menyimpan salah satu dari:
 * - User
 * - Post
 * - Comment
 * - Subgroddit
 *
 * Elemen disimpan BY VALUE (bukan pointer), aman terhadap memory leak.
 * Setiap elemen memiliki tag type untuk identifikasi.
 */

typedef enum {
    TYPE_USER,
    TYPE_POST,
    TYPE_COMMENT,
    TYPE_SUBGRODDIT
} ElementType;

typedef struct {
    ElementType type;
    union {
        User user;
        Post post;
        Comment comment;
        Subgroddit subgroddit;
    } data;
} ListElement;

/* Factory untuk membuat ListElement berdasarkan tipe */
ListElement makeUserElement(User u);
ListElement makePostElement(Post p);
ListElement makeCommentElement(Comment c);
ListElement makeSubgrodditElement(Subgroddit s);

/* ============================================================================
 *                                 Node & List
 * ============================================================================
 */

typedef struct Node {
    ListElement element;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} List;

/* ============================================================================
 *                          Node Constructor / Initializer
 * ============================================================================
 */

/**
 * Membuat node baru berisi satu ListElement.
 * @param elem  Elemen yang ingin dimasukkan ke node.
 * @return      Pointer ke Node baru; NULL jika alokasi gagal.
 */
Node* newNode(ListElement elem);

/* ============================================================================
 *                          Create & Validation
 * ============================================================================
 */

/**
 * Inisialisasi list kosong.
 * @param L Pointer ke List yang akan diinisialisasi.
 */
void createList(List *L);

/**
 * Mengecek apakah list kosong.
 * @param L List yang akan dicek.
 * @return true jika kosong, false jika tidak.
 */
boolean isEmptyList(List L);

/**
 * Menghasilkan panjang list.
 * @param L List input.
 * @return jumlah elemen dalam list.
 */
int listLength(List L);

/* ============================================================================
 *                          Insertion Operations
 * ============================================================================
 */

void insertFirstList(List *L, ListElement elem);
void insertLastList(List *L, ListElement elem);
void insertAtList(List *L, ListElement elem, int idx);

/* ============================================================================
 *                          Deletion Operations
 * ============================================================================
 */

void deleteFirstList(List *L, ListElement *out);
void deleteLastList(List *L, ListElement *out);
void deleteAtList(List *L, int idx, ListElement *out);

/* ============================================================================
 *                          Search / Find Operations
 * ============================================================================
 */

/**
 * Mencari indeks elemen menggunakan comparator.
 * Comparator harus memiliki bentuk:
 *      boolean cmp(ListElement a, ListElement b)
 *
 * @param L         List yang dicari.
 * @param target    Elemen target.
 * @param cmp       Fungsi pembanding.
 * @return          index (0-based) jika ketemu; -1 jika tidak.
 */
int indexOfList(
    List L,
    ListElement target,
    boolean (*cmp)(ListElement a, ListElement b)
);

/* ============================================================================
 *                          Traversal / Display
 * ============================================================================
 */

/**
 * Menampilkan seluruh elemen list menggunakan callback printer.
 * Callback print harus berbentuk:
 *      void printElement(ListElement e)
 *
 * @param L     List yang ingin ditampilkan.
 * @param print Fungsi callback untuk menampilkan satu elemen.
 */
void displayList(List L, void (*print)(ListElement e));

/* ============================================================================
 *                          Utility Operations
 * ============================================================================
 */

List concatList(List L1, List L2);

/**
 * Menghapus semua node list TANPA menghapus data di dalamnya
 * (karena elemen disimpan by value, tidak butuh free).
 */
void freeList(List *L);

#endif
