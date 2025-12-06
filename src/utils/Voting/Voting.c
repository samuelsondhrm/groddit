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
    clearScreen();
    printBreadcrumb("Home > Upvote Post");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                          %sUPVOTE POST%s                              %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to vote.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    ADVWORD_INPUT();
    
    if (currentWord.Length == 0) {
        printError("Invalid command format");
        printf("Post ID is required.\n\n");
        printf("%sUsage:%s %sUPVOTE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        printf("%sExample:%s %sUPVOTE_POST P001;%s\n", DIM, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    char postId[NMax + 1];
    wordToString(postId, currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sUPVOTE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Validating post", 6);
    
    //validasi
    Post *post = getPostById(postId);
    if (post == NULL) {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postId, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    
    const char *authorId = getPostAuthorId(postId);
    if (authorId!=NULL && strCmp(currentUserId, authorId) == 0) {
        printf("\n");
        printError("Self-voting not allowed");
        printf("You cannot vote on your own post.\n\n");
        printf("%sPost ID:%s %s%s%s\n", BOLD_WHITE, RESET, BOLD_YELLOW, postId, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Processing vote", 6);
    
    int votingIdx = findVotingIndex(currentUserId, postId, "POST");
    
    if (votingIdx==-1) {
        addNewVote(currentUserId, postId, "POST", "UPVOTE");
        updatePostVotes(postId, 1, 0);
        updateUserKarma(authorId, 1);
        printf("\n");
        printSuccess("Upvote recorded");
        printf("Post %s%s%s received your %supvote%s\n", BOLD_CYAN, postId, RESET, GREEN, RESET);
    } else {
        char existingVoteType[NMax+1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "UPVOTE") == 0) {
            printf("\n");
            printWarning("Already upvoted");
            printf("You have already upvoted post %s%s%s\n", BOLD_CYAN, postId, RESET);
        } else {
            updateVote(votingIdx, "UPVOTE");
            updatePostVotes(postId, 1, -1);
            updateUserKarma(authorId, 2);
            printf("\n");
            printSuccess("Vote changed");
            printf("Your vote on post %s%s%s changed to %supvote%s\n", BOLD_CYAN, postId, RESET, GREEN, RESET);
        }
    }
}

void commandDownvotePost() {
    clearScreen();
    printBreadcrumb("Home > Downvote Post");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                         %sDOWNVOTE POST%s                             %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to vote.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printError("Invalid command format");
        printf("Post ID is required.\n\n");
        printf("%sUsage:%s %sDOWNVOTE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sDOWNVOTE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Validating post", 6);
    
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postId, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    
    const char *authorId = getPostAuthorId(postId);
    if (authorId != NULL && strCmp(currentUserId, authorId) == 0) {
        printf("\n");
        printError("Self-voting not allowed");
        printf("You cannot vote on your own post.\n\n");
        printf("%sPost ID:%s %s%s%s\n", BOLD_WHITE, RESET, BOLD_YELLOW, postId, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Processing vote", 6);
    
    int votingIdx = findVotingIndex(currentUserId, postId, "POST");
    if (votingIdx ==-1) {
        //case blm pernah vote
        addNewVote(currentUserId, postId, "POST", "DOWNVOTE");
        updatePostVotes(postId, 0, 1);
        updateUserKarma(authorId, -1);
        printf("\n");
        printSuccess("Downvote recorded");
        printf("Post %s%s%s received your %sdownvote%s\n", BOLD_CYAN, postId, RESET, RED, RESET);
    } else {
        //pernah vote
        char existingVoteType[NMax + 1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "DOWNVOTE") == 0) {
            printf("\n");
            printWarning("Already downvoted");
            printf("You have already downvoted post %s%s%s\n", BOLD_CYAN, postId, RESET);
        } else {
            //upvote ke downvote
            updateVote(votingIdx, "DOWNVOTE");
            updatePostVotes(postId, -1, 1);
            updateUserKarma(authorId, -2);
            printf("\n");
            printSuccess("Vote changed");
            printf("Your vote on post %s%s%s changed to %sdownvote%s\n", BOLD_CYAN, postId, RESET, RED, RESET);
        }
    }
}

void commandUndoVotePost() {
    clearScreen();
    printBreadcrumb("Home > Undo Vote Post");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                         %sUNDO VOTE POST%s                            %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to undo votes.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printError("Invalid command format");
        printf("Post ID is required.\n\n");
        printf("%sUsage:%s %sUNDO_VOTE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sUNDO_VOTE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Checking vote status", 6);
    
    char currentUserId[NMax + 1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);

    int votingIdx = findVotingIndex(currentUserId, postId, "POST");
    if (votingIdx==-1) {
        printf("\n");
        printWarning("No vote found");
        printf("You haven't voted on post %s%s%s yet.\n", BOLD_CYAN, postId, RESET);
        return;
    }
    
    char voteType[NMax+1];
    wordToString(voteType, VOTINGS[votingIdx].vote_type);
    const char *authorId = getPostAuthorId(postId);
    
    printf("\n");
    spinnerAnimation("Removing vote", 6);
    
    removeVote(votingIdx);
    
    if (strCmp(voteType, "UPVOTE") ==0) {
        updatePostVotes(postId, -1, 0);
        updateUserKarma(authorId, -1);
    } else {
        updatePostVotes(postId, 0, -1);
        updateUserKarma(authorId, 1);
    }
    printf("\n");
    printSuccess("Vote removed");
    printf("Your %s%s%s vote on post %s%s%s has been cancelled.\n", 
           BOLD_YELLOW, voteType, RESET, BOLD_CYAN, postId, RESET);
}


// COMMENT VOTING COMMANDS
void commandUpvoteComment() {
    clearScreen();
    printBreadcrumb("Home > Upvote Comment");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                        %sUPVOTE COMMENT%s                            %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to vote.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    //read post id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printError("Invalid command format");
        printf("Post ID and Comment ID are required.\n\n");
        printf("%sUsage:%s %sUPVOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    //read comment id
    ADVWORD_INPUT();
    if (currentWord.Length == 0) {
        printError("Invalid command format");
        printf("Comment ID is required.\n\n");
        printf("%sUsage:%s %sUPVOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    int commentId = wordToInt(currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sUPVOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Validating post and comment", 6);
    
    //validasi: post exists
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postId, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    //validasi: comment exists
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) {
        printf("\n");
        printError("Comment not found");
        printf("No comment #%s%d%s exists on post %s%s%s\n\n", 
               BOLD_RED, commentId, RESET, BOLD_YELLOW, postId, RESET);
        printf("%sTip:%s Use %sVIEW_POST %s;%s to see comments.\n", BOLD_CYAN, RESET, BOLD_WHITE, postId, RESET);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    
    const char *authorId = getCommentAuthorId(postId, commentId);
    if (authorId != NULL && strCmp(currentUserId, authorId) == 0) {
        printf("\n");
        printError("Self-voting not allowed");
        printf("You cannot vote on your own comment.\n\n");
        printf("%sComment:%s #%s%d%s on post %s%s%s\n", 
               BOLD_WHITE, RESET, BOLD_CYAN, commentId, RESET, BOLD_YELLOW, postId, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Processing vote", 6);
    
    char commentIdStr[20];
    commentIdToString(commentId, commentIdStr);
    int votingIdx = findVotingIndex(currentUserId, commentIdStr, "COMMENT");    
    if (votingIdx==-1) {
        addNewVote(currentUserId, commentIdStr, "COMMENT", "UPVOTE");
        updateCommentVotes(postId, commentId, 1, 0);
        updateUserKarma(authorId, 1);
        printf("\n");
        printSuccess("Upvote recorded");
        printf("Comment #%s%d%s on post %s%s%s received your %supvote%s\n", 
               BOLD_CYAN, commentId, RESET, BOLD_CYAN, postId, RESET, GREEN, RESET);
    } else {
        char existingVoteType[NMax+1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "UPVOTE") == 0) {
            printf("\n");
            printWarning("Already upvoted");
            printf("You have already upvoted comment #%s%d%s\n", BOLD_CYAN, commentId, RESET);
        } else {
            updateVote(votingIdx, "UPVOTE");
            updateCommentVotes(postId, commentId, 1, -1);
            updateUserKarma(authorId, 2);
            printf("\n");
            printSuccess("Vote changed");
            printf("Your vote on comment #%s%d%s changed to %supvote%s\n", 
                   BOLD_CYAN, commentId, RESET, GREEN, RESET);
        }
    }
}

void commandDownvoteComment() {
    clearScreen();
    printBreadcrumb("Home > Downvote Comment");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                       %sDOWNVOTE COMMENT%s                           %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to vote.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    //read post id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printError("Invalid command format");
        printf("Post ID and Comment ID are required.\n\n");
        printf("%sUsage:%s %sDOWNVOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    //read comment id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printError("Invalid command format");
        printf("Comment ID is required.\n\n");
        printf("%sUsage:%s %sDOWNVOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    int commentId = wordToInt(currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sDOWNVOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Validating post and comment", 6);
    
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postId, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) {
        printf("\n");
        printError("Comment not found");
        printf("No comment #%s%d%s exists on post %s%s%s\n\n", 
               BOLD_RED, commentId, RESET, BOLD_YELLOW, postId, RESET);
        printf("%sTip:%s Use %sVIEW_POST %s;%s to see comments.\n", BOLD_CYAN, RESET, BOLD_WHITE, postId, RESET);
        return;
    }
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    const char *authorId = getCommentAuthorId(postId, commentId);
    if (authorId != NULL && strCmp(currentUserId, authorId) == 0) {
        printf("\n");
        printError("Self-voting not allowed");
        printf("You cannot vote on your own comment.\n\n");
        printf("%sComment:%s #%s%d%s on post %s%s%s\n", 
               BOLD_WHITE, RESET, BOLD_CYAN, commentId, RESET, BOLD_YELLOW, postId, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Processing vote", 6);

    char commentIdStr[20];
    commentIdToString(commentId, commentIdStr);
    int votingIdx = findVotingIndex(currentUserId, commentIdStr, "COMMENT");
    if (votingIdx==-1) {
        addNewVote(currentUserId, commentIdStr, "COMMENT", "DOWNVOTE");
        updateCommentVotes(postId, commentId, 0, 1);
        updateUserKarma(authorId, -1);
        printf("\n");
        printSuccess("Downvote recorded");
        printf("Comment #%s%d%s on post %s%s%s received your %sdownvote%s\n", 
               BOLD_CYAN, commentId, RESET, BOLD_CYAN, postId, RESET, RED, RESET);
    } else {
        char existingVoteType[NMax+1];
        wordToString(existingVoteType, VOTINGS[votingIdx].vote_type);
        
        if (strCmp(existingVoteType, "DOWNVOTE") == 0) {
            printf("\n");
            printWarning("Already downvoted");
            printf("You have already downvoted comment #%s%d%s\n", BOLD_CYAN, commentId, RESET);
        } else {
            updateVote(votingIdx, "DOWNVOTE");
            updateCommentVotes(postId, commentId, -1, 1);
            updateUserKarma(authorId, -2);
            printf("\n");
            printSuccess("Vote changed");
            printf("Your vote on comment #%s%d%s changed to %sdownvote%s\n", 
                   BOLD_CYAN, commentId, RESET, RED, RESET);
        }
    }
}

void commandUndoVoteComment() {
    clearScreen();
    printBreadcrumb("Home > Undo Vote Comment");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                       %sUNDO VOTE COMMENT%s                          %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn()) {
        printError("Authentication required");
        printf("You must be logged in to undo votes.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    //read post id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printError("Invalid command format");
        printf("Post ID and Comment ID are required.\n\n");
        printf("%sUsage:%s %sUNDO_VOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    char postId[NMax+1];
    wordToString(postId, currentWord);
    
    //read comment id
    ADVWORD_INPUT();
    if (currentWord.Length==0) {
        printError("Invalid command format");
        printf("Comment ID is required.\n\n");
        printf("%sUsage:%s %sUNDO_VOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    int commentId = wordToInt(currentWord);
    
    ADVWORD_INPUT();
    if (currentWord.Length!=0) {
        while (currentWord.Length!=0) {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sUNDO_VOTE_COMMENT <post_id> <comment_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Validating post and comment", 6);
    
    Post *post = getPostById(postId);
    if (post==NULL) {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postId, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    
    Comment *comment = getCommentById(postId, commentId);
    if (comment==NULL) {
        printf("\n");
        printError("Comment not found");
        printf("No comment #%s%d%s exists on post %s%s%s\n\n", 
               BOLD_RED, commentId, RESET, BOLD_YELLOW, postId, RESET);
        printf("%sTip:%s Use %sVIEW_POST %s;%s to see comments.\n", BOLD_CYAN, RESET, BOLD_WHITE, postId, RESET);
        return;
    }
    
    printf("\n");
    spinnerAnimation("Checking vote status", 6);
    
    char currentUserId[NMax+1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    char commentIdStr[20];
    commentIdToString(commentId, commentIdStr);
    int votingIdx = findVotingIndex(currentUserId, commentIdStr, "COMMENT");
    
    if (votingIdx==-1) {
        printf("\n");
        printWarning("No vote found");
        printf("You haven't voted on comment #%s%d%s yet.\n", BOLD_CYAN, commentId, RESET);
        return;
    }
    
    char voteType[NMax + 1];
    wordToString(voteType, VOTINGS[votingIdx].vote_type);
    const char *authorId = getCommentAuthorId(postId, commentId);
    
    printf("\n");
    spinnerAnimation("Removing vote", 6);
    
    removeVote(votingIdx);
    
    if (strCmp(voteType, "UPVOTE")==0) {
        updateCommentVotes(postId, commentId, -1, 0);
        updateUserKarma(authorId, -1);
    } else {
        updateCommentVotes(postId, commentId, 0, -1);
        updateUserKarma(authorId, 1);
    }
    
    printf("\n");
    printSuccess("Vote removed");
    printf("Your %s%s%s vote on comment #%s%d%s has been cancelled.\n", 
           BOLD_YELLOW, voteType, RESET, BOLD_CYAN, commentId, RESET);
}