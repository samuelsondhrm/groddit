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

/**
 * Mencari Post berdasarkan post_id.
 * @param postId    Post ID yang dicari (e.g., "P001")
 * @return          Pointer ke Post jika ditemukan, NULL jika tidak ada.
 */
Post* getPostById(const char *postId);

/**
 * Mencari author_id (User ID) dari sebuah Post.
 * @param postId    Post ID yang dicari
 * @return          String author_id (static buffer), NULL jika post tidak ada
 */
const char* getPostAuthorId(const char *postId);

// Command utama fitur Post
void commandPost();    
void commandViewPost();
void commandDeletePost();


#endif