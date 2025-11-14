/* adt/load-config.c */
#include <stdio.h>
#include <stdlib.h>
#include "../../adt/header/adt-sederhana.h"
#include "../../utils/header/helper.h"

// Load
int loadUsers(const char *filepath, User **userPtr, int *capacityPtr);
int loadPosts(const char *filepath, Post **postPtr, int *capacityPtr);
int loadComments(const char *filepath, Comment **commentPtr, int *capacityPtr);
int loadSubgroddits(const char *filepath, Subgroddit **subPtr, int *capacityPtr);
int loadSocial(const char *filepath, Social **socialPtr, int *capacityPtr);
int loadVoting(const char *filepath, Voting **votePtr, int *capacityPtr);