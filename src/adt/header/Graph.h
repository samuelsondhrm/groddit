#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "boolean.h"

/* ============================================================================
 *                              Definisi Tipe
 * ============================================================================
 *
 * Representasi graf berarah dengan adjacency list.
 *
 * - Vertex direpresentasikan sebagai integer 0..(nVertex-1)
 * - Dalam konteks sosial:
 *      vertex i ≡ USERS[i]
 *      edge (u -> v) ≡ user u FOLLOW user v
 */

typedef struct tAdjNode *AdjAddr;

typedef struct tAdjNode {
    int v;          /* index vertex tetangga (0..nVertex-1) */
    AdjAddr next;   /* pointer ke node berikutnya */
} AdjNode;

typedef struct {
    int nVertex;    /* jumlah vertex saat ini (0..nVertex-1) */
    AdjAddr *adj;   /* array of adjacency-list head, panjang nVertex */
} Graph;

#define IDX_UNDEF (-1)

/* ============================================================================
 *                          Konstruktor & Destruktor
 * ============================================================================
 */

/**
 * I.S.  G sembarang
 * F.S.  G terdefinisi dengan nVertex buah vertex (0..nVertex-1),
 *       dan tiap adj[i] = NULL. Jika nVertex <= 0 atau alokasi gagal,
 *       G->adj = NULL dan G->nVertex = 0.
 */
void createGraph(Graph *G, int nVertex);

/**
 * I.S.  G terdefinisi
 * F.S.  Seluruh node adjacency dan array adj dibebaskan,
 *       G->adj = NULL, G->nVertex = 0
 */
void deallocateGraph(Graph *G);

/* ============================================================================
 *                          Validasi & Query Dasar
 * ============================================================================
 */

boolean isValidVertex(Graph G, int v);

boolean isAdjacent(Graph G, int u, int v);

int outDegree(Graph G, int u);

int inDegree(Graph G, int v);

/* ============================================================================
 *                          Operasi Update Edge
 * ============================================================================
 */

/**
 * Menambahkan edge berarah (u -> v) ke graf.
 *
 * - Jika u atau v tidak valid → tidak melakukan apa-apa.
 * - Jika u == v → tidak menambahkan self-loop.
 * - Jika edge sudah ada → tidak menambahkan duplikat.
 */
void addEdge(Graph *G, int u, int v);

/**
 * Menghapus edge berarah (u -> v) dari graf.
 * Jika edge tidak ada atau u/v tidak valid → tidak melakukan apa-apa.
 */
void removeEdge(Graph *G, int u, int v);

/* ============================================================================
 *                          Operasi Update Vertex
 * ============================================================================
 */

/**
 * Menambahkan satu vertex baru ke graf.
 *
 * I.S.  G terdefinisi
 * F.S.  G->nVertex bertambah 1, array adj di-realloc untuk ukuran baru,
 *       dan adj[vertexBaru] = NULL.
 *
 * Jika realloc gagal → G tidak berubah.
 *
 * Catatan:
 *   - Dipanggil setiap REGISTER user baru selesai ditambahkan ke USERS.
 */
void addVertex(Graph *G);

/* ============================================================================
 *                          Traversal / Debug
 * ============================================================================
 */

/**
 * Menampilkan adjacency list graf dalam format:
 *   u: v1 v2 v3 ...
 * untuk debugging.
 */
void printAdjacencyGraph(Graph G);

#endif /* GRAPH_H */
