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

            if (currentWord.Length != 0) {
                while (currentWord.Length != 0) {
                    ADVWORD_INPUT();
                }
                printf("Format perintah POST salah. Gunakan 'POST;' tanpa argumen.\n");
            } else {
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

            if (currentWord.Length != 0) {
                while (currentWord.Length != 0) {
                    ADVWORD_INPUT();
                }
                printf("Format perintah LOGIN salah. Gunakan 'LOGIN;' tanpa argumen.\n");
            } else {
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

            if (currentWord.Length == 0) {
                printf("Username tidak boleh kosong.\n");
            } else {
                char username[256];
                wordToString(username, currentWord);

                ADVWORD_INPUT();

                if (currentWord.Length != 0) {
                    while (currentWord.Length != 0) {
                        ADVWORD_INPUT();
                    }
                    printf("Format perintah PROFILE salah. Gunakan 'PROFILE <username>;' tanpa argumen lain.\n");
                } else {
                    showUserProfile(username);
                }
            }
        }
        else
        {
            printf("Perintah '%s' tidak dikenali.\n", commandStr);
        }
    }

    return 0;
}
