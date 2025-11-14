#ifndef COMMENT_H
#define COMMENT_H

typedef struct
{
    char user_id[20];
    char username[256];
    char password[25];
    int karma;
    char created_at[20];
} User;

typedef struct
{
    char post_id[20];
    char subgroddit_id[20];
    char author_id[20];
    char title[256];
    char *content;
    char created_at[20];
    int upvotes;
    int downvotes;
} Post;

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

typedef struct
{
    char subgroddit_id[20];
    char name[256];
} Subgroddit;

typedef struct
{
    char follower_id[20];
    char following_id[20];
} Social;

typedef struct
{
    char user_id[20];
    char target_type[20];
    char target_id[20];
    char vote_type[20];
} Voting;

// Inisialisasi comment
void initUser(User *u);
void initPost(Post *p);
void initComment(Comment *c);
void initSubgroddit(Subgroddit *s);
void initSocial(Social *s);
void initVoting(Voting *v);


#endif
