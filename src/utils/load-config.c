#include <stdio.h>
#include <stdlib.h>
#include "../../adt/header/adt-sederhana.h"
#include "header/load-config.h"
#include "header/helper.h"

int loadUsers(const char *filepath, User **userPtr, int *capacityPtr)
{
    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        printf("[DEBUG] fopen failed: %s\n", filepath);
        return -1;
    }

    char buffer[512];
    int count = 0;

    int capacity = *capacityPtr;
    User *users = *userPtr;

    if (!fgets(buffer, sizeof(buffer), f))
    {
        fclose(f);
        return 0;
    }

    while (1)
    {
        if (!ensureCapacity((void **)&users, &capacity, sizeof(User), count + 1))
        {
            fclose(f);
            return -1;
        }
        *userPtr = users;
        *capacityPtr = capacity;

        initUser(&users[count]);

        char f1[32], f2[256], f3[64], f4[32], f5[32];

        int end = readField(f1, sizeof(f1), f);
        if (end && f1[0] == '\0')
        {
            break;
        }
        readField(f2, sizeof(f2), f);
        readField(f3, sizeof(f3), f);
        readField(f4, sizeof(f4), f);
        readField(f5, sizeof(f5), f);

        copyString(users[count].user_id, f1, 20);
        copyString(users[count].username, f2, 256);
        copyString(users[count].password, f3, 25);
        users[count].karma = toInt(f4);
        users[count].created_at = parseTime(f5);

        count++;
    }

    fclose(f);
    return count;
}

int loadPosts(const char *filepath, Post **postPtr, int *capacityPtr)
{
    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        printf("[DEBUG] fopen failed: %s\n", filepath);
        return -1;
    }

    char buffer[512];
    int count = 0;

    int capacity = *capacityPtr;
    Post *posts = *postPtr;

    if (!fgets(buffer, sizeof(buffer), f))
    {
        fclose(f);
        return 0;
    }

    while (1)
    {
        if (!ensureCapacity((void **)&posts, &capacity, sizeof(Post), count + 1))
        {
            fclose(f);
            return -1;
        }
        *postPtr = posts;
        *capacityPtr = capacity;

        initPost(&posts[count]);

        char f1[32], f2[32], f3[32], f4[256], f5[512], f6[32], f7[32], f8[32];

        int end = readField(f1, sizeof(f1), f);
        if (end && f1[0] == '\0')
        {
            break;
        }
        readField(f2, sizeof(f2), f);
        readField(f3, sizeof(f3), f);
        readField(f4, sizeof(f4), f);
        readField(f5, sizeof(f5), f);
        readField(f6, sizeof(f6), f);
        readField(f7, sizeof(f7), f);
        readField(f8, sizeof(f8), f);

        copyString(posts[count].post_id, f1, 20);
        copyString(posts[count].subgroddit_id, f2, 20);
        copyString(posts[count].author_id, f3, 20);
        copyString(posts[count].title, f4, 256);
        copyStringDynamic(&posts[count].content, f5);
        posts[count].created_at = parseTime(f6);
        posts[count].upvotes = toInt(f7);
        posts[count].downvotes = toInt(f8);

        count++;
    }

    fclose(f);
    return count;
}

int loadComments(const char *filepath, Comment **commentPtr, int *capacityPtr)
{
    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        printf("[DEBUG] fopen failed: %s\n", filepath);
        return -1;
    }

    char buffer[512];
    int count = 0;

    int capacity = *capacityPtr;
    Comment *comment = *commentPtr;

    if (!fgets(buffer, sizeof(buffer), f))
    {
        fclose(f);
        return 0;
    }

    while (1)
    {
        if (!ensureCapacity((void **)&comment, &capacity, sizeof(Comment), count + 1))
        {
            fclose(f);
            return -1;
        }
        *commentPtr = comment;
        *capacityPtr = capacity;

        initComment(&comment[count]);

        char f1[32], f2[32], f3[64], f4[32], f5[512], f6[32], f7[32];

        int end = readField(f1, sizeof(f1), f);
        if (end && f1[0] == '\0')
        {
            break;
        }
        readField(f2, sizeof(f2), f);
        readField(f3, sizeof(f3), f);
        readField(f4, sizeof(f4), f);
        readField(f5, sizeof(f5), f);
        readField(f6, sizeof(f6), f);
        readField(f7, sizeof(f7), f);

        comment[count].comment_id = toInt(f1);
        copyString(comment[count].post_id, f2, 20);
        copyString(comment[count].author_id, f3, 20);
        comment[count].parent_comment_id = toInt(f4);
        copyStringDynamic(&comment[count].content, f5);
        comment[count].upvotes = toInt(f6);
        comment[count].downvotes = toInt(f7);

        count++;
    }

    fclose(f);

    return count;
}

int loadSubgroddits(const char *filepath, Subgroddit **subPtr, int *capacityPtr)
{
    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        printf("[DEBUG] fopen failed: %s\n", filepath);
        return -1;
    }

    char buffer[512];
    int count = 0;

    int capacity = *capacityPtr;
    Subgroddit *subs = *subPtr;

    if (!fgets(buffer, sizeof(buffer), f))
    {
        fclose(f);
        return 0;
    }

    while (1)
    {
        if (!ensureCapacity((void **)&subs, &capacity, sizeof(Subgroddit), count + 1))
        {
            fclose(f);
            return -1;
        }

        *subPtr = subs;
        *capacityPtr = capacity;

        initSubgroddit(&subs[count]);

        char f1[32], f2[256];

        int end = readField(f1, sizeof(f1), f);
        if (end && f1[0] == '\0')
        {
            break;
        }
        readField(f2, sizeof(f2), f);

        copyString(subs[count].subgroddit_id, f1, 20);
        copyString(subs[count].name, f2, 256);

        count++;
    }

    fclose(f);
    return count;
}

int loadSocial(const char *filepath, Social **socialPtr, int *capacityPtr)
{
    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        printf("[DEBUG] fopen failed: %s\n", filepath);
        return -1;
    }

    char buffer[256];
    int count = 0;

    int capacity = *capacityPtr;
    Social *socials = *socialPtr;

    if (!fgets(buffer, sizeof(buffer), f))
    {
        fclose(f);
        return 0;
    }

    while (1)
    {
        if (!ensureCapacity((void **)&socials, &capacity, sizeof(Social), count + 1))
        {
            fclose(f);
            return -1;
        }

        *socialPtr = socials;
        *capacityPtr = capacity;

        initSocial(&socials[count]);

        char f1[32], f2[32];

        int end = readField(f1, sizeof(f1), f);
        if (end && f1[0] == '\0')
        {
            break;
        }
        readField(f2, sizeof(f2), f);

        copyString(socials[count].follower_id, f1, 20);
        copyString(socials[count].following_id, f2, 20);

        count++;
    }

    fclose(f);
    return count;
}

int loadVoting(const char *filepath, Voting **votePtr, int *capacityPtr)
{
    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        printf("[DEBUG] fopen failed: %s\n", filepath);
        return -1;
    }

    char buffer[256];
    int count = 0;

    int capacity = *capacityPtr;
    Voting *votes = *votePtr;

    if (!fgets(buffer, sizeof(buffer), f))
    {
        fclose(f);
        return 0;
    }

    while (1)
    {
        if (!ensureCapacity((void **)&votes, &capacity, sizeof(Voting), count + 1))
        {
            fclose(f);
            return -1;
        }

        *votePtr = votes;
        *capacityPtr = capacity;

        initVoting(&votes[count]);

        char f1[32], f2[32], f3[32], f4[32];

        int end = readField(f1, sizeof(f1), f);
        if (end && f1[0] == '\0')
        {
            break;
        }
        readField(f2, sizeof(f2), f);
        readField(f3, sizeof(f3), f);
        readField(f4, sizeof(f4), f);

        copyString(votes[count].user_id, f1, 20);
        copyString(votes[count].target_type, f2, 20);
        copyString(votes[count].target_id, f3, 20);
        copyString(votes[count].vote_type, f4, 20);

        count++;
    }

    fclose(f);
    return count;
}
