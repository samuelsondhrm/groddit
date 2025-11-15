#ifndef ADTSEDERHANA_H
#define ADTSEDERHANA_H

#include "MesinKata.h"
#include "MesinKalimat.h"

typedef struct
{
    Word user_id;
    Word username;
    Word password;
    int karma;
    Sentence created_at;
} User;

typedef struct
{
    Word post_id;
    Word subgroddit_id;
    Word author_id;
    Sentence title;
    Sentence content;
    Sentence created_at;
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
