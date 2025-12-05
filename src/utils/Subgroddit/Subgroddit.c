#include "Subgroddit.h"

// Struktur helper untuk sorting post
typedef struct
{
    Post post;
    int sortValue;
} PostWithSort;

void createSubgroddit()
{
    clearScreen();
    printBreadcrumb("Home > Create Subgroddit");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                     %sCREATE SUBGRODDIT%s                          %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    printSectionHeader("", "SUBGRODDIT NAME");
    printf("\n");
    printf("%s %sFormat:%s Must start with %sr/%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_YELLOW, RESET);
    printf("%s %sExample:%s %sr/programming%s, %sr/datascience%s\n", BOX_V, DIM, RESET, BOLD_CYAN, RESET, BOLD_CYAN, RESET);
    printSectionDivider();
    printf("\n");
    printf("%s└─▶ %sEnter name:%s ", BOLD_CYAN, BOLD_WHITE, RESET);
    fflush(stdout);

    STARTWORD_INPUT();

    if (currentWord.Length == 0)
    {
        printf("\n");
        printError("Invalid input");
        printf("Subgroddit name cannot be empty.\n\n");
        printf("%sTip:%s Names must start with %sr/%s followed by the community name.\n", 
               BOLD_CYAN, RESET, BOLD_YELLOW, RESET);
        return;
    }

    char subgroddit_name[256];
    wordToString(subgroddit_name, currentWord);

    if (currentWord.Length < 2 || subgroddit_name[0] != 'r' || subgroddit_name[1] != '/')
    {
        printf("\n");
        printError("Invalid format");
        printf("Name must start with %sr/%s\n\n", BOLD_YELLOW, RESET);
        printf("%sYou entered:%s %s%s%s\n", BOLD_WHITE, RESET, BOLD_RED, subgroddit_name, RESET);
        printf("%sCorrect format:%s %sr/yourname%s\n", BOLD_WHITE, RESET, BOLD_GREEN, RESET);
        return;
    }

    printf("\n");
    spinnerAnimation("Checking name availability", 6);

    if (findSubgrodditIndexByName(subgroddit_name) != -1)
    {
        printf("\n");
        printError("Name already exists");
        printf("The subgroddit %s%s%s is already taken.\n\n", BOLD_RED, subgroddit_name, RESET);
        printf("%sTip:%s Choose a unique name for your community.\n", BOLD_CYAN, RESET);
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

    printf("\n");
    spinnerAnimation("Creating subgroddit", 8);
    printf("\n");
    printSuccess("Subgroddit created successfully");
    printf("%s╭─────────────────────────────────────────────╮%s\n", BOLD_CYAN, RESET);
    printf("%s│%s Name : %s%s%s\n", BOLD_CYAN, RESET, BOLD_GREEN, subgroddit_name, RESET);
    printf("%s│%s ID   : %s%s%s\n", BOLD_CYAN, RESET, BOLD_WHITE, id_str, RESET);
    printf("%s╰─────────────────────────────────────────────╯%s\n", BOLD_CYAN, RESET);
}

void viewSubgroddit()
{
    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Invalid command format");
        printf("Use: %sVIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;%s\n", BOLD_WHITE, RESET);
        return;
    }

    char subgrodditName[256];
    wordToString(subgrodditName, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Missing MODE parameter");
        printf("Use: %sVIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;%s\n", BOLD_WHITE, RESET);
        return;
    }

    char mode[256];
    wordToString(mode, currentWord);

    ADVWORD_INPUT();
    if (currentWord.Length == 0)
    {
        printError("Missing ORDER parameter");
        printf("Use: %sVIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;%s\n", BOLD_WHITE, RESET);
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
        printError("Too many arguments");
        printf("Use: %sVIEW_SUBGRODDIT <NAME> <MODE> <ORDER>;%s\n", BOLD_WHITE, RESET);
        return;
    }

    if (strCmp(mode, "HOT") != 0 && strCmp(mode, "NEW") != 0)
    {
        printError("Invalid MODE");
        printf("Mode %s%s%s not recognized. Use %sHOT%s or %sNEW%s.\n", 
               BOLD_YELLOW, mode, RESET, BOLD_WHITE, RESET, BOLD_WHITE, RESET);
        return;
    }

    if (strCmp(order, "INCR") != 0 && strCmp(order, "DECR") != 0)
    {
        printError("Invalid ORDER");
        printf("Order %s%s%s not recognized. Use %sINCR%s or %sDECR%s.\n", 
               BOLD_YELLOW, order, RESET, BOLD_WHITE, RESET, BOLD_WHITE, RESET);
        return;
    }

    int subIdx = findSubgrodditIndexByName(subgrodditName);
    if (subIdx == -1)
    {
        printError("Subgroddit not found");
        printf("Subgroddit %s%s%s doesn't exist.\n", BOLD_YELLOW, subgrodditName, RESET);
        printf("\n%sTip:%s Use %sCREATE_SUBGRODDIT;%s to create it first.\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
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

    // Clear screen and show header
    clearScreen();
    char breadcrumbPath[512];
    sprintf(breadcrumbPath, "Main Menu > Subgroddit > %s", subgrodditName);
    printBreadcrumb(breadcrumbPath);

    printf("\n");
    printf("%s", BOLD_MAGENTA);
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s              %s  SUBGRODDIT : %s%-30s%s  %s\n", 
           DBOX_V, strCmp(mode, "HOT") == 0 ? "🔥" : "⚫", 
           BOLD_WHITE, subgrodditName, RESET, DBOX_V);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    printf("%s%s ⚡ Sort Mode:%s %s%s%s  %s🔽 Order:%s %s%s %s%s\n", 
           BOX_V, BOLD_WHITE, RESET, 
           BOLD_CYAN, mode, RESET,
           BOLD_WHITE, RESET,
           BOLD_CYAN, order, 
           strCmp(order, "INCR") == 0 ? "↑" : "↓", RESET);
    printSectionDivider();

    if (postCount == 0)
    {
        printf("\n");
        printWarning("No posts in this subgroddit");
        printf("This subgroddit doesn't have any posts yet.\n");
        printf("\n%sTip:%s Use %sPOST;%s to create the first post!\n", 
               BOLD_CYAN, RESET, BOLD_WHITE, RESET);
    }
    else
    {
        printf("\n");
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

            printf("%s %s%d.%s [%s%s%s] %s%s%s\n",
                   BOX_V, BOLD_CYAN, i + 1, RESET,
                   BOLD_MAGENTA, postIdStr, RESET,
                   BOLD_WHITE, titleStr, RESET);
            printf("%s    %s↑ %d%s %s│%s %s↓ %d%s %s│%s by %s@%s%s %s│%s %s%s%s\n",
                   BOX_V, GREEN, post->upvotes, RESET,
                   DIM, RESET,
                   RED, post->downvotes, RESET,
                   DIM, RESET, BOLD_CYAN, authorUsername, RESET,
                   DIM, RESET, DIM, createdStr, RESET);
            
            if (i < postCount - 1) {
                printf("%s\n", BOX_V);
            }
        }
    }

    printf("\n");
    printSectionDivider();
    printf("\n");
    printInfo("Subgroddit loaded successfully");
    printf("%sTip:%s Use %sVIEW_POST <ID>;%s to view post details.\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET);

    free(posts);
}
