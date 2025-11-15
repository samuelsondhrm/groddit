#include <stdlib.h>
#include "header/adt-sederhana.h"

void initUser(User *u)
{
    u->user_id[0] = '\0';
    u->username[0] = '\0';
    u->password[0] = '\0';
    u->karma = 0;
    u->created_at[0] = '\0';
}

void initPost(Post *p)
{
    p->post_id[0] = '\0';
    p->subgroddit_id[0] = '\0';
    p->author_id[0] = '\0';
    p->title[0] = '\0';
    p->content = NULL;
    p->created_at[0] = '\0';
    p->upvotes = 0;
    p->downvotes = 0;
}

void initComment(Comment *c)
{
    c->comment_id = 0;
    c->post_id[0] = '\0';
    c->author_id[0] = '\0';
    c->parent_comment_id = -1;
    c->content = NULL;
    c->upvotes = 0;
    c->downvotes = 0;
}

void initSubgroddit(Subgroddit *s)
{
    s->subgroddit_id[0] = '\0';
    s->name[0] = '\n';
}

void initSocial(Social *s)
{
    s->follower_id[0] = '\0';
    s->following_id[0] = '\0';
}

void initVoting(Voting *v)
{
    v->user_id[0] = '\0';
    v->target_type[0] = '\0';
    v->target_id[0] = '\0';
    v->vote_type[0] = '\0';
}
