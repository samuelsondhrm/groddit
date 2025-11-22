#ifndef LISTBERKAIT_H
#define LISTBERKAIT_H

#include <stdio.h>
#include <stdlib.h>
#include "../Boolean/Boolean.h"
#include "../ADTSederhana/ADTSederhana.h"

/*
 * ListElement dpt menyimpan salah satu dari:
 * - User
 * - Post
 * - Comment
 * - Subgroddit
 *
 * Elemen disimpan BY VALUE (bukan pointer).
 * Setiap elemen punya tag type buat identifikasi.
 */
typedef enum
{
    TYPE_USER,
    TYPE_POST,
    TYPE_COMMENT,
    TYPE_SUBGRODDIT
} ElementType;

typedef struct
{
    ElementType type;
    union
    {
        User user;
        Post post;
        Comment comment;
        SubGroddit subgroddit;
    } data;
} ListElement;

ListElement makeUserElement(User u);
ListElement makePostElement(Post p);
ListElement makeCommentElement(Comment c);
ListElement makeSubgrodditElement(SubGroddit s);

typedef struct Node
{
    ListElement element;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
} List;

/**
 * Membuat node baru berisi satu ListElement.
 * @param elem  Elemen yang ingin dimasukkan ke node.
 * @return      Pointer ke Node baru, NULL jika alokasi gagal.
 */
Node *newNode(ListElement elem);


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


// INSERT OPS
void insertFirstList(List *L, ListElement elem);
void insertLastList(List *L, ListElement elem);
void insertAtList(List *L, ListElement elem, int idx);


// DELETE OPS
void deleteFirstList(List *L, ListElement *out);
void deleteLastList(List *L, ListElement *out);
void deleteAtList(List *L, int idx, ListElement *out);


// ELMT ACCESS OPS
/**
 * Mengambil elemen pada index tertentu.
 * @param L     List yang akan diakses.
 * @param idx   Index elemen (0-based).
 * @param out   Output: elemen yang ditemukan.
 * @return      true jika berhasil, false jika index invalid.
 */
boolean getElementAt(List L, int idx, ListElement *out);

/**
 * Mengubah elemen pada index tertentu.
 * @param L     Pointer ke List.
 * @param idx   Index elemen (0-based).
 * @param elem  Elemen baru yang akan menggantikan.
 * @return      true jika berhasil, false jika index invalid.
 * 
 * Contoh buat update upvote:
 *   ListElement elem;
 *   getElementAt(POSTS, idx, &elem);
 *   elem.data.post.upvotes++;
 *   setElementAt(&POSTS, idx, elem);
 */
boolean setElementAt(List *L, int idx, ListElement elem);


// SEARCH OPS
/**
 * Mencari indeks elmt pak comparator.
 * @param L         List yang dicari.
 * @param target    Elemen target.
 * @param cmp       Fungsi pembanding.
 * @return          index (0-based) jika ketemu; -1 jika tidak.
 */
int indexOfList(
    List L,
    ListElement target,
    boolean (*cmp)(ListElement a, ListElement b));

/**
 * Mencari elemen dan return pointer ke elmt tsb.
 * @param L      List yang dicari.
 * @param target Elemen target.
 * @param cmp    Fungsi pembanding.
 * @return       Pointer ke ListElement (NULL jika tidak ketemu).
 */
ListElement* findElement(
    List L,
    ListElement target,
    boolean (*cmp)(ListElement a, ListElement b));


// FILTER AND COLLECTION OPS
/**
 * Membuat list baru berisi elmt yang memenuhi predicate.
 * @param L         List source
 * @param predicate Fungsi filter
 * @param context   param tambahan buat predicate (bisa NULL).
 * @return          List baru hasil filter .
 * 
 * e.g:
 *   boolean isPostByUser(ListElement e, void *ctx) {
 *       const char *userId = (const char*)ctx;
 *       if (e.type != TYPE_POST) return false;
 *       return wordEquals(e.data.post.author_id, userId);
 *   }
 *   
 *   List userPosts = filterList(POSTS, isPostByUser, "USER001");
 */
List filterList(
    List L,
    boolean (*predicate)(ListElement e, void *context),
    void *context
);

/**
 * Menghitung jumlah elemen yang memenuhi predicate.
 * Parameter sama kek filterList.
 */
int countIf(
    List L,
    boolean (*predicate)(ListElement e, void *context),
    void *context
);

// SORTING OPS
/**
 * Mengurutkan list menggunakan cmparator (inplace).
 * @param L     Pointer ke List yang akan diurutkan.
 * @param cmp   Fungsi compare.
 * 
 * e.g. buat sort post by upvotes (descending):
 * 
 *   int cmpPostByUpvotes(ListElement a, ListElement b) {
 *       return b.data.post.upvotes - a.data.post.upvotes; 
 *   }
 *   
 *   sortList(&POSTS, cmpPostByUpvotes);
 */
void sortList(List *L, int (*cmp)(ListElement a, ListElement b));

/**
 * Membuat copy list yang sudah diurut.
 * List asli TIDAK berubah.
 * 
 * @return List baru teruru.
 */
List sortedCopyList(List L, int (*cmp)(ListElement a, ListElement b));

/**
 * Membuat deep copy dari list.
 * Setiap node dialokasikan ulang, data disalin by value.
 * 
 * @param L List yang akan disalin.
 * @return  List baru (HARUS di-free dengan freeList).
 */
List copyList(List L);

/**
 * Membalik urutan list (IN-PLACE).
 * List asli akan berubah.
 */
void reverseList(List *L);

/**
 * Membuat copy list yang sudah dibalik.
 * List asli TIDAK berubah.
 */
List reversedCopyList(List L);

/**
 * Menampilkan seluruh elemen list menggunakan callback printer.
 * @param L     List yang ditampilkan
 * @param print Fungsi callback untuk menampilkan satu elemen.
 */
void displayList(List L, void (*print)(ListElement e));


List concatList(List L1, List L2);
void freeList(List *L);

#endif