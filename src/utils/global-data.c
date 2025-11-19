#include "header/global-data.h"
#include <stdlib.h>

// Comment
Comment *COMMENTS = NULL;
int COMMENT_CAPACITY = 0;
int COMMENT_COUNT = 0;

// Posts
Post *POSTS = NULL;
int POST_CAPACITY = 0;
int POST_COUNT = 0;

// Users
User *USERS = NULL;
int USER_CAPACITY = 0;
int USER_COUNT = 0;

// SubGroddits
SubGroddit *SUBGRODDITS = NULL;
int SUBGRODDIT_CAPACITY = 0;
int SUBGRODDIT_COUNT = 0;

// Social
Social *SOCIALS = NULL;
int SOCIAL_CAPACITY = 0;
int SOCIAL_COUNT = 0;
Graph SOCIAL_GRAPH;

// Votings
Voting *VOTINGS = NULL;
int VOTING_CAPACITY = 0;
int VOTING_COUNT = 0;

int CURRENT_USER_INDEX = -1;

boolean isLoggedIn()
{
    return CURRENT_USER_INDEX != -1;
}