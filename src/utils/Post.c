#include "header/Post.h"

int findSubgrodditIndexByName(const char *subName) {
    // Mencari index Subgroddit berdasarkan nama
    char nameBuf[256];
    int idx = 0;
    Node *p = SUBGRODDITS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_SUBGRODDIT) {
            SubGroddit s = p->element.data.subgroddit;
            wordToString(nameBuf, s.name);
            if (strCmp(nameBuf, subName) == 0) {
                return idx;
            }
            idx++;
        }
        p = p->next;
    }
    return -1;
}

int findPostIndexById(const char *postId) {
    // Mencari index Post berdasarkan ID
    char idBuf[256];
    int idx = 0;
    Node *p = POSTS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_POST) {
            Post post = p->element.data.post;
            wordToString(idBuf, post.post_id);
            if (strCmp(idBuf, postId) == 0) return idx;
            idx++;
        }
        p = p->next;
    }
    return -1;
}

void generatePostID(char *id, int num) {
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

int getPostNumberFromWord(Word postId) {
    // Mengambil angka dari post_id dengan format "PXYZ" (3 digit).
    char buf[16];
    wordToString(buf, postId);

    if (buf[0] != 'P') return 0;
    if (buf[1] < '0' || buf[1] > '9') return 0;
    if (buf[2] < '0' || buf[2] > '9') return 0;
    if (buf[3] < '0' || buf[3] > '9') return 0;

    int hundreds = buf[1] - '0';
    int tens     = buf[2] - '0';
    int ones     = buf[3] - '0';
    return hundreds * 100 + tens * 10 + ones;
}

int getNextPostNumberMex() {
    // Menghitung MEX (minimum excluded) dari nomor post yang sudah ada.
    int maxCheck = POST_COUNT + 2; // cukup untuk menjamin ada MEX
    int used[maxCheck];
    for (int i = 0; i < maxCheck; i++) used[i] = 0;

    Node *p = POSTS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_POST) {
            Post post = p->element.data.post;
            int num = getPostNumberFromWord(post.post_id);
            if (num >= 1 && num < maxCheck) {
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

void commandPost() {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat mengakses Groddit.\n");
        return;
    }

    printf("Masukkan nama Subgroddit:\n");

    Word subNameWord;
    readLineWord(&subNameWord);

    if (subNameWord.Length == 0) {
        printf("Nama Subgroddit tidak boleh kosong!\n");
        return;
    }

    char subNameStr[256];
    wordToString(subNameStr, subNameWord);

    int subIdx = findSubgrodditIndexByName(subNameStr);
    if (subIdx == -1) {
        printf("Subgroddit %s belum ditemukan!\n", subNameStr);
        printf("Gunakan perintah CREATE_SUBGRODDIT terlebih dahulu untuk membuatnya.\n");
        return;
    }

    // Ambil Subgroddit ke-subIdx dari List
    Node *p = SUBGRODDITS.head;
    int idx = 0;
    while (p != NULL && idx < subIdx) {
        if (p->element.type == TYPE_SUBGRODDIT) {
            idx++;
            if (idx > subIdx) break;
        }
        p = p->next;
    }

    Word subIdWord;
    if (p != NULL && p->element.type == TYPE_SUBGRODDIT) {
        subIdWord = p->element.data.subgroddit.subgroddit_id;
    }

    printf("Masukkan judul post:\n");
    Word titleW;
    readLineWord(&titleW);

    if (titleW.Length == 0) {
        printf("Judul post tidak boleh kosong!\n");
        return;
    }

    printf("Masukkan konten post:\n");
    Word contentW;
    readLineWord(&contentW);

    if (contentW.Length == 0) {
        printf("Konten post tidak boleh kosong!\n");
        return;
    }

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

    printf("Post berhasil dibuat dengan ID: %s di Subgroddit %s!\n", postIdStr, subNameStr);
}

void printPostHeader(const Post *p) {
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
    for (int i = 0; i < USER_COUNT; i++) {
        if (compareWord(USERS[i].user_id, p->author_id) != 0) {
            authorIndex = i;
            break;
        }
    }

    char authorUsername[256];
    if (authorIndex != -1) {
        wordToString(authorUsername, USERS[authorIndex].username);
    } else {
        wordToString(authorUsername, p->author_id);
    }

    printf("[%s] %s\n", subIdStr, titleStr);
    printf("oleh: %s (%s)\n", authorUsername, createdStr);
    printf("=======================================\n");
    printf("%s\n", contentStr);
    printf("%c ↑%d %c ↓%d\n", 24, p->upvotes, 25, p->downvotes);
}

void buildPostTree(const Post *p, PostTree *T) {
    CreatePostTree(T, *p);

    for (int i = 0; i < COMMENT_COUNT; i++) {
        if (compareWord(COMMENTS[i].post_id, p->post_id) != 0) {
            Comment c = COMMENTS[i];
            AddComment(T, c.parent_comment_id, c);
        }
    }
}

void commandViewPost() {
    ADVWORD_INPUT();

    if (currentWord.Length == 0) {
        // Tidak ada ID setelah VIEW_POST
        printf("Format perintah VIEW_POST salah. Gunakan 'VIEW_POST <ID>;' tanpa argumen lain.\n");
        return;
    }

    char postIdStr[256];
    wordToString(postIdStr, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        // Ada argumen berlebih setelah ID
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Format perintah VIEW_POST salah. Gunakan 'VIEW_POST <ID>;' tanpa argumen lain.\n");
        return;
    }

    int idx = findPostIndexById(postIdStr);
    if (idx == -1) {
        printf("Post dengan ID %s tidak ditemukan!\n", postIdStr);
        return;
    }

    Node *node = POSTS.head;
    int curIdx = 0;
    while (node != NULL && curIdx < idx) {
        node = node->next;
        curIdx++;
    }
    if (node == NULL || node->element.type != TYPE_POST) {
        return;
    }

    Post *p = &node->element.data.post;

    printPostHeader(p);

    PostTree T;
    buildPostTree(p, &T);

    printf("Komentar:\n");
    PrintPostTree(&T);
    printf("=======================================\n");
}

void deleteCommentsForPost(const Word postIdWord) {
    for (int i = COMMENT_COUNT - 1; i >= 0; i--) {
        if (compareWord(COMMENTS[i].post_id, postIdWord) != 0) {
            for (int j = i; j < COMMENT_COUNT - 1; j++) {
                COMMENTS[j] = COMMENTS[j + 1];
            }
            COMMENT_COUNT--;
        }
    }
}

void commandDeletePost() {
    ADVWORD_INPUT();

    if (currentWord.Length == 0) {
        // Tidak ada ID setelah DELETE_POST
        printf("Format perintah DELETE_POST salah. Gunakan 'DELETE_POST <ID>;' tanpa argumen lain.\n");
        return;
    }

    char postIdStr[256];
    wordToString(postIdStr, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0) {
        // Ada argumen berlebih setelah ID
        while (currentWord.Length != 0) {
            ADVWORD_INPUT();
        }
        printf("Format perintah DELETE_POST salah. Gunakan 'DELETE_POST <ID>;' tanpa argumen lain.\n");
        return;
    }

    int idx = findPostIndexById(postIdStr);
    if (idx == -1) {
        printf("Post [%s] tidak ditemukan!\n", postIdStr);
        printf("Anda tidak bisa menghapus fakta bahwa dia tidak dengan mu </3!!\n");
        return;
    }

    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat mengakses Groddit\n");
        return;
    }

    Node *node = POSTS.head;
    int curIdx = 0;
    while (node != NULL && curIdx < idx) {
        node = node->next;
        curIdx++;
    }
    if (node == NULL || node->element.type != TYPE_POST) {
        return;
    }

    Post *p = &node->element.data.post;

    if (compareWord(p->author_id, USERS[CURRENT_USER_INDEX].user_id) == 0) {
        printf("Anda bukan pembuat post [%s]!\n", postIdStr);
        printf("Hanya pembuat post yang dapat menghapus postingan ini.\n");
        return;
    }

    char titleStr[256];
    wordToString(titleStr, p->title);

    printf("Apakah Anda yakin ingin menghapus post [%s] \"%s\"? (Y/N)\n", postIdStr, titleStr);

    STARTWORD_INPUT();
    char answer[8];
    wordToString(answer, currentWord);
    IgnoreNewline();

    if (answer[0] != 'Y' && answer[0] != 'y') {
        printf("Penghapusan post dibatalkan.\n");
        return;
    }

    deleteCommentsForPost(p->post_id);

    ListElement deletedElem;
    deleteAtList(&POSTS, idx, &deletedElem);
    POST_COUNT--;

    printf("Post [%s] dan seluruh komentar terkait berhasil dihapus.\n", postIdStr);
}
