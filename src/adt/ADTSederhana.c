#include "header/ADTSederhana.h"

void initComment(Comment *c)
{
    c->comment_id = 0;
    c->post_id.Length = 0;
    c->author_id.Length = 0;
    c->content.Length = 0;
    c->parent_comment_id = -1;
    c->upvotes = 0;
    c->downvotes = 0;
}

void initPost(Post *p)
{
    p->post_id.Length = 0;
    p->subgroddit_id.Length = 0;
    p->author_id.Length = 0;
    p->title.Length = 0;
    p->content.Length = 0;
    p->created_at = 0;
    p->upvotes = 0;
    p->downvotes = 0;
}

void initUser(User *u)
{
    u->user_id.Length = 0;
    u->username.Length = 0;
    u->password.Length = 0;
    u->karma = 0;
    u->created_at = 0;
}

void initSubGroddit(SubGroddit *s)
{
    s->subgroddit_id.Length = 0;
    s->name.Length = 0;
}

void initSocial(Social *s)
{
    s->user_id.Length = 0;
    s->following_id.Length = 0;
}

void initVoting(Voting *v)
{
    v->user_id.Length = 0;
    v->target_type.Length = 0;
    v->target_id.Length = 0;
    v->vote_type.Length = 0;
}
