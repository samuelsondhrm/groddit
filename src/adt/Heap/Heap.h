#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include "../Boolean/Boolean.h"

/* ============================================================================
 *                               ADT Heap
 * ============================================================================
 *
 * Representasi: Binary heap berbasis tree dengan metode penyimpanan pada array
 *
 * Heap ini mendukung dua mode:
 *  - Max-heap: elemen dengan key terbesar berada di root
 *  - Min-heap: elemen dengan key terkecil berada di root
 *
 * Mode dipilih lewat field isMax saat createHeap().
 *
 * Dalam konteks FEED:
 *  - key      = waktu (dalam konteks fitur Feed, atribut created_at yang diconvert ke long) atau prioritas numerik sejenis
 *  - dataIdx  = indeks ke struktur data lain (mis. indeks post)
 */

typedef struct {
    long key;    /* nilai prioritas: makin besar/ kecil tergantung mode heap */
    int  dataIdx;/* indeks/ID data yang sebenarnya (mis. index POST) */
} HeapElType;

typedef struct {
    HeapElType *buffer; /* array penyimpan elemen heap */
    int size;           /* banyak elemen saat ini */
    int capacity;       /* kapasitas buffer (jumlah slot yang dialokasikan) */
    boolean isMax;      /* true  => max-heap, false => min-heap */
} Heap;

/* ============================================================================
 *                          Konstruktor & Destruktor
 * ============================================================================
 */

/**
 * I.S.  H sembarang
 * F.S.  Heap H terdefinisi dengan kapasitas awal = initialCap
 *       - Jika initialCap <= 0 atau malloc gagal:
 *           H.buffer = NULL, H.size = 0, H.capacity = 0
 *       - isMax di-set sesuai argumen
 */
void createHeap(Heap *H, int initialCap, boolean isMax);

/**
 * I.S.  H terdefinisi
 * F.S.  Memori buffer H dilepas (free), H menjadi heap kosong tak teralokasi:
 *       H.buffer = NULL, H.size = 0, H.capacity = 0
 */
void deallocateHeap(Heap *H);

/* ============================================================================
 *                          Selektor & Predikat
 * ============================================================================
 */

boolean isHeapEmpty(Heap H);
/* Mengembalikan true jika H.size == 0 */

int heapSize(Heap H);
/* Mengembalikan banyak elemen dalam heap */

int heapCapacity(Heap H);
/* Mengembalikan kapasitas array buffer heap */

/* ============================================================================
 *                          Operasi Utama Heap
 * ============================================================================
 */

/**
 * Menambahkan elemen x ke dalam heap H.
 *
 * I.S.  H terdefinisi
 * F.S.  Jika perlu, buffer direalokasi (kapasitas dinaikkan, misalnya 2x).
 *       H.size bertambah 1, heap property terjaga.
 *
 * Mengembalikan:
 *  - true  jika sukses
 *  - false jika realloc gagal (heap tidak berubah)
 */
boolean heapPush(Heap *H, HeapElType x);

/**
 * Mengambil elemen berprioritas tertinggi (root) tanpa menghapusnya.
 *
 * I.S.  H terdefinisi, mungkin kosong
 * F.S.  Jika H tidak kosong, *out diisi dengan elemen root dan fungsi
 *       mengembalikan true. Jika kosong, mengembalikan false dan *out
 *       tidak diubah.
 */
boolean heapTop(Heap H, HeapElType *out);

/**
 * Menghapus dan mengembalikan elemen berprioritas tertinggi (root).
 *
 * I.S.  H terdefinisi, mungkin kosong
 * F.S.  Jika H tidak kosong:
 *         - *out diisi elemen root
 *         - H.size berkurang 1
 *         - heap property tetap terjaga
 *       Jika kosong:
 *         - mengembalikan false, *out tidak diubah
 */
boolean heapPop(Heap *H, HeapElType *out);

#endif /* HEAP_H */
