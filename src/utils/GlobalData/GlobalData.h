#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include "../../adt/ADTSederhana/ADTSederhana.h"
#include "../../adt/Graph/Graph.h"
#include "../../adt/ListBerkait/ListBerkait.h"

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
extern Social *SOCIALS; // hanya buffer hasil parsing social.csv
extern int SOCIAL_CAPACITY;
extern int SOCIAL_COUNT;

extern Graph SOCIAL_GRAPH; // Semua operasi Social akan terhubung ke graf ini

// Votings
extern Voting *VOTINGS;
extern int VOTING_CAPACITY;
extern int VOTING_COUNT;

// User saat ini
extern int CURRENT_USER_INDEX;
boolean isLoggedIn();
#endif
