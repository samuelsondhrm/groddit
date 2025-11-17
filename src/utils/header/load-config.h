#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include "../../adt/header/ADTSederhana.h"
#include "../../adt/header/MesinKata.h"
#include "helper.h"

// Load
int loadComments(const char *filepath, Comment **commentPtr, int *capacityPtr);
int loadPosts(const char *filepath, Post **postPtr, int *capacityPtr);
int loadUsers(const char *filepath, User **userPtr, int *capacityPtr);
int loadSubGroddits(const char *filepath, SubGroddit **subPtr, int *capacityPtr);
int loadSocials(const char *filepath, Social **socialPtr, int *capacityPtr);
int loadVotings(const char *filepath, Voting **votingPtr, int *capacityPtr);

#endif