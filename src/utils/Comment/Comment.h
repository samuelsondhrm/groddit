#ifndef COMMENT_UTIL_H
#define COMMENT_UTIL_H

#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../User/User.h"
#include "../Profil/Profil.h"
#include "../Voting/Voting.h"
#include "../ContentModeration/ContentModeration.h"

/**
 * Mencari Comment berdasarkan comment_id dan post_id.
 * @param postId        Post ID (e.g., "P001")
 * @param commentId     Comment ID (integer, e.g., 3)
 * @return              Pointer ke Comment jika ditemukan, NULL jika tidak ada
 */
Comment *getCommentById(const char *postId, int commentId);

/**
 * Mencari author_id dari sebuah Comment.
 * @param postId        Post ID terkait komentar
 * @param commentId     Comment ID yang dicari
 * @return              Pointer ke string author_id, atau NULL jika tidak ditemukan
 */
const char *getCommentAuthorId(const char *postId, int commentId);

/**
 * Mencari Comment menggunakan comment_id dan Word postId.
 * @param commentId     Comment ID yang dicari
 * @param postIdWord    Post ID dalam bentuk Word
 * @return              Pointer ke Comment jika ditemukan, NULL jika tidak ada
 */
Comment *getCommentPtr(int commentId, Word postIdWord);

/**
 * Menghapus komentar dari array COMMENTS pada index tertentu.
 * @param index         Index elemen yang akan dihapus
 */
void deleteCommentAtIndex(int index);

/**
 * Menghapus komentar dan seluruh reply di bawahnya secara rekursif.
 * @param commentId     ID komentar yang menjadi root penghapusan
 * @param postId        Post ID tempat komentar berada
 */
void deleteCommentRecursive(int commentId, const char *postId);

/**
 * Menangani perintah COMMENT untuk menambahkan komentar atau balasan.
 */
void commandAddComment();

/**
 * Menangani perintah DELETE_COMMENT untuk menghapus komentar.
 */
void commandDeleteComment();

/**
 * Menghasilkan ID komentar baru yang unik.
 * @return  Nilai max comment_id saat ini + 1
 */
int generateNewCommentId();

#endif