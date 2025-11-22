#include "Comment.h"


Comment* getCommentById(const char *postId, int commentId) {
    for (int i=0; i<COMMENT_COUNT; i++) {
        Comment *c = &COMMENTS[i];
        if (c->comment_id==commentId) {
            char currentPostId[NMax+1];
            wordToString(currentPostId, c->post_id);
            if (strCmp(currentPostId, postId) == 0) return c;
        }
    }
    return NULL;
}

const char* getCommentAuthorId(const char *postId, int commentId) {
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) return NULL;
    static char authorId[NMax+1];
    wordToString(authorId, comment->author_id);
    
    return authorId;
}