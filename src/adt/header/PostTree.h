#ifndef POSTTREE_H
#define POSTTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "Boolean.h"
#include "ADTSederhana.h"

/* ============================================================================
 *                              Comment Tree Node
 * ============================================================================
 * Setiap node komentar menyimpan:
 * - Data Comment (BY VALUE).
 * - Daftar anak komentar (pointer ke CommentNode).
 *
 * Komentar dapat memiliki kedalaman tak terbatas.
 * ==========================================================================*/

typedef struct CommentNode *AddrComment;

/* ============================================================================
 *                        Linked List of CommentNode*
 * ============================================================================
 * ChildrenList menyimpan node-node anak komentar.
 * Setiap ChildNode adalah pointer ke CommentNode (AddrComment).
 * ==========================================================================*/

typedef struct ChildNode *AddrChild;

typedef struct ChildNode
{
    AddrComment info; /* pointer ke node komentar */
    AddrChild next;   /* next sibling */
} ChildNode;

typedef struct
{
    AddrChild head; /* head of children list */
} CommentChildrenList;

/* ============================================================================
 *                               Comment Node
 * ==========================================================================*/

typedef struct CommentNode
{
    Comment data;                 /* data komentar (value) */
    CommentChildrenList children; /* daftar anak komentar */
} CommentNode;

/* ============================================================================
 *                                  PostTree
 * ============================================================================
 * Struktur tree untuk satu Post:
 * - root : Post
 * - children : daftar komentar level-1 (CommentNode*)
 * ==========================================================================*/

typedef struct
{
    Post root;                    /* data post (root) */
    CommentChildrenList children; /* komentar level pertama */
} PostTree;

/* ============================================================================
 *                                 PRIMITIVES
 * ==========================================================================*/

/**
 * @brief Inisialisasi sebuah PostTree dari Post.
 *
 * @param T Pointer ke PostTree
 * @param p Post yang menjadi akar
 */
void CreatePostTree(PostTree *T, Post p);

/**
 * @brief Membuat sebuah node komentar baru.
 *
 * @param c Data komentar
 * @return AddrComment Alamat node komentar baru (NULL jika gagal)
 */
AddrComment NewCommentNode(Comment c);

/**
 * @brief Menambahkan komentar sebagai anak langsung Post (level-1).
 *
 * @param T Pointer ke PostTree
 * @param c Komentar baru
 */
void AddCommentToRoot(PostTree *T, Comment c);

/**
 * @brief Menambahkan komentar sebagai anak dari node parent.
 *
 * @param parent Pointer ke CommentNode parent
 * @param c Komentar baru
 */
void AddCommentToNode(AddrComment parent, Comment c);

/**
 * @brief Menambahkan komentar sesuai parent_comment_id.
 *
 * @param T Pointer ke PostTree
 * @param parent_comment_id -1 untuk komentar level pertama,
 *                          selain itu reply ke komentar tertentu
 * @param c Komentar baru
 */
void AddComment(PostTree *T, int parent_comment_id, Comment c);

/**
 * @brief Mencari komentar berdasarkan ID dengan DFS.
 *
 * @param T Pointer ke PostTree
 * @param comment_id ID komentar
 * @return AddrComment Node komentar jika ditemukan, NULL jika tidak
 */
AddrComment FindCommentInTree(PostTree *T, int comment_id);

/**
 * @brief Menghapus sebuah komentar berserta seluruh subtree anaknya.
 *
 * @param T Pointer ke PostTree
 * @param comment_id ID komentar yang ingin dihapus
 */
void DeleteCommentSubtree(PostTree *T, int comment_id);

/**
 * @brief Menampilkan seluruh komentar (indentasi otomatis).
 *
 * @param T Pointer ke PostTree
 */
void PrintPostTree(const PostTree *T);

#endif
