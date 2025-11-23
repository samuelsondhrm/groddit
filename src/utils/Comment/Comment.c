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
    if (!isLoggedIn())
    {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat mengomentari.\n");
        return;
    }

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format salah. Gunakan: COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }

    char postId[NMax + 1];
    wordToString(postId, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format salah. Gunakan: COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    int parentId = wordToInt(currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0)
    {
        while (currentWord.Length != 0)
            ADVWORD_INPUT();
        printf("Format salah. Gunakan: COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }

    printf("Masukkan isi komentar:\n");
    IgnoreNewline();
    Word contentW;
    readLineWord(&contentW);
    if (contentW.Length == 0)
    {
        printf("Konten komentar tidak boleh kosong!\n");
        return;
    }

    Post *post = getPostById(postId);
    if (post == NULL)
    {
        printf("Post dengan ID %s tidak ditemukan!\n", postId);
        return;
    }

    if (getCommentById(postId, parentId) == NULL && parentId != -1)
    {
        printf("Komentar induk dengan ID #%d tidak ditemukan pada post [%s]!\n", parentId, postId);
        return;
    }

    if (COMMENT_COUNT >= COMMENT_CAPACITY)
    {
        int newCap = COMMENT_CAPACITY * 2;
        if (newCap < 10)
            newCap = 10;
        if (!ensureCapacity((void **)&COMMENTS, &COMMENT_CAPACITY, sizeof(Comment), newCap))
        {
            printf("Error: gagal allocating memori untuk komentar baru.\n");
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

    if (parentId == -1)
        printf("Komentar berhasil ditambahkan ke post %s.\n", postId);
    else
        printf("Balasan berhasil ditambahkan ke komentar #%d.\n", parentId);
}

void commandDeleteComment()
{
    if (!isLoggedIn())
    {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat menghapus komentar.\n");
        return;
    }

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format salah. Gunakan: DELETE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    char postId[NMax + 1];
    wordToString(postId, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format salah. Gunakan: DELETE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }
    int commentId = wordToInt(currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0)
    {
        while (currentWord.Length != 0)
            ADVWORD_INPUT();
        printf("Format salah. Gunakan: DELETE_COMMENT <POST_ID> <COMMENT_ID>;\n");
        return;
    }

    Post *post = getPostById(postId);
    if (post == NULL)
    {
        printf("Post dengan ID %s tidak ditemukan!\n", postId);
        return;
    }

    Comment *c = getCommentById(postId, commentId);
    if (c == NULL)
    {
        printf("Komentar #%d tidak ditemukan pada post [%s]!\n", commentId, postId);
        return;
    }

    char currentUserId[NMax + 1];
    wordToString(currentUserId, USERS[CURRENT_USER_INDEX].user_id);
    const char *authorId = getCommentAuthorId(postId, commentId);
    if (authorId == NULL || strCmp(currentUserId, authorId) != 0)
    {
        printf("Anda bukan pembuat komentar ini! Hanya pembuat yang dapat menghapus komentar.\n");
        return;
    }

    char contentStr[NMax + 1];
    wordToString(contentStr, c->content);
    printf("Apakah Anda yakin ingin menghapus komentar #%d: \"%s\"? (Y/N)\n", commentId, contentStr);

    STARTWORD_INPUT();
    char answer[8];
    wordToString(answer, currentWord);

    if (answer[0] != 'Y' && answer[0] != 'y')
    {
        printf("Penghapusan komentar dibatalkan.\n");
        return;
    }

    deleteCommentRecursive(commentId, postId);

    printf("Komentar #%d di post [%s] berhasil dihapus.\n", commentId, postId);
}