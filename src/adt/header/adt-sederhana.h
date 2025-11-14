#ifndef COMMENT_H
#define COMMENT_H

typedef struct {
    int comment_id;
    char post_id[100];
    char author_id[20];
    int parent_comment_id;
    char content[200];
    int upvotes;
    int downvotes;
} Comment;

// Inisialisasi comment
void initComment(Comment *c);

#endif
