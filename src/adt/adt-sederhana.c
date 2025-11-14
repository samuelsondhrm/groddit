#include "header/adt-sederhana.h"

void initComment(Comment *c)
{
    c->comment_id = 0;

    int i = 0;
    for (i = 0; i < 10; i++) c->post_id[i] = '\0';
    for (i = 0; i < 20; i++) c->author_id[i] = '\0';
    for (i = 0; i < 200; i++) c->content[i] = '\0';

    c->parent_comment_id = 0;
    c->upvotes = 0;
    c->downvotes = 0;
}