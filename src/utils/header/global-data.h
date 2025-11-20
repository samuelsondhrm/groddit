#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include "../../adt/header/ADTSederhana.h"
#include "../../adt/header/Graph.h"
#include "../../adt/header/ListBerkait.h"

// Comments
extern Comment *COMMENTS;
extern int COMMENT_CAPACITY;
extern int COMMENT_COUNT;

// Posts (disimpan dalam ListBerkait)
extern List POSTS;
extern int POST_COUNT;

// Users
extern User *USERS;
extern int USER_CAPACITY;
extern int USER_COUNT;

// SubGroddits (disimpan dalam ListBerkait)
extern List SUBGRODDITS;
extern int SUBGRODDIT_COUNT;

// Social
extern Social *SOCIALS;
extern int SOCIAL_CAPACITY;
extern int SOCIAL_COUNT;
// Socials
extern Graph SOCIAL_GRAPH;

// Votings
extern Voting *VOTINGS;
extern int VOTING_CAPACITY;
extern int VOTING_COUNT;

// User saat ini
extern int CURRENT_USER_INDEX;
boolean isLoggedIn();
#endif
