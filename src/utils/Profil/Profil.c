#include "Profil.h"

int countUserPosts(int userIndex) {
    // Menghitung post milik user userIndex
    int count = 0;
    Word userId = USERS[userIndex].user_id;
    Node *p = POSTS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_POST) {
            Post post = p->element.data.post;
            if (compareWord(post.author_id, userId) == 0)
                count++;
        }
        p = p->next;
    }
    return count;
}

int countUserComments(int userIndex) {
    // Menghitung komen milik user userIndex
    int count = 0;
    Word userId = USERS[userIndex].user_id;
    for (int i = 0; i < COMMENT_COUNT; i++) {
        if (compareWord(COMMENTS[i].author_id, userId) == 0)
            count++;
    }
    return count;
}

int countUserFollowers(int userIndex) {
    // Menghitung followers milik user userIndex
    int count = 0;
    Word userId = USERS[userIndex].user_id;
    for (int i = 0; i < SOCIAL_COUNT; i++) {
        if (compareWord(SOCIALS[i].following_id, userId) == 0)
            count++;
    }
    return count;
}

int countUserFollowing(int userIndex) {
    // Menghitung following milik user userIndex
    int count = 0;
    Word userId = USERS[userIndex].user_id;
    for (int i = 0; i < SOCIAL_COUNT; i++) {
        if (compareWord(SOCIALS[i].user_id, userId) == 0)
            count++;
    }
    return count;
}

boolean isCurrentUserFollowing(int currentIndex, int targetIndex) {
    // Cek apakah current user follow target user
    if (currentIndex == -1)
        return false;

    Word currId = USERS[currentIndex].user_id;
    Word targetId = USERS[targetIndex].user_id;

    for (int i = 0; i < SOCIAL_COUNT; i++) {
        if (compareWord(SOCIALS[i].user_id, currId) == 0 &&
            compareWord(SOCIALS[i].following_id, targetId) == 0) {
            return true;
        }
    }
    return false;
}

int computeUserKarma(int userIndex) {
    // Menghitung karma milik user userIndex
    int karma = 0;
    Word userId = USERS[userIndex].user_id;

    // Hitung karma untuk post
    Node *p = POSTS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_POST) {
            Post post = p->element.data.post;
            if (compareWord(post.author_id, userId) == 0)
            {
                karma += post.upvotes;
                karma -= post.downvotes;
            }
        }
        p = p->next;
    }

    // Hitung karma untuk komen
    for (int i = 0; i < COMMENT_COUNT; i++) {
        if (compareWord(COMMENTS[i].author_id, userId) == 0)
        {
            karma += COMMENTS[i].upvotes;
            karma -= COMMENTS[i].downvotes;
        }
    }

    return karma;
}

void findLatestPostsForUser(int userIndex, int indices[3], int *foundCount) {
    // Mencari 3 post terbaru milik user userIndex
    for (int i = 0; i < 3; i++)
        indices[i] = -1;
    *foundCount = 0;

    Word userId = USERS[userIndex].user_id;

    for (int k = 0; k < 3; k++) {
        time_t bestTime = 0;
        int bestIdx = -1;

        int idx = 0;
        Node *p = POSTS.head;
        while (p != NULL) {
            if (p->element.type == TYPE_POST) {
                Post post = p->element.data.post;
                if (compareWord(post.author_id, userId) == 0)
                {
                    // Jangan pilih post yang sudah dipilih sebelumnya
                    if (!((k >= 1 && idx == indices[0]) || (k >= 2 && idx == indices[1]))) {
                        if (bestIdx == -1 || post.created_at > bestTime) {
                            bestIdx = idx;
                            bestTime = post.created_at;
                        }
                    }
                }
                idx++;
            }
            p = p->next;
        }

        if (bestIdx == -1)
            break;

        indices[k] = bestIdx;
        (*foundCount)++;
    }
}

void getSubgrodditName(Word subId, char *out) {
    // default to ID jika tidak ketemu
    wordToString(out, subId);
    Node *p = SUBGRODDITS.head;
    while (p != NULL) {
        if (p->element.type == TYPE_SUBGRODDIT) {
            SubGroddit s = p->element.data.subgroddit;
            if (compareWord(s.subgroddit_id, subId) == 0) {
                wordToString(out, s.name);
                return;
            }
        }
        p = p->next;
    }
}

void showUserProfile(const char *username) {
    if (!isLoggedIn()) {
        printf("Anda belum login! Masuk terlebih dahulu untuk dapat mengakses Groddit.\n");
        return;
    }

    int idx = findIdByUsername(username);
    if (idx == -1) {
        printf("Tidak ada pengguna dengan username %s. Harap periksa masukan Anda!\n", username);
        return;
    }

    User *u = &USERS[idx];

    char userIdStr[256];
    char usernameStr[256];
    char createdAtStr[256];

    wordToString(userIdStr, u->user_id);
    wordToString(usernameStr, u->username);
    timeToStr(createdAtStr, u->created_at);

    int posts = countUserPosts(idx);
    int comments = countUserComments(idx);
    int followers = countUserFollowers(idx);
    int following = countUserFollowing(idx);
    int karma = computeUserKarma(idx);

    printf("USER PROFILE : %s\n", usernameStr);
    printf("---------------------\n");
    printf("ID: %s\n", userIdStr);
    printf("Karma: %d\n", karma);
    printf("Account Created: %s\n", createdAtStr);
    printf("Posts: %d\n", posts);
    printf("Comments: %d\n", comments);
    printf("Followers: %d\n", followers);
    printf("Following: %d\n\n", following);

    if (isCurrentUserFollowing(CURRENT_USER_INDEX, idx)) {
        printf("Anda mengikuti pengguna ini.\n\n");
    } else {
        printf("Anda tidak mengikuti pengguna ini.\n\n");
    }

    int latestIdx[3];
    int foundCount = 0;
    findLatestPostsForUser(idx, latestIdx, &foundCount);

    if (foundCount > 0) {
        printf("Post Terbaru:\n");
        for (int i = 0; i < foundCount; i++) {
            int pIdx = latestIdx[i];

            int idx = 0;
            Node *pNode = POSTS.head;
            while (pNode != NULL) {
                if (pNode->element.type == TYPE_POST) {
                    if (idx == pIdx) {
                        break;
                    }
                    idx++;
                }
                pNode = pNode->next;
            }
            if (pNode == NULL || pNode->element.type != TYPE_POST) {
                continue;
            }

            Post *p = &pNode->element.data.post;

            char subName[256];
            char titleStr[256];
            char createdStr[256];

            getSubgrodditName(p->subgroddit_id, subName);
            wordToString(titleStr, p->title);
            timeToStr(createdStr, p->created_at);

            int score = p->upvotes - p->downvotes;

            printf("%d. [%s] %s (%d↑)\n", i + 1, subName, titleStr, score);
            printf("posted: %s\n", createdStr);
        }
    }

    printf("\n---------------------\n");
}
