#include "Heap.h"

/* ============================================================================
 *                      Helper Internal: Utilitas Heap
 * ============================================================================
 */

static void swapEl(HeapElType *a, HeapElType *b) {
    HeapElType tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Mengembalikan true jika a lebih "diutamakan" daripada b
 * - Max-heap: a.key > b.key
 * - Min-heap: a.key < b.key
 */
static boolean isHigherPriority(Heap *H, HeapElType a, HeapElType b) {
    if (H->isMax) {
        return (a.key > b.key);
    } else {
        return (a.key < b.key);
    }
}

/* Memperluas kapasitas buffer jika penuh.
 * Strategi: jika capacity == 0 -> set ke 4,
 *           else -> gandakan (capacity *= 2).
 * Return true jika sukses, false jika realloc gagal.
 */
static boolean ensureHeapCapacity(Heap *H, int minCapacity) {
    if (H->capacity >= minCapacity) return true;

    int newCap = (H->capacity <= 0) ? 4 : (H->capacity * 2);
    if (newCap < minCapacity) {
        newCap = minCapacity;
    }

    HeapElType *newBuf = (HeapElType *) realloc(H->buffer, newCap * sizeof(HeapElType));
    if (newBuf == NULL) {
        return false;
    }

    H->buffer = newBuf;
    H->capacity = newCap;
    return true;
}

static void upHeap(Heap *H, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (!isHigherPriority(H, H->buffer[idx], H->buffer[parent])) {
            break;
        }
        swapEl(&H->buffer[idx], &H->buffer[parent]);
        idx = parent;
    }
}

static void downHeap(Heap *H, int idx) {
    int n = H->size;
    while (1) {
        int left  = 2 * idx + 1;
        int right = 2 * idx + 2;
        int best = idx;

        if (left < n && isHigherPriority(H, H->buffer[left], H->buffer[best])) {
            best = left;
        }
        if (right < n && isHigherPriority(H, H->buffer[right], H->buffer[best])) {
            best = right;
        }

        if (best == idx) break;

        swapEl(&H->buffer[idx], &H->buffer[best]);
        idx = best;
    }
}

/* ============================================================================
 *                          Konstruktor & Destruktor
 * ============================================================================
 */

void createHeap(Heap *H, int initialCap, boolean isMax) {
    H->buffer = NULL;
    H->size = 0;
    H->capacity = 0;
    H->isMax = isMax;

    if (initialCap <= 0) {
        return;
    }

    H->buffer = (HeapElType *) malloc(initialCap * sizeof(HeapElType));
    if (H->buffer == NULL) {
        H->capacity = 0;
        return;
    }

    H->capacity = initialCap;
}

void deallocateHeap(Heap *H) {
    if (H->buffer != NULL) {
        free(H->buffer);
    }
    H->buffer = NULL;
    H->size = 0;
    H->capacity = 0;
    /* isMax bisa dibiarkan apa adanya, tidak masalah */
}

/* ============================================================================
 *                          Selektor & Predikat
 * ============================================================================
 */

boolean isHeapEmpty(Heap H) {
    return (H.size == 0);
}

int heapSize(Heap H) {
    return H.size;
}

int heapCapacity(Heap H) {
    return H.capacity;
}

/* ============================================================================
 *                          Operasi Utama Heap
 * ============================================================================
 */

boolean heapPush(Heap *H, HeapElType x) {
    /* Pastikan kapasitas cukup */
    if (!ensureHeapCapacity(H, H->size + 1)) {
        return false;
    }

    /* Tambah di posisi paling belakang, lalu upHeap */
    H->buffer[H->size] = x;
    H->size++;
    upHeap(H, H->size - 1);
    return true;
}

boolean heapTop(Heap H, HeapElType *out) {
    if (H.size == 0) {
        return false;
    }
    if (out != NULL) {
        *out = H.buffer[0];
    }
    return true;
}

boolean heapPop(Heap *H, HeapElType *out) {
    if (H->size == 0) {
        return false;
    }

    if (out != NULL) {
        *out = H->buffer[0];
    }

    /* Pindahkan elemen terakhir ke root, perkecil size, lalu downHeap */
    H->buffer[0] = H->buffer[H->size - 1];
    H->size--;

    if (H->size > 0) {
        downHeap(H, 0);
    }

    return true;
}
