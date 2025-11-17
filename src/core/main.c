#include "main.h"

int main()
{
    initialize();
    char commandStr[50];
    boolean quit = false;

    while (!quit)
    {
        printf("\nMasukkan perintah: ");
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
        else if (strCmp(commandStr, "EXIT") == 0 || strCmp(commandStr, "QUIT") == 0)
        {
            printf("Keluar dari program.\n");
            quit = true;
        }
        else
        {
            printf("Perintah '%s' tidak dikenali.\n", commandStr);
        }
    }

    return 0;
}
