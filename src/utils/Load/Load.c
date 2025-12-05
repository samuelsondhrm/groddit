#include "Load.h"

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

int loadPosts(const char *filepath, List *postList)
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

    createList(postList);
    int count = 0;

    ADVWORD_CSV();
    while (!EndWordCSV)
    {
        Post P;
        initPost(&P);

        int colidx = 0;
        while (!EndWordCSV && colidx < 8)
        {
            switch (colidx)
            {
            case 0:
                copyWord(&P.post_id, currentWord);
                break;
            case 1:
                copyWord(&P.subgroddit_id, currentWord);
                break;
            case 2:
                copyWord(&P.author_id, currentWord);
                break;
            case 3:
                copyWord(&P.title, currentWord);
                break;
            case 4:
                copyWord(&P.content, currentWord);
                break;
            case 5:
            {
                char created_at_str[64];
                wordToString_safe(created_at_str, sizeof(created_at_str), currentWord);
                P.created_at = parseTime(created_at_str);
            }
            break;
            case 6:
                P.upvotes = wordToInt(currentWord);
                break;
            case 7:
                P.downvotes = wordToInt(currentWord);
                break;
            }
            colidx++;
            ADVWORD_CSV();
        }

        insertLastList(postList, makePostElement(P));
        count++;
    }
    CLOSEPITA();
    return count;
}

int loadUsers(const char *filepath, User **userPtr, int *capacityPtr)
{
    size_t len = 0;
    boolean encrypted = false;

    if (file_encryption_enabled())
    {
        uint8_t *buffer = read_encrypted_file(filepath, &len, &encrypted);

        if (!buffer || len == 0)
            return 0;

        STARTWORD_CSV_BUFFER(buffer, len);

        User *users = *userPtr;
        int capacity = *capacityPtr;
        int count = 0;

        if (EndWordCSV)
        {
            free(buffer);
            return 0;
        }

        while (!EOP && currentChar != '\n' && currentChar != '\r')
            ADVWORD_CSV();
        while (!EOP && (currentChar == '\n' || currentChar == '\r'))
            ADVCSV();

        if (EOP)
        {
            free(buffer);
            return 0;
        }

        ADVWORD_CSV();

        while (!EndWordCSV)
        {
            if (!ensureCapacity((void **)&users, &capacity, sizeof(User), count + 1))
            {
                free(buffer);
                return -1;
            }

            *userPtr = users;
            *capacityPtr = capacity;

            initUser(&users[count]);
            User *U = &users[count];

            int col = 0;
            while (!EndWordCSV && col < 5)
            {
                switch (col)
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
                {
                    char created[64];
                    wordToString_safe(created, 64, currentWord);
                    U->created_at = parseTime(created);
                }
                break;
                }

                col++;
                ADVWORD_CSV();
            }

            count++;
        }

        free(buffer);
        return count;
    }

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
            return -1;

        *userPtr = users;
        *capacityPtr = capacity;

        initUser(&users[count]);
        User *U = &users[count];

        int col = 0;
        while (!EndWordCSV && col < 5)
        {
            switch (col)
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
            {
                char created[64];
                wordToString_safe(created, 64, currentWord);
                U->created_at = parseTime(created);
            }
            break;
            }

            col++;
            ADVWORD_CSV();
        }

        count++;
    }

    return count;
}

int loadSubGroddits(const char *filepath, List *subList)
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

    createList(subList);
    int count = 0;

    ADVWORD_CSV();
    while (!EndWordCSV)
    {
        SubGroddit S;
        initSubGroddit(&S);

        int colidx = 0;
        while (!EndWordCSV && colidx < 2)
        {
            switch (colidx)
            {
            case 0:
                copyWord(&S.subgroddit_id, currentWord);
                break;
            case 1:
                copyWord(&S.name, currentWord);
                break;
            }
            colidx++;
            ADVWORD_CSV();
        }

        insertLastList(subList, makeSubgrodditElement(S));
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

int loadComments_Buffer(uint8_t *buffer, size_t len,
                        Comment **commentPtr, int *capacityPtr)
{
    EndWordCSV = false;
    EOP = false;

    STARTWORD_CSV_BUFFER(buffer, len);

    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV_BUFFER();

    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    Comment *comments = *commentPtr;

    ADVWORD_CSV();

    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&comments,
                            &capacity,
                            sizeof(Comment),
                            count + 1))
            return -1;

        *commentPtr = comments;
        *capacityPtr = capacity;

        initComment(&comments[count]);
        Comment *C = &comments[count];

        int col = 0;

        while (!EndWordCSV && col < 7)
        {
            switch (col)
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

            col++;
            ADVWORD_CSV();
        }

        count++;
    }

    return count;
}

int loadPosts_Buffer(uint8_t *buffer, size_t len, List *postList)
{
    EndWordCSV = false;
    EOP = false;

    STARTWORD_CSV_BUFFER(buffer, len);

    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV_BUFFER();

    if (EOP)
        return 0;

    createList(postList);
    int count = 0;

    ADVWORD_CSV();

    while (!EndWordCSV)
    {
        Post P;
        initPost(&P);

        int col = 0;

        while (!EndWordCSV && col < 8)
        {
            switch (col)
            {
            case 0:
                copyWord(&P.post_id, currentWord);
                break;
            case 1:
                copyWord(&P.subgroddit_id, currentWord);
                break;
            case 2:
                copyWord(&P.author_id, currentWord);
                break;
            case 3:
                copyWord(&P.title, currentWord);
                break;
            case 4:
                copyWord(&P.content, currentWord);
                break;
            case 5:
            {
                char t[64];
                wordToString_safe(t, 64, currentWord);
                P.created_at = parseTime(t);
            }
            break;
            case 6:
                P.upvotes = wordToInt(currentWord);
                break;
            case 7:
                P.downvotes = wordToInt(currentWord);
                break;
            }

            col++;
            ADVWORD_CSV();
        }

        insertLastList(postList, makePostElement(P));
        count++;
    }

    return count;
}

int loadUsers_Buffer(uint8_t *buffer, size_t len, User **userPtr, int *capacityPtr)
{
    EndWordCSV = false;
    EOP = false;

    STARTWORD_CSV_BUFFER(buffer, len);

    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV_BUFFER();

    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    User *users = *userPtr;

    ADVWORD_CSV();

    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&users, &capacity, sizeof(User), count + 1))
            return -1;

        *userPtr = users;
        *capacityPtr = capacity;

        initUser(&users[count]);
        User *U = &users[count];

        int col = 0;
        while (!EndWordCSV && col < 5)
        {
            switch (col)
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
            {
                char t[64];
                wordToString_safe(t, 64, currentWord);
                U->created_at = parseTime(t);
            }
            break;
            }

            col++;
            ADVWORD_CSV();
        }

        count++;
    }

    return count;
}

int loadSubGroddits_Buffer(uint8_t *buffer, size_t len, List *subList)
{
    EndWordCSV = false;
    EOP = false;

    STARTWORD_CSV_BUFFER(buffer, len);

    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV_BUFFER();

    if (EOP)
        return 0;

    createList(subList);
    int count = 0;

    ADVWORD_CSV();

    while (!EndWordCSV)
    {
        SubGroddit S;
        initSubGroddit(&S);

        int col = 0;
        while (!EndWordCSV && col < 2)
        {
            switch (col)
            {
            case 0:
                copyWord(&S.subgroddit_id, currentWord);
                break;
            case 1:
                copyWord(&S.name, currentWord);
                break;
            }
            col++;
            ADVWORD_CSV();
        }

        insertLastList(subList, makeSubgrodditElement(S));
        count++;
    }

    return count;
}

int loadSocials_Buffer(uint8_t *buffer, size_t len,
                       Social **socialPtr, int *capacityPtr)
{
    EndWordCSV = false;
    EOP = false;

    STARTWORD_CSV_BUFFER(buffer, len);

    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV_BUFFER();

    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    Social *socials = *socialPtr;

    ADVWORD_CSV();

    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&socials, &capacity, sizeof(Social), count + 1))
            return -1;

        *socialPtr = socials;
        *capacityPtr = capacity;

        initSocial(&socials[count]);
        Social *S = &socials[count];

        int col = 0;
        while (!EndWordCSV && col < 2)
        {
            switch (col)
            {
            case 0:
                copyWord(&S->user_id, currentWord);
                break;
            case 1:
                copyWord(&S->following_id, currentWord);
                break;
            }

            col++;
            ADVWORD_CSV();
        }

        count++;
    }

    return count;
}

int loadVotings_Buffer(uint8_t *buffer, size_t len,
                       Voting **votingPtr, int *capacityPtr)
{
    EndWordCSV = false;
    EOP = false;

    STARTWORD_CSV_BUFFER(buffer, len);

    if (EndWordCSV)
        return 0;

    while (!EOP && currentChar != '\n' && currentChar != '\r')
        ADVWORD_CSV();
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        ADVCSV_BUFFER();

    if (EOP)
        return 0;

    int count = 0;
    int capacity = *capacityPtr;
    Voting *votings = *votingPtr;

    ADVWORD_CSV();

    while (!EndWordCSV)
    {
        if (!ensureCapacity((void **)&votings, &capacity, sizeof(Voting), count + 1))
            return -1;

        *votingPtr = votings;
        *capacityPtr = capacity;

        initVoting(&votings[count]);
        Voting *V = &votings[count];

        int col = 0;
        while (!EndWordCSV && col < 4)
        {
            switch (col)
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

            col++;
            ADVWORD_CSV();
        }

        count++;
    }

    return count;
}