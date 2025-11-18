#ifndef COMMENT_H
#define COMMENT_H

#include <stdlib.h>
#include "MesinKata.h"

typedef struct
{
    int comment_id;
    Word post_id;
    Word author_id;
    int parent_comment_id;
    Word content;
    int upvotes;
    int downvotes;
} Comment;

typedef struct
{
    Word post_id;
    Word subgroddit_id;
    Word author_id;
    Word title;
    Word content;
    time_t created_at;
    int upvotes;
    int downvotes;
} Post;

typedef struct
{
    Word user_id;
    Word username;
    Word password;
    int karma;
    time_t created_at;
} User;

typedef struct
{
    Word subgroddit_id;
    Word name;
} SubGroddit;

typedef struct
{
    Word user_id;
    Word following_id;
} Social;

typedef struct
{
    Word user_id;
    Word target_type;
    Word target_id;
    Word vote_type;
} Voting;

void initComment(Comment *c);
void initPost(Post *p);
void initUser(User *u);
void initSubGroddit(SubGroddit *s);
void initSocial(Social *s);
void initVoting(Voting *v);

#endif
