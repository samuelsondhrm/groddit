#include <stdlib.h>
#include "header/adt-sederhana.h"

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