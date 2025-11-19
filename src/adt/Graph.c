#include "header/Graph.h"

/* ============================================================================
 *               Helper Internal: Alokasi / Dealokasi Node
 * ============================================================================
 */

static AdjAddr newAdjNode(int v) {
    AdjAddr p = (AdjAddr) malloc(sizeof(AdjNode));
    if (p != NULL) {
        p->v = v;
        p->next = NULL;
    }
    return p;
}

static void freeAdjList(AdjAddr head) {
    AdjAddr p = head;
    while (p != NULL) {
        AdjAddr next = p->next;
        free(p);
        p = next;
    }
}

/* ============================================================================
 *                          Konstruktor & Destruktor
 * ============================================================================
 */

void createGraph(Graph *G, int nVertex) {
    G->nVertex = 0;
    G->adj = NULL;

    if (nVertex <= 0) {
        return;
    }

    AdjAddr *buf = (AdjAddr *) malloc(nVertex * sizeof(AdjAddr));
    if (buf == NULL) {
        return;
    }

    for (int i = 0; i < nVertex; i++) {
        buf[i] = NULL;
    }

    G->adj = buf;
    G->nVertex = nVertex;
}

void deallocateGraph(Graph *G) {
    if (G->adj != NULL) {
        for (int i = 0; i < G->nVertex; i++) {
            freeAdjList(G->adj[i]);
        }
        free(G->adj);
    }
    G->adj = NULL;
    G->nVertex = 0;
}

/* ============================================================================
 *                          Validasi & Query Dasar
 * ============================================================================
 */

boolean isValidVertex(Graph G, int v) {
    return (v >= 0 && v < G.nVertex);
}

boolean isAdjacent(Graph G, int u, int v) {
    if (!isValidVertex(G, u) || !isValidVertex(G, v)) {
        return false;
    }
    AdjAddr p = G.adj[u];
    while (p != NULL) {
        if (p->v == v) {
            return true;
        }
        p = p->next;
    }
    return false;
}

int outDegree(Graph G, int u) {
    if (!isValidVertex(G, u)) return 0;
    int count = 0;
    AdjAddr p = G.adj[u];
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}

int inDegree(Graph G, int v) {
    if (!isValidVertex(G, v)) return 0;
    int count = 0;
    for (int u = 0; u < G.nVertex; u++) {
        AdjAddr p = G.adj[u];
        while (p != NULL) {
            if (p->v == v) {
                count++;
                break; /* cukup satu per u */
            }
            p = p->next;
        }
    }
    return count;
}

/* ============================================================================
 *                          Operasi Update Edge
 * ============================================================================
 */

void addEdge(Graph *G, int u, int v) {
    if (!isValidVertex(*G, u) || !isValidVertex(*G, v)) return;
    if (u == v) return;                   /* tidak izinkan self-loop */
    if (isAdjacent(*G, u, v)) return;     /* hindari duplikat */

    AdjAddr node = newAdjNode(v);
    if (node == NULL) return;            /* gagal malloc */

    node->next = G->adj[u];
    G->adj[u] = node;
}

void removeEdge(Graph *G, int u, int v) {
    if (!isValidVertex(*G, u) || !isValidVertex(*G, v)) return;

    AdjAddr *pp = &G->adj[u];
    while (*pp != NULL) {
        if ((*pp)->v == v) {
            AdjAddr del = *pp;
            *pp = (*pp)->next;
            free(del);
            return;
        }
        pp = &(*pp)->next;
    }
}

/* ============================================================================
 *                          Operasi Update Vertex
 * ============================================================================
 */

void addVertex(Graph *G) {
    int oldN = G->nVertex;
    int newN = oldN + 1;

    AdjAddr *newAdj = (AdjAddr *) realloc(G->adj, newN * sizeof(AdjAddr));
    if (newAdj == NULL) {
        return;  /* gagal realloc → G tidak berubah */
    }

    G->adj = newAdj;
    G->adj[newN - 1] = NULL;
    G->nVertex = newN;
}

/* ============================================================================
 *                          Traversal / Debug
 * ============================================================================
 */

void printAdjacencyGraph(Graph G) {
    printf("Graph dengan %d vertex\n", G.nVertex);
    for (int u = 0; u < G.nVertex; u++) {
        printf("%d:", u);
        AdjAddr p = G.adj[u];
        while (p != NULL) {
            printf(" %d", p->v);
            p = p->next;
        }
        printf("\n");
    }
}
