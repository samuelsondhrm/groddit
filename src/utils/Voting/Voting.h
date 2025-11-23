#ifndef VOTING_UTIL_H
#define VOTING_UTIL_H

#include <stdio.h>
#include <stdlib.h>

#include "../../adt/ADTSederhana/ADTSederhana.h"
#include "../../adt/ListBerkait/ListBerkait.h"
#include "../../adt/MesinKata/MesinKata.h"
#include "../GlobalData/GlobalData.h"
#include "../Helper/Helper.h"
#include "../Post/Post.h"
#include "../Comment/Comment.h"
#include "../User/User.h"

/**
 * Mencari index voting di VOTINGS array.
 * @param userId        User ID yang melakukan vote (e.g., "USER001")
 * @param targetId      Target ID (Post ID atau Comment ID as string)
 * @param targetType    "POST" / "COMMENT"
 * @return              Index di VOTINGS array, -1 jika kl ga nemu
 */
int findVotingIndex(const char *userId, const char *targetId, const char *targetType);

/**
 * Menambah vote baru ke VOTINGS array.
 * Otomatis melakukan realloc jika kapasitas tidak cukup.
 * @param userId        User ID yang melakukan vote
 * @param targetId      Target ID (Post/Comment ID)
 * @param targetType    "POST" or "COMMENT"
 * @param voteType      "UPVOTE" or "DOWNVOTE"
 */
void addNewVote(const char *userId, const char *targetId, 
                const char *targetType, const char *voteType);

/**
 * Mengubah vote yang sudah ada (toggle upvote <-> downvote).
 * @param votingIdx     Index voting di VOTINGS array
 * @param newVoteType   "UPVOTE" atau "DOWNVOTE"
 */
void updateVote(int votingIdx, const char *newVoteType);

/**
 * Menghapus vote dari VOTINGS array.
 * Menggeser elemen setelahnya ke kiri (compact array).
 * @param votingIdx     Index voting yang akan dihapus
 */
void removeVote(int votingIdx);

/**
 * Update counter upvotes/downvotes pada Post.
 * @param postId        Post ID (e.g., "P001")
 * @param upvoteDelta   Perubahan upvotes: +1, -1, atau 0
 * @param downvoteDelta Perubahan downvotes: +1, -1, atau 0
 */
void updatePostVotes(const char *postId, int upvoteDelta, int downvoteDelta);

/**
 * Update counter upvotes/downvotes pada Comment.
 * @param postId        Post ID tempat comment berada
 * @param commentId     Comment ID (integer)
 * @param upvoteDelta   Perubahan upvotes: +1, -1, atau 0
 * @param downvoteDelta Perubahan downvotes: +1, -1, atau 0
 */
void updateCommentVotes(const char *postId, int commentId, int upvoteDelta, int downvoteDelta);

/**
 * Update karma user (pemilik konten).
 * @param userId        User ID pemilik konten
 * @param karmaDelta    Perubahan karma: +1, -1, +2, atau -2
 */
void updateUserKarma(const char *userId, int karmaDelta);

/**
 * Menghapus semua voting terkait target tertentu (buat CASCADINH delete).
 * @param targetId      Target ID (Post/Comment ID)
 * @param targetType    "POST" or "COMMENT"
 */
void deleteVotingsByTarget(const char *targetId, const char *targetType);

/**
 * Helper buat convert integer comment ID ke string.
 * @param commentId     Comment ID (integer)
 * @param out           Buffer output
 */
void commentIdToString(int commentId, char *out);


// COMMAND HANDLERS
void commandUpvotePost();
void commandDownvotePost();
void commandUndoVotePost();
void commandUpvoteComment();
void commandDownvoteComment();
void commandUndoVoteComment();

#endif