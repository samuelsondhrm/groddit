#include "main.h"

int main()
{
    initialize();
    char commandStr[50];
    boolean quit = false;

    while (!quit)
    {
        printf("Masukkan perintah: ");
        fflush(stdout);

        STARTWORD_INPUT();

        if (currentWord.Length == 0)
        {
            continue;
        }

        wordToString(commandStr, currentWord);

        if (strCmp(commandStr, "PRINTCOMMENTS") == 0)
        {
            printComments();
        }
        else if (strCmp(commandStr, "PRINTPOSTS") == 0)
        {
            printPosts();
        }
        else if (strCmp(commandStr, "PRINTUSERS") == 0)
        {
            printUsers();
        }
        else if (strCmp(commandStr, "PRINTSUBGRODDITS") == 0)
        {
            printSubGroddits();
        }
        else if (strCmp(commandStr, "PRINTSOCIALS") == 0)
        {
            printSocials();
        }
        else if (strCmp(commandStr, "PRINTVOTINGS") == 0)
        {
            printVotings();
        }
        else if (strCmp(commandStr, "POST") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length != 0)
            {
                while (currentWord.Length != 0)
                {
                    ADVWORD_INPUT();
                }
                printf("Format perintah POST salah. Gunakan 'POST;' tanpa argumen.\n");
            }
            else
            {
                commandPost();
            }
        }
        else if (strCmp(commandStr, "VIEW_POST") == 0)
        {
            commandViewPost();
        }
        else if (strCmp(commandStr, "DELETE_POST") == 0)
        {
            commandDeletePost();
        }
        else if (strCmp(commandStr, "EXIT") == 0 || strCmp(commandStr, "QUIT") == 0)
        {
            printf("Keluar dari program.\n");
            quit = true;
        }
        else if (strCmp(commandStr, "REGISTER") == 0)
        {
            registerUser();
        }
        else if (strCmp(commandStr, "LOGIN") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length != 0)
            {
                while (currentWord.Length != 0)
                {
                    ADVWORD_INPUT();
                }
                printf("Format perintah LOGIN salah. Gunakan 'LOGIN;' tanpa argumen.\n");
            }
            else
            {
                loginUser();
            }
        }
        else if (strCmp(commandStr, "LOGOUT") == 0)
        {
            logoutUser();
        }
        else if (strCmp(commandStr, "PROFILE") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length == 0)
            {
                printf("Username tidak boleh kosong.\n");
            }
            else
            {
                char username[256];
                wordToString(username, currentWord);

                ADVWORD_INPUT();

                if (currentWord.Length != 0)
                {
                    while (currentWord.Length != 0)
                    {
                        ADVWORD_INPUT();
                    }
                    printf("Format perintah PROFILE salah. Gunakan 'PROFILE <username>;' tanpa argumen lain.\n");
                }
                else
                {
                    showUserProfile(username);
                }
            }
        }
        else if (strCmp(commandStr, "LOAD") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length != 0)
            {
                while (currentWord.Length != 0)
                {
                    ADVWORD_INPUT();
                }
                printf("Format perintah LOAD salah. Gunakan 'LOAD;' tanpa argumen.\n");
            }
            else
            {
                commandLoad();
            }
        }
        else if (strCmp(commandStr, "SAVE") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length != 0)
            {
                while (currentWord.Length != 0)
                {
                    ADVWORD_INPUT();
                }
                printf("Format perintah SAVE salah. Gunakan 'SAVE;' tanpa argumen.\n");
            }
            else
            {
                commandSave();
            }
        }
        else if (strCmp(commandStr, "CREATE_SUBGRODDIT") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length == 0)
            {
                printf("[Error] Nama subgroddit tidak boleh kosong!\n");
            }
            else
            {
                // Restore the word to currentWord for commandCreateSubgroddit
                Word tempWord = currentWord;

                ADVWORD_INPUT();

                if (currentWord.Length != 0)
                {
                    while (currentWord.Length != 0)
                    {
                        ADVWORD_INPUT();
                    }
                    printf("Format perintah CREATE_SUBGRODDIT salah. Gunakan 'CREATE_SUBGRODDIT <NAME>;' tanpa argumen lain.\n");
                }
                else
                {
                    // Process subgroddit creation with the name
                    char subgroddit_name[256];
                    wordToString(subgroddit_name, tempWord);

                    // Validate that name starts with "r/"
                    if (tempWord.Length < 2 || subgroddit_name[0] != 'r' || subgroddit_name[1] != '/')
                    {
                        printf("[Error] Nama Subgroddit HARUS UNIK dan diawali dengan \"r/\".\n");
                    }
                    else if (findSubgrodditIndexByName(subgroddit_name) != -1)
                    {
                        printf("[Error] Nama Subgroddit HARUS UNIK dan diawali dengan \"r/\".\n");
                    }
                    else
                    {
                        // Create new subgroddit
                        SubGroddit newSubgroddit;
                        initSubGroddit(&newSubgroddit);

                        // Generate subgroddit_id with format S00X
                        char id_str[256];
                        sprintf(id_str, "S%03d", SUBGRODDIT_COUNT + 1);
                        stringToWord(&newSubgroddit.subgroddit_id, id_str);

                        // Set name
                        stringToWord(&newSubgroddit.name, subgroddit_name);

                        // Insert into linked list
                        ListElement elem;
                        elem.type = TYPE_SUBGRODDIT;
                        elem.data.subgroddit = newSubgroddit;
                        insertLastList(&SUBGRODDITS, elem);

                        SUBGRODDIT_COUNT++;

                        printf("Subgroddit %s berhasil dibuat!\n", subgroddit_name);
                    }
                }
            }
        }
        else if (strCmp(commandStr, "VIEW_SUBGRODDIT") == 0)
        {
            viewSubgroddit();
        }
        else if (strCmp(commandStr, "COMMENT") == 0)
        {
            commandAddComment();
        }
        else if (strCmp(commandStr, "DELETE_COMMENT") == 0)
        {
            commandDeleteComment();
        }
        else if (strCmp(commandStr, "UPVOTE_POST") == 0)
        {
            commandUpvotePost();
        }
        else if (strCmp(commandStr, "DOWNVOTE_POST") == 0)
        {
            commandDownvotePost();
        }
        else if (strCmp(commandStr, "UNDO_VOTE_POST") == 0)
        {
            commandUndoVotePost();
        }
        else if (strCmp(commandStr, "UPVOTE_COMMENT") == 0)
        {
            commandUpvoteComment();
        }
        else if (strCmp(commandStr, "DOWNVOTE_COMMENT") == 0)
        {
            commandDownvoteComment();
        }
        else if (strCmp(commandStr, "UNDO_VOTE_COMMENT") == 0)
        {
            commandUndoVoteComment();
        }
        else if (strCmp(commandStr, "FOLLOW") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length == 0)
            {
                printf("Format perintah FOLLOW salah. Gunakan 'FOLLOW <username>;'\n");
            }
            else
            {
                Word tempWord = currentWord;

                ADVWORD_INPUT();
                if (currentWord.Length != 0)
                {
                    while (currentWord.Length != 0)
                    {
                        ADVWORD_INPUT();
                    }
                    printf("Format perintah FOLLOW salah. Gunakan 'FOLLOW <username>;'\n");
                }
                else
                {
                    char username[256];
                    wordToString(username, tempWord);
                    socialFollowUser(username);
                }
            }
        }
        else if (strCmp(commandStr, "UNFOLLOW") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length == 0)
            {
                printf("Format perintah UNFOLLOW salah. Gunakan 'UNFOLLOW <username>;'\n");
            }
            else
            {
                Word tempWord = currentWord;

                ADVWORD_INPUT();
                if (currentWord.Length != 0)
                {
                    while (currentWord.Length != 0)
                    {
                        ADVWORD_INPUT();
                    }
                    printf("Format perintah UNFOLLOW salah. Gunakan 'UNFOLLOW <username>;'\n");
                }
                else
                {
                    char username[256];
                    wordToString(username, tempWord);
                    socialUnfollowUser(username);
                }
            }
        }
        else if (strCmp(commandStr, "FOLLOWING") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length == 0)
            {
                if (!isLoggedIn())
                {
                    printf("Anda belum login. Silakan login terlebih dahulu.\n");
                }
                else
                {
                    socialShowFollowing(NULL);
                }
            }
            else
            {
                if (!isLoggedIn())
                {
                    printf("Anda belum login. Silakan login terlebih dahulu.\n");

                    while (currentWord.Length != 0)
                        ADVWORD_INPUT();
                }
                else
                {
                    Word tempWord = currentWord;

                    ADVWORD_INPUT();
                    if (currentWord.Length != 0)
                    {
                        while (currentWord.Length != 0)
                        {
                            ADVWORD_INPUT();
                        }
                        printf("Format perintah FOLLOWING salah. Gunakan 'FOLLOWING;' atau 'FOLLOWING <username>;'\n");
                    }
                    else
                    {
                        char username[256];
                        wordToString(username, tempWord);
                        socialShowFollowing(username);
                    }
                }
            }
        }
        else if (strCmp(commandStr, "FOLLOWERS") == 0)
        {
            ADVWORD_INPUT();

            if (currentWord.Length == 0)
            {
                if (!isLoggedIn())
                {
                    printf("Anda belum login. Silakan login terlebih dahulu.\n");
                }
                else
                {
                    socialShowFollowers(NULL);
                }
            }
            else
            {
                if (!isLoggedIn())
                {
                    printf("Anda belum login. Silakan login terlebih dahulu.\n");

                    while (currentWord.Length != 0)
                        ADVWORD_INPUT();
                }
                else
                {
                    Word tempWord = currentWord;

                    ADVWORD_INPUT();
                    if (currentWord.Length != 0)
                    {
                        while (currentWord.Length != 0)
                        {
                            ADVWORD_INPUT();
                        }
                        printf("Format perintah FOLLOWERS salah. Gunakan 'FOLLOWERS;' atau 'FOLLOWERS <username>;'\n");
                    }
                    else
                    {
                        char username[256];
                        wordToString(username, tempWord);
                        socialShowFollowers(username);
                    }
                }
            }
        }
        else if (strCmp(commandStr, "SHOW_FEED") == 0)
        {
            commandShowFeed();
        }
        else if (strCmp(commandStr, "FRIEND_RECOMMENDATION") == 0)
        {
            commandFriendRecommendation();
        }
        else
        {
            printf("Perintah '%s' tidak dikenali.\n", commandStr);
        }
    }

    return 0;
}
