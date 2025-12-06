#include "Post.h"

int findSubgrodditIndexByName(const char *subName)
{
    // Mencari index Subgroddit berdasarkan nama
    char nameBuf[256];
    int idx = 0;
    Node *p = SUBGRODDITS.head;
    while (p != NULL)
    {
        if (p->element.type == TYPE_SUBGRODDIT)
        {
            SubGroddit s = p->element.data.subgroddit;
            wordToString(nameBuf, s.name);
            if (strCmp(nameBuf, subName) == 0)
            {
                return idx;
            }
            idx++;
        }
        p = p->next;
    }
    return -1;
}

int findPostIndexById(const char *postId)
{
    // Mencari index Post berdasarkan ID
    char idBuf[256];
    int idx = 0;
    Node *p = POSTS.head;
    while (p != NULL)
    {
        if (p->element.type == TYPE_POST)
        {
            Post post = p->element.data.post;
            wordToString(idBuf, post.post_id);
            if (strCmp(idBuf, postId) == 0)
                return idx;
            idx++;
        }
        p = p->next;
    }
    return -1;
}

void generatePostID(char *id, int num)
{
    // Membuat ID Post dengan format "PXYZ"
    id[0] = 'P';
    int hundreds = (num / 100) % 10;
    int tens = (num / 10) % 10;
    int ones = num % 10;
    id[1] = '0' + hundreds;
    id[2] = '0' + tens;
    id[3] = '0' + ones;
    id[4] = '\0';
}

int getPostNumberFromWord(Word postId)
{
    // Mengambil angka dari post_id dengan format "PXYZ" (3 digit).
    char buf[16];
    wordToString(buf, postId);

    if (buf[0] != 'P')
        return 0;
    if (buf[1] < '0' || buf[1] > '9')
        return 0;
    if (buf[2] < '0' || buf[2] > '9')
        return 0;
    if (buf[3] < '0' || buf[3] > '9')
        return 0;

    int hundreds = buf[1] - '0';
    int tens = buf[2] - '0';
    int ones = buf[3] - '0';
    return hundreds * 100 + tens * 10 + ones;
}

int getNextPostNumberMex()
{
    // Menghitung MEX (minimum excluded) dari nomor post yang sudah ada.
    int maxCheck = POST_COUNT + 2; // cukup untuk menjamin ada MEX
    int used[maxCheck];
    for (int i = 0; i < maxCheck; i++)
        used[i] = 0;

    Node *p = POSTS.head;
    while (p != NULL)
    {
        if (p->element.type == TYPE_POST)
        {
            Post post = p->element.data.post;
            int num = getPostNumberFromWord(post.post_id);
            if (num >= 1 && num < maxCheck)
            {
                used[num] = 1;
            }
        }
        p = p->next;
    }

    int mex = 1;
    while (mex < maxCheck && used[mex])
        mex++;

    return mex;
}
Post *getPostById(const char *postId)
{
    Node *p = POSTS.head;
    while (p != NULL)
    {
        if (p->element.type == TYPE_POST)
        {
            Post *post = &(p->element.data.post);
            char currentPostId[NMax + 1];
            wordToString(currentPostId, post->post_id);
            if (strCmp(currentPostId, postId) == 0)
                return post;
        }
        p = p->next;
    }
    return NULL;
}

const char *getPostAuthorId(const char *postId)
{
    Post *post = getPostById(postId);
    if (post == NULL)
        return NULL;
    static char authorId[NMax + 1];
    wordToString(authorId, post->author_id);
    return authorId;
}

void commandPost()
{
    if (!isLoggedIn())
    {
        printError("Not logged in");
        printf("You must be logged in to create posts.\n");
        printf("\nUse %sLOGIN;%s to sign in to your account.\n", BOLD_WHITE, RESET);
        return;
    }

    clearScreen();
    printBreadcrumb("Main Menu > Create Post");

    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s                          CREATE NEW POST                            %s\n", DBOX_V, DBOX_V);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    printSectionHeader("", "SUBGRODDIT SELECTION");
    printf("\n%s %sEnter subgroddit name (must start with r/):%s\n", BOX_V, BOLD_WHITE, RESET);
    printf("%s %s└─▶%s ", BOX_V, BOLD_CYAN, RESET);

    Word subNameWord;
    readLineWord(&subNameWord);

    if (subNameWord.Length == 0)
    {
        printf("\n");
        printError("Empty subgroddit name");
        printf("Subgroddit name cannot be empty!\n");
        return;
    }

    char subNameStr[256];
    wordToString(subNameStr, subNameWord);

    int subIdx = findSubgrodditIndexByName(subNameStr);
    if (subIdx == -1)
    {
        printf("\n");
        printError("Subgroddit not found");
        printf("Subgroddit %s%s%s doesn't exist.\n", BOLD_YELLOW, subNameStr, RESET);
        printf("\n%sTip:%s Use %sCREATE_SUBGRODDIT;%s to create it first.\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    // Ambil Subgroddit ke-subIdx dari List
    Node *p = SUBGRODDITS.head;
    int idx = 0;
    while (p != NULL && idx < subIdx)
    {
        if (p->element.type == TYPE_SUBGRODDIT)
        {
            idx++;
            if (idx > subIdx)
                break;
        }
        p = p->next;
    }

    Word subIdWord;
    if (p != NULL && p->element.type == TYPE_SUBGRODDIT)
    {
        subIdWord = p->element.data.subgroddit.subgroddit_id;
    }

    printf("\n");
    printSectionDivider();
    printSectionHeader("", "POST CONTENT");
    printf("\n%s %sEnter post title:%s\n", BOX_V, BOLD_WHITE, RESET);
    printf("%s %s└─▶%s ", BOX_V, BOLD_CYAN, RESET);
    
    Word titleW;
    readLineWord(&titleW);

    if (titleW.Length == 0)
    {
        printf("\n");
        printError("Empty title");
        printf("Post title cannot be empty!\n");
        return;
    }

    printf("\n%s %sEnter post content:%s\n", BOX_V, BOLD_WHITE, RESET);
    printf("%s %s└─▶%s ", BOX_V, BOLD_CYAN, RESET);
    
    Word contentW;
    readLineWord(&contentW);

    if (contentW.Length == 0)
    {
        printf("\n");
        printError("Empty content");
        printf("Post content cannot be empty!\n");
        return;
    }

    // ===========================
    // MODERASI KONTEN POST
    // ===========================
    printf("\n");
    spinnerAnimation("Checking content moderation", 8);
    
    char titleStr[256], contentStr[512];
    wordToString(titleStr, titleW);
    wordToString(contentStr, contentW);

    char foundWords[MAX_FOUND][MAX_WORD_LEN];
    int foundCount = 0;

    if (CheckBlacklistedContent(titleStr, foundWords, &foundCount) ||
        CheckBlacklistedContent(contentStr, foundWords, &foundCount))
    {
        printf("\n");
        printError("Content moderation failed");
        printf("Post contains blacklisted words: %s", BOLD_RED);
        for (int i = 0; i < foundCount; i++)
        {
            printf("%s", foundWords[i]);
            if (i + 1 < foundCount)
                printf(", ");
        }
        printf("%s\n", RESET);
        return;
    }

    printf("\n");
    printSuccess("Content approved");

    Post newPost;
    initPost(&newPost);

    char postIdStr[8];
    int nextNum = getNextPostNumberMex();
    generatePostID(postIdStr, nextNum);

    Word postIdWord;
    stringToWord(&postIdWord, postIdStr);

    copyWord(&newPost.post_id, postIdWord);
    copyWord(&newPost.subgroddit_id, subIdWord);
    copyWord(&newPost.author_id, USERS[CURRENT_USER_INDEX].user_id);
    copyWord(&newPost.title, titleW);
    copyWord(&newPost.content, contentW);
    newPost.created_at = time(NULL);
    newPost.upvotes = 0;
    newPost.downvotes = 0;

    insertLastList(&POSTS, makePostElement(newPost));
    POST_COUNT++;

    printf("\n");
    spinnerAnimation("Creating post", 8);
    printf("\n");
    printSuccess("Post created successfully");
    printf("%s╭─────────────────────────────────────────────╮%s\n", BOLD_CYAN, RESET);
    printf("%s│%s Post ID       : %s%s%s\n", BOLD_CYAN, RESET, BOLD_WHITE, postIdStr, RESET);
    printf("%s│%s Subgroddit   : %s%s%s\n", BOLD_CYAN, RESET, BOLD_YELLOW, subNameStr, RESET);
    printf("%s╰─────────────────────────────────────────────╯%s\n", BOLD_CYAN, RESET);
}

void printPostHeader(const Post *p)
{
    char subIdStr[256];
    char titleStr[256];
    char contentStr[256];
    char createdStr[256];
    char authorIdStr[256];

    wordToString(subIdStr, p->subgroddit_id);
    wordToString(titleStr, p->title);
    wordToString(contentStr, p->content);
    timeToStr(createdStr, p->created_at);
    wordToString(authorIdStr, p->author_id);

    int authorIndex = -1;
    for (int i = 0; i < USER_COUNT; i++)
    {
        if (compareWord(USERS[i].user_id, p->author_id) != 0)
        {
            authorIndex = i;
            break;
        }
    }

    char authorUsername[256];
    if (authorIndex != -1)
    {
        wordToString(authorUsername, USERS[authorIndex].username);
    }
    else
    {
        wordToString(authorUsername, p->author_id);
    }

    printf("[%s] %s\n", subIdStr, titleStr);
    printf("oleh: %s (%s)\n", authorUsername, createdStr);
    printf("=======================================\n");
    printf("%s\n", contentStr);
    printf("%c ↑%d %c ↓%d\n", 24, p->upvotes, 25, p->downvotes);
}

void buildPostTree(const Post *p, PostTree *T)
{
    CreatePostTree(T, *p);

    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        if (compareWord(COMMENTS[i].post_id, p->post_id) != 0)
        {
            Comment c = COMMENTS[i];
            AddComment(T, c.parent_comment_id, c);
        }
    }
}

void commandViewPost()
{
    ADVWORD_INPUT();

    if (currentWord.Length == 0)
    {
        // Tidak ada ID setelah VIEW_POST
        printError("Missing post ID");
        printf("Use: %sVIEW_POST <ID>;%s\n", BOLD_WHITE, RESET);
        return;
    }

    char postIdStr[256];
    wordToString(postIdStr, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0)
    {
        // Ada argumen berlebih setelah ID
        while (currentWord.Length != 0)
        {
            ADVWORD_INPUT();
        }
        printError("Too many arguments");
        printf("Use: %sVIEW_POST <ID>;%s (without additional arguments)\n", BOLD_WHITE, RESET);
        return;
    }

    int idx = findPostIndexById(postIdStr);
    if (idx == -1)
    {
        printError("Post not found");
        printf("Post with ID %s%s%s doesn't exist.\n", BOLD_YELLOW, postIdStr, RESET);
        return;
    }

    Node *node = POSTS.head;
    int curIdx = 0;
    while (node != NULL && curIdx < idx)
    {
        node = node->next;
        curIdx++;
    }
    if (node == NULL || node->element.type != TYPE_POST)
    {
        return;
    }

    Post *p = &node->element.data.post;

    // Clear screen and show header
    clearScreen();
    char breadcrumbPath[512];
    char subName[256];
    getSubgrodditName(p->subgroddit_id, subName);
    sprintf(breadcrumbPath, "Main Menu > %s > Post", subName);
    printBreadcrumb(breadcrumbPath);

    // Get post details
    char titleStr[256];
    char contentStr[512];
    char createdStr[256];
    char authorIdStr[256];

    wordToString(titleStr, p->title);
    wordToString(contentStr, p->content);
    timeToStr(createdStr, p->created_at);
    wordToString(authorIdStr, p->author_id);

    int authorIndex = -1;
    for (int i = 0; i < USER_COUNT; i++)
    {
        if (compareWord(USERS[i].user_id, p->author_id) != 0)
        {
            authorIndex = i;
            break;
        }
    }

    char authorUsername[256];
    if (authorIndex != -1)
    {
        wordToString(authorUsername, USERS[authorIndex].username);
    }
    else
    {
        wordToString(authorUsername, p->author_id);
    }

    printf("\n");
    printf("%s", BOLD_CYAN);
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s                        📝  POST : %s%-25s%s    %s\n", 
           DBOX_V, BOLD_WHITE, postIdStr, RESET, DBOX_V);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    printSectionHeader("", "POST DETAILS");
    printf("\n");
    printf("%s %sSubgroddit:%s   %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_MAGENTA, subName, RESET);
    printf("%s %sTitle:%s        %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_WHITE, titleStr, RESET);
    printf("%s %sAuthor:%s       %s@%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, authorUsername, RESET);
    printf("%s %sPosted:%s       %s%s%s\n", BOX_V, BOLD_WHITE, RESET, DIM, createdStr, RESET);
    
    printSectionDivider();
    printSectionHeader("", "CONTENT");
    printf("\n");
    printf("%s %s%s%s\n", BOX_V, RESET, contentStr, RESET);
    
    printSectionDivider();
    printSectionHeader("", "VOTING");
    printf("\n");
    printf("%s %s↑ Upvotes:%s   %s%d%s\n", BOX_V, GREEN, RESET, BOLD_WHITE, p->upvotes, RESET);
    printf("%s %s↓ Downvotes:%s %s%d%s\n", BOX_V, RED, RESET, BOLD_WHITE, p->downvotes, RESET);

    printSectionDivider();

    PostTree T;
    buildPostTree(p, &T);

    printSectionHeader("", "COMMENTS");
    printf("\n");
    PrintPostTree(&T);
    
    printSectionDivider();
    printf("\n");
    printInfo("Post loaded successfully");
}

void deleteCommentsForPost(const Word postIdWord)
{
    for (int i = COMMENT_COUNT - 1; i >= 0; i--)
    {
        if (compareWord(COMMENTS[i].post_id, postIdWord) != 0)
        {
            for (int j = i; j < COMMENT_COUNT - 1; j++)
            {
                COMMENTS[j] = COMMENTS[j + 1];
            }
            COMMENT_COUNT--;
        }
    }
}

void commandDeletePost()
{
    clearScreen();
    printBreadcrumb("Home > Delete Post");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                           %sDELETE POST%s                             %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    if (!isLoggedIn())
    {
        ADVWORD_INPUT();
        while (currentWord.Length != 0)
        {
            ADVWORD_INPUT();
        }

        printError("Authentication required");
        printf("You must be logged in to delete posts.\n\n");
        printf("%sTip:%s Use %sLOGIN;%s to access your account.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    ADVWORD_INPUT();

    if (currentWord.Length == 0)
    {
        printError("Invalid command format");
        printf("Post ID is required.\n\n");
        printf("%sUsage:%s %sDELETE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        printf("%sExample:%s %sDELETE_POST P123;%s\n", DIM, RESET, BOLD_WHITE, RESET);
        return;
    }

    char postIdStr[256];
    wordToString(postIdStr, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0)
    {
        while (currentWord.Length != 0)
        {
            ADVWORD_INPUT();
        }
        printError("Invalid command format");
        printf("Too many arguments provided.\n\n");
        printf("%sUsage:%s %sDELETE_POST <post_id>;%s\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Locating post", 6);

    int idx = findPostIndexById(postIdStr);
    if (idx == -1)
    {
        printf("\n");
        printError("Post not found");
        printf("No post exists with ID: %s%s%s\n\n", BOLD_RED, postIdStr, RESET);
        printf("%sTip:%s Use %sSHOW_FEED;%s to view available posts.\n", BOLD_CYAN, RESET, BOLD_WHITE, RESET);
        return;
    }

    Node *node = POSTS.head;
    int curIdx = 0;
    while (node != NULL && curIdx < idx)
    {
        node = node->next;
        curIdx++;
    }
    if (node == NULL || node->element.type != TYPE_POST)
    {
        return;
    }

    Post *p = &node->element.data.post;

    /*
     * Hanya pembuat post yang boleh menghapus postingan.
     * Jika author_id != user yang sedang login, tolak operasi.
     */
    if (compareWord(p->author_id, USERS[CURRENT_USER_INDEX].user_id) == 0)
    {
        printf("\n");
        printError("Authorization failed");
        printf("Only the post author can delete this post.\n\n");
        printf("%sPost ID:%s %s%s%s\n", BOLD_WHITE, RESET, BOLD_RED, postIdStr, RESET);
        return;
    }

    char titleStr[256];
    wordToString(titleStr, p->title);

    printf("\n");
    printSectionHeader("", "CONFIRMATION REQUIRED");
    printf("\n");
    printf("%s %sPost ID:%s  %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_YELLOW, postIdStr, RESET);
    printf("%s %sTitle:%s    %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_WHITE, titleStr, RESET);
    printf("%s\n", BOX_V);
    printf("%s %sThis will permanently delete the post and ALL its comments!%s\n", BOX_V, BOLD_RED, RESET);
    printSectionDivider();
    printf("\n");
    printf("%s└─▶ %sAre you sure? (Y/N):%s ", BOLD_CYAN, BOLD_WHITE, RESET);

    STARTWORD_INPUT();
    char answer[8];
    wordToString(answer, currentWord);
    IgnoreNewline();

    if (answer[0] != 'Y' && answer[0] != 'y')
    {
        printf("\n");
        printWarning("Deletion cancelled");
        printf("Post %s%s%s has been preserved.\n", BOLD_CYAN, postIdStr, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Deleting post and comments", 10);

    deleteCommentsForPost(p->post_id);

    ListElement deletedElem;
    deleteAtList(&POSTS, idx, &deletedElem);
    POST_COUNT--;

    printf("\n");
    printSuccess("Post deleted successfully");
    printf("%s╭─────────────────────────────────────────────╮%s\n", BOLD_CYAN, RESET);
    printf("%s│%s Deleted Post: %s%s%s\n", BOLD_CYAN, RESET, BOLD_RED, postIdStr, RESET);
    printf("%s│%s Comments removed: All associated comments  %s│%s\n", BOLD_CYAN, RESET, BOLD_CYAN, RESET);
    printf("%s╰─────────────────────────────────────────────╯%s\n", BOLD_CYAN, RESET);
}
