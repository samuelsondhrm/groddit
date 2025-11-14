#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include "../../adt/header/adt-sederhana.h" 

// Users
extern User *USERS;
extern int USER_CAPACITY;
extern int USER_COUNT;

// Posts
extern Post *POSTS;
extern int POST_CAPACITY;
extern int POST_COUNT;

// Comments
extern Comment *COMMENTS;
extern int COMMENT_CAPACITY;
extern int COMMENT_COUNT;

// Subgroddits
extern Subgroddit *SUBGRODDITS;
extern int SUBGRODDIT_CAPACITY;
extern int SUBGRODDIT_COUNT;

// Socials
extern Social *SOCIALS;
extern int SOCIAL_CAPACITY;
extern int SOCIAL_COUNT;

// Votings
extern Voting *VOTINGS;
extern int VOTING_CAPACITY;
extern int VOTING_COUNT;

#endif
