#include "main.h"

void printComments()
{
    printf("\n============== COMMENTS ==============\n");
    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        Comment c = COMMENTS[i];
        printf("ID: %d | Post: %s | Author: %s | Parent: %d\n",
               c.comment_id, c.post_id, c.author_id, c.parent_comment_id);
        printf("Content: %s\n", c.content);
        printf("Upvotes: %d | Downvotes: %d\n\n",
               c.upvotes, c.downvotes);
    }
}

// ======================== MAIN ========================
int main()
{
    initialize(); // load data

    char command[50];

    while (1)
    {
        printf("Masukkan perintah: ");

        IgnoreBlanks();
        IgnoreNewline();
        STARTWORD();
        IgnoreBlanks();
        IgnoreNewline();

        wordToString(command, currentWord);

        // cek perintah
        if (strCmp(command, "PRINTCOMMENTS") == 0)
        {
            printComments();
        }
        else
        {
            printf("Perintah '%s' tidak dikenali.\n", command);
        }

        printf("\n");
    }

    return 0;
}