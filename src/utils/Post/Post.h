#ifndef POST_UTIL_H
#define POST_UTIL_H

#include "../../adt/PostTree/PostTree.h"
#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../User/User.h"
#include "../Profil/Profil.h"

// Helper untuk Post dan SubGroddit
int findSubgrodditIndexByName(const char *subName);
int findPostIndexById(const char *postId);
void generatePostID(char *id, int num);

// Command utama fitur Post
void commandPost();                
void commandViewPost();            
void commandDeletePost();          

#endif