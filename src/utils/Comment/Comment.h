#ifndef COMMENT_UTIL_H
#define COMMENT_UTIL_H

#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../User/User.h"
#include "../Profil/Profil.h"

/**
 * Mencari Comment berdasarkan comment_id dan post_id.
 * @param postId        Post ID (e.g., "P001")
 * @param commentId     Comment ID (integer, e.g., 3)
 * @return              Pointer ke Comment jika ditemukan, NULL jika tidak ada
 */
Comment* getCommentById(const char *postId, int commentId);

/**
 * Mencari author_id dari sebuah Comment.
 */
const char* getCommentAuthorId(const char *postId, int commentId);


// nanti implementasi ini juga ris
void commandDeleteComment();


#endif