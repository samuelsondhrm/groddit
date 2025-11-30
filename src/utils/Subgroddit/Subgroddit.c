#include "Subgroddit.h"

// Struktur helper untuk sorting post
typedef struct
{
    Post post;
    int sortValue;
} PostWithSort;

void createSubgroddit()
{
    printf("Masukkan nama subgroddit.\n");
    fflush(stdout);

    STARTWORD_INPUT();

    if (currentWord.Length == 0)
    {
        printf("Nama subgroddit tidak boleh kosong!\n");
        return;
    }

    char subgroddit_name[256];
    wordToString(subgroddit_name, currentWord);

    if (currentWord.Length < 2 || subgroddit_name[0] != 'r' || subgroddit_name[1] != '/')
    {
        printf("Nama Subgroddit HARUS UNIK dan diawali dengan \"r/\".\n");
        return;
    }

    if (findSubgrodditIndexByName(subgroddit_name) != -1)
    {
        printf("Nama Subgroddit HARUS UNIK dan diawali dengan \"r/\".\n");
        return;
    }

    SubGroddit newSubgroddit;
    initSubGroddit(&newSubgroddit);

    char id_str[256];
    sprintf(id_str, "S%03d", SUBGRODDIT_COUNT + 1);
    stringToWord(&newSubgroddit.subgroddit_id, id_str);

    stringToWord(&newSubgroddit.name, subgroddit_name);

    ListElement elem;
    elem.type = TYPE_SUBGRODDIT;
    elem.data.subgroddit = newSubgroddit;
    insertLastList(&SUBGRODDITS, elem);

    SUBGRODDIT_COUNT++;

    printf("Subgroddit %s berhasil dibuat!\n", subgroddit_name);
}

void viewSubgroddit()
{
    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format perintah VIEW_SUBGRODDIT salah. Gunakan 'VIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;'.\n");
        return;
    }

    char subgrodditName[256];
    wordToString(subgrodditName, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format perintah VIEW_SUBGRODDIT salah. Gunakan 'VIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;'.\n");
        return;
    }

    char mode[256];
    wordToString(mode, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printf("Format perintah VIEW_SUBGRODDIT salah. Gunakan 'VIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;'.\n");
        return;
    }

    char order[256];
    wordToString(order, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length != 0)
    {
        while (currentWord.Length != 0)
        {
            ADVWORD_INPUT();
        }
        printf("Format perintah VIEW_SUBGRODDIT salah. Gunakan 'VIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;'.\n");
        return;
    }

    if (strCmp(mode, "HOT") != 0 && strCmp(mode, "NEW") != 0)
    {
        printf("Mode %s tidak dikenali!\nGunakan HOT atau NEW sebagai mode yang valid.\n", mode);
        return;
    }

    if (strCmp(order, "INCR") != 0 && strCmp(order, "DECR") != 0)
    {
        printf("Order %s tidak dikenali!\nGunakan INCR atau DECR sebagai order yang valid.\n", order);
        return;
    }

    int subIdx = findSubgrodditIndexByName(subgrodditName);
    if (subIdx == -1)
    {
        printf("Subgroddit %s belum ditemukan!\nGunakan perintah CREATE_SUBGRODDIT untuk membuatnya terlebih dahulu.\n", subgrodditName);
        return;
    }

    Node *subNode = SUBGRODDITS.head;
    int curIdx = 0;
    while (subNode != NULL && curIdx < subIdx)
    {
        subNode = subNode->next;
        curIdx++;
    }
    if (subNode == NULL || subNode->element.type != TYPE_SUBGRODDIT)
    {
        return;
    }
    SubGroddit *sub = &subNode->element.data.subgroddit;

    PostWithSort *posts = NULL;
    int postCount = 0;
    int capacity = 10;
    posts = (PostWithSort *)malloc(capacity * sizeof(PostWithSort));

    Node *p = POSTS.head;
    while (p != NULL)
    {
        if (p->element.type == TYPE_POST)
        {
            Post *post = &p->element.data.post;
            if (compareWord(post->subgroddit_id, sub->subgroddit_id) != 0)
            {
                if (postCount >= capacity)
                {
                    capacity *= 2;
                    posts = (PostWithSort *)realloc(posts, capacity * sizeof(PostWithSort));
                }

                posts[postCount].post = *post;

                if (strCmp(mode, "HOT") == 0)
                {
                    posts[postCount].sortValue = post->upvotes;
                }
                else
                {
                    posts[postCount].sortValue = (int)post->created_at;
                }

                postCount++;
            }
        }
        p = p->next;
    }

    // Bubble sorting
    for (int i = 0; i < postCount - 1; i++)
    {
        for (int j = 0; j < postCount - i - 1; j++)
        {
            boolean shouldSwap = false;

            if (strCmp(order, "DECR") == 0)
            {
                if (posts[j].sortValue < posts[j + 1].sortValue)
                {
                    shouldSwap = true;
                }
            }
            else
            {
                if (posts[j].sortValue > posts[j + 1].sortValue)
                {
                    shouldSwap = true;
                }
            }

            if (shouldSwap)
            {
                PostWithSort temp = posts[j];
                posts[j] = posts[j + 1];
                posts[j + 1] = temp;
            }
        }
    }

    if (strCmp(mode, "HOT") == 0)
    {
        printf("\xF0\x9F\x94\xA5  Subgroddit: %s (sorted by %s %c)\n", subgrodditName, mode,
               strCmp(order, "INCR") == 0 ? 24 : 25);
    }
    else
    {
        printf("\xE2\x9A\xAB  Subgroddit: %s (sorted by %s %c)\n", subgrodditName, mode,
               strCmp(order, "INCR") == 0 ? 24 : 25);
    }
    printf("=======================================\n");

    if (postCount == 0)
    {
        printf("(Tidak ada postingan)\n");
    }
    else
    {
        for (int i = 0; i < postCount; i++)
        {
            Post *post = &posts[i].post;

            char postIdStr[256];
            char titleStr[256];
            char createdStr[256];
            char authorIdStr[256];

            wordToString(postIdStr, post->post_id);
            wordToString(titleStr, post->title);
            timeToStr(createdStr, post->created_at);
            wordToString(authorIdStr, post->author_id);

            char authorUsername[256];
            int authorFound = 0;
            for (int j = 0; j < USER_COUNT; j++)
            {
                if (compareWord(USERS[j].user_id, post->author_id) != 0)
                {
                    wordToString(authorUsername, USERS[j].username);
                    authorFound = 1;
                    break;
                }
            }
            if (!authorFound)
            {
                wordToString(authorUsername, post->author_id);
            }

            printf("%d. [%s] %s (%d%c / %d%c) - oleh %s\n",
                   i + 1, postIdStr, titleStr,
                   post->upvotes, 24,
                   post->downvotes, 25,
                   authorUsername);
        }
    }

    printf("=======================================\n");
    printf("Gunakan VIEW_POST <ID> untuk melihat detail postingan.\n");

    free(posts);
}
