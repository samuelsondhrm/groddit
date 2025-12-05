#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include "../../adt/ADTSederhana/ADTSederhana.h"
#include "../../adt/MesinKata/MesinKata.h"
#include "../../adt/ListBerkait/ListBerkait.h"
#include "../Helper/Helper.h"
#include "../Security/Security.h"

// Load
int loadComments(const char *filepath, Comment **commentPtr, int *capacityPtr);
int loadPosts(const char *filepath, List *postList);
int loadUsers(const char *filepath, User **userPtr, int *capacityPtr);
int loadSubGroddits(const char *filepath, List *subList);
int loadSocials(const char *filepath, Social **socialPtr, int *capacityPtr);
int loadVotings(const char *filepath, Voting **votingPtr, int *capacityPtr);

// Load from buffer for File Encryption
int loadComments_Buffer(uint8_t *buffer, size_t len,
                        Comment **commentPtr, int *capacityPtr);
int loadPosts_Buffer(uint8_t *buffer, size_t len, List *postList);
int loadUsers_Buffer(uint8_t *buffer, size_t len, User **userPtr, int *capacityPtr);
int loadSubGroddits_Buffer(uint8_t *buffer, size_t len, List *subList);
int loadSocials_Buffer(uint8_t *buffer, size_t len,
                       Social **socialPtr, int *capacityPtr);
int loadVotings_Buffer(uint8_t *buffer, size_t len,
                       Voting **votingPtr, int *capacityPtr);

#endif