#include "Voting.h"

int findVotingIndex(const char *userId, const char *targetId, const char *targetType) {
    for (int i=0; i<VOTING_COUNT; i++){
        char vUserId[NMax+1];
        char vTargetId[NMax+1];
        char vTargetType[NMax+1];

        wordToString(vUserId, VOTINGS[i].user_id);
        wordToString(vTargetId, VOTINGS[i].target_id);
        wordToString(vTargetType, VOTINGS[i].target_type);

        if (strCmp(vUserId,userId)==0 &&
            strCmp(vTargetId, targetId)==0 &&
            strCmp(vTargetType, targetType)==0) return i;
    }
    return -1;
}

void addNewVote(const char *userId, const char *targetId, const char *targetType, const char *voteType){
    if (VOTING_COUNT >= VOTING_CAPACITY) {
        int newCapacity = VOTING_CAPACITY*2;
        if (newCapacity<10) newCapacity=10;

        if (!ensureCapacity((void**)&VOTINGS, &VOTING_CAPACITY, sizeof(Voting), newCapacity)) {
            printf("Error: gagal allocating memori untuk voting baru.\n");
            return;
        }
    }

    //tambah voting baru
    Voting *v = &VOTINGS[VOTING_COUNT];
    stringToWord(&v->user_id, userId);
    stringToWord(&v->target_id, targetId);
    stringToWord(&v->target_type, targetType);
    stringToWord(&v->vote_type, voteType);
    VOTING_COUNT++;
}

void updateVote(int votingIdx, const char *newVoteType) {
    if (votingIdx<0 || votingIdx>=VOTING_COUNT) return;
    stringToWord(&VOTINGS[votingIdx].vote_type, newVoteType);
}

void removeVote(int votingIdx) {
    if (votingIdx < 0 || votingIdx >= VOTING_COUNT) return;
    // printf("[DEBUG] Removing vote at index %d (VOTING_COUNT=%d)\n", votingIdx, VOTING_COUNT);
    
    for (int i = votingIdx; i < VOTING_COUNT - 1; i++) {
        VOTINGS[i] = VOTINGS[i + 1];
    }
    
    VOTING_COUNT--;
    
    if (VOTING_COUNT >= 0) {
        Voting *lastVoting = &VOTINGS[VOTING_COUNT];
        lastVoting->user_id.Length = 0;
        lastVoting->target_id.Length = 0;
        lastVoting->target_type.Length = 0;
        lastVoting->vote_type.Length = 0;
    }
    // printf("[DEBUG] After remove: VOTING_COUNT=%d\n", VOTING_COUNT);

}

void updatePostVotes(const char *postId, int upvoteDelta, int downvoteDelta) {
    Post *post = getPostById(postId);
    if (post==NULL) return;

    post->upvotes += upvoteDelta;
    post->downvotes += downvoteDelta;

    if (post->upvotes < 0) post->upvotes = 0;
    if (post->downvotes < 0) post->downvotes = 0;
}

void updateCommentVotes(const char *postId, int commentId, int upvoteDelta, int downvoteDelta) {
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) return;

    comment->upvotes += upvoteDelta;
    comment->downvotes += downvoteDelta;
    
    if (comment->upvotes < 0) comment->upvotes = 0;
    if (comment->downvotes < 0) comment->downvotes = 0;
}

void updateUserKarma(const char *userId, int karmaDelta) {
    int userIdx = findIdByUsername(userId);
    if (userIdx==-1) return;
    USERS[userIdx].karma += karmaDelta;
}

void deleteVotingsByTarget(const char *targetId, const char *targetType) {
    int i=0;
    while (i<VOTING_COUNT) {
        char vTargetId[NMax+1];
        char vTargetType[NMax+1];
        wordToString(vTargetId, VOTINGS[i].target_id);
        wordToString(vTargetType, VOTINGS[i].target_type);

        if (strCmp(vTargetId, targetId)==0 &&
            strCmp(vTargetType, targetType)==0) removeVote(i);
        else i++;
    }
}

void commentIdToString(int commentId, char *out){
    sprintf(out, "%d", commentId);
}


// POST VOTING COMMANDS
void commandUpvotePost() {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat memberikan vote.\n");
        return;
    }
    
    ADVWORD_INPUT();
    
    if (currentWord.Length == 0) {
        printf("Format salah. Gunakan: UPVOTE_POST <POST_ID>;\n");
        return;
    }
    
    char postId[NMax + 1];
    wordToString(postId, currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Format salah. Gunakan: UPVOTE_POST <POST_ID>;\n");
        return;
    }
    
    //validasi
    Post *post = getPostById(postId);
    if (post == NULL) {
        printf("Post dengan ID %s tidak ditemukan!\n", postId);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    
    const char *authorId = getPostAuthorId(postId);
    if (authorId!=NULL && strCmp(currentUserId, authorId) == 0) {
        printf("Anda tidak dapat memberikan vote pada post Anda sendiri!\n");
        return;
    }
    
    int votingIdx = findVotingIndex(currentUserId, postId, "POST");
    
    if (votingIdx==-1) {
        addNewVote(currentUserId, postId, "POST", "UPVOTE");
        updatePostVotes(postId, 1, 0);
        updateUserKarma(authorId, 1);
        printf("Anda berhasil memberikan upvote pada post [%s].\n", postId);
    } else {
        char existingVoteType[NMax+1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "UPVOTE") == 0) {
            printf("Anda sudah memberikan upvote pada post [%s].\n", postId);
        } else {
            updateVote(votingIdx, "UPVOTE");
            updatePostVotes(postId, 1, -1);
            updateUserKarma(authorId, 2);
            printf("Vote Anda pada post [%s] berhasil diubah menjadi upvote.\n", postId);
        }
    }
}

void commandDownvotePost() {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat memberikan vote.\n");
        return;
    }
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printf("Format salah. Gunakan: UPVOTE_POST <POST_ID>;\n");
        return;
    }
    
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printf("Format salah. Gunakan: DOWNVOTE_POST <POST_ID>;\n");
        return;
    }
    
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("Post dengan ID %s tidak ditemukan!\n", postId);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    
    const char *authorId = getPostAuthorId(postId);
    if (authorId != NULL && strCmp(currentUserId, authorId) == 0) {
        printf("Anda tidak dapat memberikan vote pada post Anda sendiri!\n");
        return;
    }
    
    int votingIdx = findVotingIndex(currentUserId, postId, "POST");
    if (votingIdx ==-1) {
        //case blm pernah vote
        addNewVote(currentUserId, postId, "POST", "DOWNVOTE");
        updatePostVotes(postId, 0, 1);
        updateUserKarma(authorId, -1);
        printf("Anda berhasil memberikan downvote pada post [%s].\n", postId);
    } else {
        //pernah vote
        char existingVoteType[NMax + 1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "DOWNVOTE") == 0) {
            printf("Anda sudah memberikan downvote pada post [%s].\n", postId);
        } else {
            //upvote ke downvote
            updateVote(votingIdx, "DOWNVOTE");
            updatePostVotes(postId, -1, 1);
            updateUserKarma(authorId, -2);
            printf("Vote Anda pada post [%s] berhasil diubah menjadi downvote.\n", postId);
        }
    }
}

void commandUndoVotePost() {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat memberikan vote.\n");
        return;
    }
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printf("Format salah. Gunakan: UPVOTE_POST <POST_ID>;\n");
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printf("Format salah. Gunakan: UNDO_VOTE_POST <POST_ID>;\n");
        return;
    }
    
    char currentUserId[NMax + 1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);

    int votingIdx = findVotingIndex(currentUserId, postId, "POST");
    if (votingIdx==-1) {
        printf("Anda belum memberikan vote pada post [%s].\n", postId);
        return;
    }
    
    char voteType[NMax+1];
    wordToString(voteType, VOTINGS[votingIdx].vote_type);
    const char *authorId = getPostAuthorId(postId);
    removeVote(votingIdx);
    
    if (strCmp(voteType, "UPVOTE") ==0) {
        updatePostVotes(postId, -1, 0);
        updateUserKarma(authorId, -1);
    } else {
        updatePostVotes(postId, 0, -1);
        updateUserKarma(authorId, 1);
    }
    printf("Vote Anda pada post [%s] berhasil dibatalkan.\n", postId);
}


// COMMENT VOTING COMMANDS
void commandUpvoteComment() {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat memberikan vote.\n");
        return;
    }
    
    //read post id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    //read comment id
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    int commentId = wordToInt(currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    
    //validasi: post exists
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("Post dengan ID %s tidak ditemukan!\n", postId);
        return;
    }
    
    //validasi: comment exists
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) {
        printf("Komentar #%d tidak ditemukan pada post [%s]!\n", commentId, postId);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    
    const char *authorId = getCommentAuthorId(postId, commentId);
    if (authorId != NULL && strCmp(currentUserId, authorId) == 0) {
        printf("Anda tidak dapat memberikan vote pada komentar Anda sendiri!\n");
        return;
    }
    
    char commentIdStr[20];
    commentIdToString(commentId, commentIdStr);
    int votingIdx = findVotingIndex(currentUserId, commentIdStr, "COMMENT");    
    if (votingIdx==-1) {
        addNewVote(currentUserId, commentIdStr, "COMMENT", "UPVOTE");
        updateCommentVotes(postId, commentId, 1, 0);
        updateUserKarma(authorId, 1);
        printf("Anda berhasil memberikan upvote pada komentar #%d di post [%s].\n", commentId, postId);
    } else {
        char existingVoteType[NMax+1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "UPVOTE") == 0) {
            printf("Anda sudah memberikan upvote pada komentar #%d di post [%s].\n", commentId, postId);
        } else {
            updateVote(votingIdx, "UPVOTE");
            updateCommentVotes(postId, commentId, 1, -1);
            updateUserKarma(authorId, 2);
            printf("Vote Anda pada komentar #%d di post [%s] berhasil diubah menjadi upvote.\n", commentId, postId);
        }
    }
}

void commandDownvoteComment() {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat memberikan vote.\n");
        return;
    }
    
    //read post id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    //read comment id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    int commentId = wordToInt(currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("Post dengan ID %s tidak ditemukan!\n", postId);
        return;
    }
    
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) {
        printf("Komentar #%d tidak ditemukan pada post [%s]!\n", commentId, postId);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    const char *authorId = getCommentAuthorId(postId, commentId);
    if (authorId != NULL && strCmp(currentUserId, authorId) == 0) {
        printf("Anda tidak dapat memberikan vote pada komentar Anda sendiri!\n");
        return;
    }

    char commentIdStr[20];
    commentIdToString(commentId, commentIdStr);
    int votingIdx = findVotingIndex(currentUserId, commentIdStr, "COMMENT");
    if (votingIdx==-1) {
        addNewVote(currentUserId, commentIdStr, "COMMENT", "DOWNVOTE");
        updateCommentVotes(postId, commentId, 0, 1);
        updateUserKarma(authorId, -1);
        printf("Anda berhasil memberikan downvote pada komentar #%d di post [%s].\n", commentId, postId);
    } else {
        char existingVoteType[NMax+1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "DOWNVOTE") == 0) {
            printf("Anda sudah memberikan downvote pada komentar #%d di post [%s].\n", commentId, postId);
        } else {
            updateVote(votingIdx, "DOWNVOTE");
            updateCommentVotes(postId, commentId, -1, 1);
            updateUserKarma(authorId, -2);
            printf("Vote Anda pada komentar #%d di post [%s] berhasil diubah menjadi downvote.\n", commentId, postId);
        }
    }
}

void commandUndoVoteComment() {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat memberikan vote.\n");
        return;
    }
    
    //read post id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    //read comment id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    int commentId = wordToInt(currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printf("Format salah. Gunakan: UPVOTE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("Post dengan ID %s tidak ditemukan!\n", postId);
        return;
    }
    
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) {
        printf("Komentar #%d tidak ditemukan pada post [%s]!\n", commentId, postId);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    char commentIdStr[20];
    commentIdToString(commentId, commentIdStr);
    int votingIdx = findVotingIndex(currentUserId, commentIdStr, "COMMENT");
    
    if (votingIdx==-1) {
        printf("Anda belum memberikan vote pada komentar #%d di post [%s].\n", commentId, postId);
        return;
    }
    
    char voteType[NMax + 1];
    wordToString(voteType, VOTINGS[votingIdx].vote_type);
    const char *authorId = getCommentAuthorId(postId, commentId);
    removeVote(votingIdx);
    
    if (strCmp(voteType, "UPVOTE")==0) {
        updateCommentVotes(postId, commentId, -1, 0);
        updateUserKarma(authorId, -1);
    } else {
        updateCommentVotes(postId, commentId, 0, -1);
        updateUserKarma(authorId, 1);
    }
    
    printf("Vote Anda pada komentar #%d di post [%s] berhasil dibatalkan.\n", commentId, postId);
}