#ifndef ADTSEDERHANA_H
#define ADTSEDERHANA_H

#include "MesinKata.h"
#include "MesinKalimat.h"

#include <time.h>

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
    char post_id[20];
    char subgroddit_id[20];
    char author_id[20];
    char title[256];
    char *content;
    time_t created_at;
    int upvotes;
    int downvotes;
} Post;

typedef struct
{
    int comment_id;
    Word post_id;
    Word author_id;
    int parent_comment_id;
    Sentence content;
    int upvotes;
    int downvotes;
} Comment;

typedef struct
{
    Word subgroddit_id;
    Word name;
} Subgroddit;

typedef struct
{
    Word follower_id;
    Word following_id;
} Social;

typedef struct
{
    int user_id;
    Word target_type;
    Word target_id;
    Word vote_type;
} Voting;

// Inisialisasi struktur
void initUser(User *u);
void initPost(Post *p);
void initComment(Comment *c);
void initSubgroddit(Subgroddit *s);
void initSocial(Social *s);
void initVoting(Voting *v);

#endif
