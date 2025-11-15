#include <stdlib.h>
#include "header/adt-sederhana.h"
#include "header/MesinKata.h"
#include "header/MesinKalimat.h"

void initWord(Word *w) {
    w->Length = 0;
    w->TabWord[0] = '\0';
}

void initSentence(Sentence *s) {
    s->Length = 0;
    s->TabSentence[0] = '\0';
}

void initUser(User *u) {
    initWord(&u->user_id);
    initWord(&u->username);
    initWord(&u->password);
    u->karma = 0;
    initSentence(&u->created_at);
}

void initPost(Post *p) {
    initWord(&p->post_id);
    initWord(&p->subgroddit_id);
    initWord(&p->author_id);

    initSentence(&p->title);
    initSentence(&p->content);
    initSentence(&p->created_at);

    p->upvotes = 0;
    p->downvotes = 0;
}

void initComment(Comment *c) {
    c->comment_id = 0;
    initWord(&c->post_id);
    initWord(&c->author_id);
    c->parent_comment_id = -1;

    initSentence(&c->content);

    c->upvotes = 0;
    c->downvotes = 0;
}

void initSubgroddit(Subgroddit *s) {
    initWord(&s->subgroddit_id);
    initWord(&s->name);
}

void initSocial(Social *s) {
    initWord(&s->follower_id);
    initWord(&s->following_id);
}

void initVoting(Voting *v) {
    v->user_id = 0;
    initWord(&v->target_type);
    initWord(&v->target_id);
    initWord(&v->vote_type);
}
