#include "Comment.h"

Comment *getCommentById(const char *postId, int commentId)
{
    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        Comment *c = &COMMENTS[i];
        if (c->comment_id == commentId)
        {
            char currentPostId[NMax + 1];
            wordToString(currentPostId, c->post_id);
            if (strCmp(currentPostId, postId) == 0)
                return c;
        }
    }
    return NULL;
}

const char *getCommentAuthorId(const char *postId, int commentId)
{
    Comment *comment = getCommentById(postId, commentId);
    if (comment == NULL)
        return NULL;
    static char authorId[NMax + 1];
    wordToString(authorId, comment->author_id);

    return authorId;
}

Comment *getCommentPtr(int commentId, Word postIdWord)
{
    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        if (COMMENTS[i].comment_id == commentId && compareWord(COMMENTS[i].post_id, postIdWord) == 1)
            return &COMMENTS[i];
    }
    return NULL;
}

int generateNewCommentId()
{
    int maxId = 0;
    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        if (COMMENTS[i].comment_id > maxId)
        {
            maxId = COMMENTS[i].comment_id;
        }
    }
    return maxId + 1;
}

void deleteCommentAtIndex(int index)
{
    if (index < 0 || index >= COMMENT_COUNT)
        return;

    char commentIdStr[32];
    sprintf(commentIdStr, "%d", COMMENTS[index].comment_id);
    deleteVotingsByTarget(commentIdStr, "COMMENT");

    for (int i = index; i < COMMENT_COUNT - 1; i++)
    {
        COMMENTS[i] = COMMENTS[i + 1];
    }
    COMMENT_COUNT--;
    if (COMMENT_COUNT >= 0)
        initComment(&COMMENTS[COMMENT_COUNT]);
}

void deleteCommentRecursive(int commentId, const char *postId)
{
    int i = 0;
    while (i < COMMENT_COUNT)
    {
        if (COMMENTS[i].parent_comment_id == commentId)
        {
            char curPostId[NMax + 1];
            wordToString(curPostId, COMMENTS[i].post_id);
            if (strCmp(curPostId, postId) == 0)
            {
                int childId = COMMENTS[i].comment_id;
                deleteCommentRecursive(childId, postId);
                continue;
            }
        }
        i++;
    }

    int foundIdx = -1;
    for (int j = 0; j < COMMENT_COUNT; j++)
    {
        if (COMMENTS[j].comment_id == commentId)
        {
            char curPostId[NMax + 1];
            wordToString(curPostId, COMMENTS[j].post_id);
            if (strCmp(curPostId, postId) == 0)
            {
                foundIdx = j;
                break;
            }
        }
    }
    if (foundIdx != -1)
        deleteCommentAtIndex(foundIdx);
}

void commandAddComment()
{
    clearScreen();
    printBreadcrumb("Home > Add Comment");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                           %sADD COMMENT%s                              %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn())
    {
        printError("Authentication required");
        printf("You must be logged in to comment.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Invalid command format");
        printf("Post ID and Parent Comment ID are required.\n\n");
        printf("%sUsage:%s %sCOMMENT <post_id> <parent_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        printf("%sNote:%s Use %s-1%s for parent_id to comment directly on the post.\n", 
               DIM, RESET, BOLD_YELLOW, RESET);
        return;
    }

    char postId[NMax + 1];
    wordToString(postId, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Invalid command format");
        printf("Parent Comment ID is required.\n\n");
        printf("%sUsage:%s %sCOMMENT <post_id> <parent_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    int parentId = wordToInt(currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0)
    {
        while (currentWord.Length != 0)
            ADVWORD_INPUT();
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sCOMMENT <post_id> <parent_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Validating post", 6);

    Post *post = getPostById(postId);
    if (post == NULL)
    {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postId, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    if (getCommentById(postId, parentId) == NULL && parentId != -1)
    {
        printf("\n");
        printError("Parent comment not found");
        printf("No comment #%s%d%s exists on post %s%s%s\n\n", 
               BOLD_RED, parentId, RESET, BOLD_YELLOW, postId, RESET);
        printf("%sTip:%s Use %sVIEW_POST %s;%s to see existing comments.\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, postId, RESET);
        return;
    }

    printf("\n");
    printSectionHeader("", "COMMENT CONTENT");
    printf("\n");
    if (parentId == -1)
    {
        printf("%s %sCommenting on post:%s %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, postId, RESET);
    }
    else
    {
        printf("%s %sReplying to comment:%s %s#%d%s on post %s%s%s\n", 
               BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, parentId, RESET, BOLD_YELLOW, postId, RESET);
    }
    printSectionDivider();
    printf("\n");
    printf("%s└─▶ %sEnter your comment:%s\n", BOLD_CYAN, BOLD_WHITE, RESET);
    printf("    ");
    
    IgnoreNewline();
    Word contentW;
    readLineWord(&contentW);

    if (contentW.Length == 0)
    {
        printf("\n");
        printError("Invalid input");
        printf("Comment content cannot be empty.\n");
        return;
    }

    // ===========================
    // MODERASI KOMENTAR
    // ===========================
    printf("\n");
    spinnerAnimation("Checking content moderation", 8);
    
    char commentStr[512];
    wordToString(commentStr, contentW);

    char foundWordsC[1][MAX_WORD_LEN];
    int foundCountC = 0;

    if (CheckBlacklistedContent(commentStr, foundWordsC, &foundCountC))
    {
        printf("\n");
        printError("Content moderation failed");
        printf("Comment contains blacklisted word: %s\"%s\"%s\n", 
               BOLD_RED, foundWordsC[0], RESET);
        return;
    }

    printf("\n");
    printSuccess("Content approved");

    if (COMMENT_COUNT >= COMMENT_CAPACITY)
    {
        int newCap = COMMENT_CAPACITY * 2;
        if (newCap < 10)
            newCap = 10;
        if (!ensureCapacity((void **)&COMMENTS, &COMMENT_CAPACITY, sizeof(Comment), newCap))
        {
            printf("\n");
            printError("Memory allocation failed");
            printf("Could not allocate memory for new comment.\n");
            return;
        }
    }

    Comment newC;
    initComment(&newC);
    newC.comment_id = generateNewCommentId();
    stringToWord(&newC.post_id, postId);
    copyWord(&newC.author_id, USERS[CURRENT_USER_INDEX].user_id);
    newC.parent_comment_id = parentId;
    copyWord(&newC.content, contentW);
    newC.upvotes = 0;
    newC.downvotes = 0;

    COMMENTS[COMMENT_COUNT] = newC;
    COMMENT_COUNT++;

    printf("\n");
    spinnerAnimation("Posting comment", 8);
    printf("\n");
    printSuccess("Comment posted successfully");
    printf("%s╭─────────────────────────────────────────────╮%s\n", BOLD_CYAN, RESET);
    printf("%s│%s Comment ID : %s#%d%s\n", BOLD_CYAN, RESET, BOLD_WHITE, newC.comment_id, RESET);
    printf("%s│%s Post ID    : %s%s%s\n", BOLD_CYAN, RESET, BOLD_YELLOW, postId, RESET);
    if (parentId == -1)
        printf("%s│%s Type       : %sDirect comment%s\n", BOLD_CYAN, RESET, BOLD_GREEN, RESET);
    else
        printf("%s│%s Type       : %sReply to #%d%s\n", BOLD_CYAN, RESET, BOLD_GREEN, parentId, RESET);
    printf("%s╰─────────────────────────────────────────────╯%s\n", BOLD_CYAN, RESET);
}

void commandDeleteComment()
{
    clearScreen();
    printBreadcrumb("Home > Delete Comment");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                         %sDELETE COMMENT%s                            %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn())
    {
        printError("Authentication required");
        printf("You must be logged in to delete comments.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Invalid command format");
        printf("Post ID and Comment ID are required.\n\n");
        printf("%sUsage:%s %sDELETE_COMMENT <post_id> <comment_id>;%s\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        printf("%sExample:%s %sDELETE_COMMENT P001 5;%s\n", DIM, RESET, BOLD_WHITE, RESET);
        return;
    }
    char postId[NMax + 1];
    wordToString(postId, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Invalid command format");
        printf("Comment ID is required.\n\n");
        printf("%sUsage:%s %sDELETE_COMMENT <post_id> <comment_id>;%s\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }
    int commentId = wordToInt(currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0)
    {
        while (currentWord.Length != 0)
            ADVWORD_INPUT();
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sDELETE_COMMENT <post_id> <comment_id>;%s\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Validating post", 6);

    Post *post = getPostById(postId);
    if (post == NULL)
    {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postId, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Locating comment", 6);

    Comment *c = getCommentById(postId, commentId);
    if (c == NULL)
    {
        printf("\n");
        printError("Comment not found");
        printf("No comment #%s%d%s exists on post %s%s%s\n\n", 
               BOLD_RED, commentId, RESET, BOLD_YELLOW, postId, RESET);
        printf("%sTip:%s Use %sVIEW_POST %s;%s to see existing comments.\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, postId, RESET);
        return;
    }

    char currentUserId[NMax + 1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    const char *authorId = getCommentAuthorId(postId, commentId);
    if (authorId == NULL || strCmp(currentUserId, authorId) != 0)
    {
        printf("\n");
        printError("Authorization failed");
        printf("Only the comment author can delete this comment.\n\n");
        printf("%sComment ID:%s %s#%d%s on post %s%s%s\n", 
               BOLD_WHITE, RESET, BOLD_RED, commentId, RESET, BOLD_YELLOW, postId, RESET);
        return;
    }

    char contentStr[NMax + 1];
    wordToString(contentStr, c->content);
    
    printf("\n");
    printSectionHeader("", "CONFIRMATION REQUIRED");
    printf("\n");
    printf("%s %sPost ID:%s     %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_YELLOW, postId, RESET);
    printf("%s %sComment ID:%s  %s#%d%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, commentId, RESET);
    printf("%s %sContent:%s     %s%s%s\n", BOX_V, BOLD_WHITE, RESET, DIM, contentStr, RESET);
    printf("%s\n", BOX_V);
    printf("%s %sThis will permanently delete the comment and ALL its replies!%s\n", 
           BOX_V, BOLD_RED, RESET);
    printSectionDivider();
    printf("\n");
    printf("%s└─▶ %sAre you sure? (Y/N):%s ", BOLD_CYAN, BOLD_WHITE, RESET);

    STARTWORD_INPUT();
    char answer[8];
    wordToString(answer, currentWord);

    if (answer[0] != 'Y' && answer[0] != 'y')
    {
        printf("\n");
        printWarning("Deletion cancelled");
        printf("Comment #%s%d%s has been preserved.\n", BOLD_CYAN, commentId, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Deleting comment and replies", 10);

    deleteCommentRecursive(commentId, postId);

    printf("\n");
    printSuccess("Comment deleted successfully");
    printf("%s╭─────────────────────────────────────────────╮%s\n", BOLD_CYAN, RESET);
    printf("%s│%s Deleted Comment: %s#%d%s\n", BOLD_CYAN, RESET, BOLD_RED, commentId, RESET);
    printf("%s│%s From Post: %s%s%s\n", BOLD_CYAN, RESET, BOLD_YELLOW, postId, RESET);
    printf("%s│%s Replies removed: All nested replies        %s│%s\n", BOLD_CYAN, RESET, BOLD_CYAN, RESET);
    printf("%s╰─────────────────────────────────────────────╯%s\n", BOLD_CYAN, RESET);
}