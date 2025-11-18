#include "PostTree.h"

static void createChildrenList(CommentChildrenList *L) {
    L->head = NULL;
}

static void insertLastChild(CommentChildrenList *L, AddrComment node) {
    AddrChild n = malloc(sizeof(ChildNode));
    if (!n) return;
    n->info = node;
    n->next = NULL;

    if (!L->head) L->head = n;
    else {
        AddrChild p = L->head;
        while (p->next) p = p->next;
        p->next = n;
    }
}

static void deleteChildNode(CommentChildrenList *L, AddrComment target) {
    AddrChild p = L->head, prev = NULL;
    while (p) {
        if (p->info == target) {
            if (!prev) L->head = p->next;
            else prev->next = p->next;
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
}

static void deleteRec(AddrComment node) {
    AddrChild p = node->children.head;
    while (p) {
        AddrChild nx = p->next;
        deleteRec(p->info);
        free(p);
        p = nx;
    }
    free(node);
}

static void printIndent(int d) {
    for (int i = 0; i < d; i++) printf("  ");
}

static void printNodeRec(AddrComment node, int depth) {
    printIndent(depth);
    printf("- (%d) %s\n", node->data.comment_id, node->data.content.TabWord);

    AddrChild p = node->children.head;
    while (p) {
        printNodeRec(p->info, depth + 1);
        p = p->next;
    }
}

static AddrComment findDFS(AddrComment node, int id) {
    if (node->data.comment_id == id) return node;

    AddrChild p = node->children.head;
    while (p) {
        AddrComment res = findDFS(p->info, id);
        if (res) return res;
        p = p->next;
    }
    return NULL;
}

static int deleteInSubtree(AddrComment parent, int id) {
    AddrChild p = parent->children.head, prev = NULL;
    while (p) {
        if (p->info->data.comment_id == id) {
            if (!prev) parent->children.head = p->next;
            else prev->next = p->next;
            deleteRec(p->info);
            free(p);
            return 1;
        }
        if (deleteInSubtree(p->info, id)) return 1;
        prev = p;
        p = p->next;
    }
    return 0;
}

void CreatePostTree(PostTree *T, Post p) {
    T->root = p;
    createChildrenList(&T->children);
}

AddrComment NewCommentNode(Comment c) {
    AddrComment n = malloc(sizeof(CommentNode));
    if (!n) return NULL;
    n->data = c;
    createChildrenList(&n->children);
    return n;
}

void AddCommentToRoot(PostTree *T, Comment c) {
    AddrComment n = NewCommentNode(c);
    if (n) insertLastChild(&T->children, n);
}

void AddCommentToNode(AddrComment parent, Comment c) {
    AddrComment n = NewCommentNode(c);
    if (n) insertLastChild(&parent->children, n);
}

void AddComment(PostTree *T, int parent_comment_id, Comment c) {
    if (parent_comment_id == -1) {
        AddCommentToRoot(T, c);
        return;
    }
    AddrComment parent = FindCommentInTree(T, parent_comment_id);
    if (parent) AddCommentToNode(parent, c);
    else printf("Komentar parent %d tidak ditemukan!\n", parent_comment_id);
}

AddrComment FindCommentInTree(PostTree *T, int id) {
    AddrChild p = T->children.head;
    while (p) {
        AddrComment r = findDFS(p->info, id);
        if (r) return r;
        p = p->next;
    }
    return NULL;
}

void DeleteCommentSubtree(PostTree *T, int id) {
    AddrChild p = T->children.head, prev = NULL;
    while (p) {
        if (p->info->data.comment_id == id) {
            if (!prev) T->children.head = p->next;
            else prev->next = p->next;
            deleteRec(p->info);
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
    p = T->children.head;
    while (p) {
        if (deleteInSubtree(p->info, id)) return;
        p = p->next;
    }
}

void PrintPostTree(const PostTree *T) {
    AddrChild p = T->children.head;
    while (p) {
        printNodeRec(p->info, 0);
        p = p->next;
    }
}