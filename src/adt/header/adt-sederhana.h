#ifndef COMMENT_H
#define COMMENT_H
typedef struct
{
    int comment_id;
    char post_id[20];
    char author_id[20];
    int parent_comment_id;
    char *content;
    int upvotes;
    int downvotes;
} Comment;

#endif
