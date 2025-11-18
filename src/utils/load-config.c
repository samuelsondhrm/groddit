#include "header/load-config.h"

int loadComments(const char *filepath, Comment **commentPtr, int *capacityPtr)
{
    STARTWORD_CSV(filepath);
    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();

    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV();

    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    Comment *comments = *commentPtr;

    ADVWORD_CSV();

    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&comments, &capacity, sizeof(Comment), count + 1))
        {
            CLOSEPITA();
            return -1;
        }
        *commentPtr = comments;
        *capacityPtr = capacity;

        initComment(&comments[count]);
        Comment *C = &comments[count];

        int colidx = 0;

        while (!EndWordCSV && colidx < 7)
        {
            switch (colidx)
            {
            case 0:
                C->comment_id = wordToInt(currentWord);
                break;
            case 1:
                copyWord(&C->post_id, currentWord);
                break;
            case 2:
                copyWord(&C->author_id, currentWord);
                break;
            case 3:
                C->parent_comment_id = wordToInt(currentWord);
                break;
            case 4:
                copyWord(&C->content, currentWord);
                break;
            case 5:
                C->upvotes = wordToInt(currentWord);
                break;
            case 6:
                C->downvotes = wordToInt(currentWord);
                break;
            }

            colidx++;
            ADVWORD_CSV();
        }

        count++;
    }

    CLOSEPITA();
    return count;
}

int loadPosts(const char *filepath, Post **postPtr, int *capacityPtr)
{
    STARTWORD_CSV(filepath);
    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV();
    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    Post *posts = *postPtr;

    ADVWORD_CSV();
    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&posts, &capacity, sizeof(Post), count + 1))
        {
            CLOSEPITA();
            return -1;
        }
        *postPtr = posts;
        *capacityPtr = capacity;

        initPost(&posts[count]);
        Post *P = &posts[count];

        int colidx = 0;
        while (!EndWordCSV && colidx < 8)
        {
            switch (colidx)
            {
            case 0:
                copyWord(&P->post_id, currentWord);
                break;
            case 1:
                copyWord(&P->subgroddit_id, currentWord);
                break;
            case 2:
                copyWord(&P->author_id, currentWord);
                break;
            case 3:
                copyWord(&P->title, currentWord);
                break;
            case 4:
                copyWord(&P->content, currentWord);
                break;
            case 5:
                copyWord(&P->created_at, currentWord);
                break;
            case 6:
                P->upvotes = wordToInt(currentWord);
                break;
            case 7:
                P->downvotes = wordToInt(currentWord);
                break;
            }
            colidx++;
            ADVWORD_CSV();
        }
        count++;
    }
    CLOSEPITA();
    return count;
}

int loadUsers(const char *filepath, User **userPtr, int *capacityPtr)
{
    STARTWORD_CSV(filepath);
    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV();
    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    User *users = *userPtr;

    ADVWORD_CSV();
    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&users, &capacity, sizeof(User), count + 1))
        {
            CLOSEPITA();
            return -1;
        }
        *userPtr = users;
        *capacityPtr = capacity;

        initUser(&users[count]);
        User *U = &users[count];

        int colidx = 0;
        while (!EndWordCSV && colidx < 5)
        {
            switch (colidx)
            {
            case 0:
                copyWord(&U->user_id, currentWord);
                break;
            case 1:
                copyWord(&U->username, currentWord);
                break;
            case 2:
                copyWord(&U->password, currentWord);
                break;
            case 3:
                U->karma = wordToInt(currentWord);
                break;
            case 4:
                char created_at_str[64];
                copyWord(&created_at_str, currentWord);
                U->created_at = parseTime(created_at_str);
                break;
            }
            colidx++;
            ADVWORD_CSV();
        }
        count++;
    }
    CLOSEPITA();
    return count;
}

int loadSubGroddits(const char *filepath, SubGroddit **subPtr, int *capacityPtr)
{
    STARTWORD_CSV(filepath);
    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV();
    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    SubGroddit *subs = *subPtr;

    ADVWORD_CSV();
    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&subs, &capacity, sizeof(SubGroddit), count + 1))
        {
            CLOSEPITA();
            return -1;
        }
        *subPtr = subs;
        *capacityPtr = capacity;

        initSubGroddit(&subs[count]);
        SubGroddit *S = &subs[count];

        int colidx = 0;
        while (!EndWordCSV && colidx < 2)
        {
            switch (colidx)
            {
            case 0:
                copyWord(&S->subgroddit_id, currentWord);
                break;
            case 1:
                copyWord(&S->name, currentWord);
                break;
            }
            colidx++;
            ADVWORD_CSV();
        }
        count++;
    }
    CLOSEPITA();
    return count;
}

int loadSocials(const char *filepath, Social **socialPtr, int *capacityPtr)
{
    STARTWORD_CSV(filepath);
    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV();
    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    Social *socials = *socialPtr;

    ADVWORD_CSV();
    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&socials, &capacity, sizeof(Social), count + 1))
        {
            CLOSEPITA();
            return -1;
        }
        *socialPtr = socials;
        *capacityPtr = capacity;

        initSocial(&socials[count]);
        Social *SO = &socials[count];

        int colidx = 0;
        while (!EndWordCSV && colidx < 2)
        {
            switch (colidx)
            {
            case 0:
                copyWord(&SO->user_id, currentWord);
                break;
            case 1:
                copyWord(&SO->following_id, currentWord);
                break;
            }
            colidx++;
            ADVWORD_CSV();
        }
        count++;
    }
    CLOSEPITA();
    return count;
}

int loadVotings(const char *filepath, Voting **votingPtr, int *capacityPtr)
{
    STARTWORD_CSV(filepath);
    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV();
    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    Voting *votings = *votingPtr;

    ADVWORD_CSV();
    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&votings, &capacity, sizeof(Voting), count + 1))
        {
            CLOSEPITA();
            return -1;
        }
        *votingPtr = votings;
        *capacityPtr = capacity;

        initVoting(&votings[count]);
        Voting *V = &votings[count];

        int colidx = 0;
        while (!EndWordCSV && colidx < 4)
        {
            switch (colidx)
            {
            case 0:
                copyWord(&V->user_id, currentWord);
                break;
            case 1:
                copyWord(&V->target_type, currentWord);
                break;
            case 2:
                copyWord(&V->target_id, currentWord);
                break;
            case 3:
                copyWord(&V->vote_type, currentWord);
                break;
            }
            colidx++;
            ADVWORD_CSV();
        }
        count++;
    }
    CLOSEPITA();
    return count;
}