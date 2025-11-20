#ifndef POST_UTIL_H
#define POST_UTIL_H

#include "global-data.h"
#include "helper.h"
#include "user.h"
#include "Profil.h"
#include "../../adt/header/PostTree.h"

// Helper untuk Post dan SubGroddit
int findSubgrodditIndexByName(const char *subName);
int findPostIndexById(const char *postId);
void generatePostID(char *id, int num);

// Command utama fitur Post
void commandPost();                
void commandViewPost();            
void commandDeletePost();          

#endif