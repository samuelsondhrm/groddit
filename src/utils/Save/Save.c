#include "Save.h"

int createFolderIfNotExists(const char *folder)
{
    char path[150];
    buildPath(path, folder, "");

    int len = 0;
    while (path[len] != '\0')
        len++;
    if (len > 0 && path[len - 1] == '/')
    {
        path[len - 1] = '\0';
    }

    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
    {
        return 0;
    }

    // Create folder
    if (mkdir(path, 0755) == 0)
    {
        return 1;
    }
    return -1;
}

void performSave(const char *folder)
{
    printf("Anda akan melakukan penyimpanan di %s.\n\n", folder);
    printf("Mohon tunggu...\n");

    for (int i = 1; i <= 3; i++)
    {
        printf("%d...\n", i);
        sleep(1);
    }
    printf("\n");

    // Save comments
    char pathComments[150];
    buildPath(pathComments, folder, "comment.csv");
    FILE *fileComments = fopen(pathComments, "w");
    if (fileComments != NULL)
    {
        fprintf(fileComments, "comment_id,post_id,author_id,parent_comment_id,content,upvotes,downvotes\n");
        for (int i = 0; i < COMMENT_COUNT; i++)
        {
            char post_id[256], author_id[256], content[256];
            wordToString(post_id, COMMENTS[i].post_id);
            wordToString(author_id, COMMENTS[i].author_id);
            wordToString(content, COMMENTS[i].content);

            fprintf(fileComments, "%d,%s,%s,%d,%s,%d,%d\n",
                    COMMENTS[i].comment_id,
                    post_id,
                    author_id,
                    COMMENTS[i].parent_comment_id,
                    content,
                    COMMENTS[i].upvotes,
                    COMMENTS[i].downvotes);
        }
        fclose(fileComments);
    }

    // Save posts
    char pathPosts[150];
    buildPath(pathPosts, folder, "post.csv");
    FILE *filePosts = fopen(pathPosts, "w");
    if (filePosts != NULL)
    {
        fprintf(filePosts, "post_id,subgroddit_id,author_id,title,content,created_at,upvotes,downvotes\n");
        Node *current = POSTS.head;
        while (current != NULL)
        {
            if (current->element.type == TYPE_POST)
            {
                Post P = current->element.data.post;
                char post_id[256], subgroddit_id[256], author_id[256], title[256], content[256], created_at[256];
                wordToString(post_id, P.post_id);
                wordToString(subgroddit_id, P.subgroddit_id);
                wordToString(author_id, P.author_id);
                wordToString(title, P.title);
                wordToString(content, P.content);
                timeToStr(created_at, P.created_at);

                fprintf(filePosts, "%s,%s,%s,%s,%s,%s,%d,%d\n",
                        post_id,
                        subgroddit_id,
                        author_id,
                        title,
                        content,
                        created_at,
                        P.upvotes,
                        P.downvotes);
            }

            current = current->next;
        }
        fclose(filePosts);
    }

    // Save users
    char pathUsers[150];
    buildPath(pathUsers, folder, "user.csv");
    FILE *fileUsers = fopen(pathUsers, "w");
    if (fileUsers != NULL)
    {
        fprintf(fileUsers, "user_id,username,password,karma,created_at\n");
        for (int i = 0; i < USER_COUNT; i++)
        {
            char user_id[256], username[256], password[256], created_at[256];
            wordToString(user_id, USERS[i].user_id);
            wordToString(username, USERS[i].username);
            wordToString(password, USERS[i].password);
            timeToStr(created_at, USERS[i].created_at);

            fprintf(fileUsers, "%s,%s,%s,%d,%s\n",
                    user_id,
                    username,
                    password,
                    USERS[i].karma,
                    created_at);
        }
        fclose(fileUsers);
    }

    // Save subgroddits
    char pathSubs[150];
    buildPath(pathSubs, folder, "subgroddit.csv");
    FILE *fileSubs = fopen(pathSubs, "w");
    if (fileSubs != NULL)
    {
        fprintf(fileSubs, "subgroddit_id,name\n");
        Node *current = SUBGRODDITS.head;
        while (current != NULL)
        {
            if (current->element.type == TYPE_SUBGRODDIT)
            {
                SubGroddit S = current->element.data.subgroddit;
                char subgroddit_id[256], name[256];
                wordToString(subgroddit_id, S.subgroddit_id);
                wordToString(name, S.name);

                fprintf(fileSubs, "%s,%s\n", subgroddit_id, name);
            }

            current = current->next;
        }
        fclose(fileSubs);
    } 
    
    // Save socials
    char pathSocials[150];
    buildPath(pathSocials, folder, "social.csv");
    FILE *fileSocials = fopen(pathSocials, "w");
    if (fileSocials != NULL)
    {
        fprintf(fileSocials, "user_id,following_id\n");
        for (int i = 0; i < SOCIAL_COUNT; i++)
        {
            char user_id[256], following_id[256];
            wordToString(user_id, SOCIALS[i].user_id);
            wordToString(following_id, SOCIALS[i].following_id);

            fprintf(fileSocials, "%s,%s\n", user_id, following_id);
        }
        fclose(fileSocials);
    }

    // Save votings
    char pathVotings[150];
    buildPath(pathVotings, folder, "voting.csv");
    FILE *fileVotings = fopen(pathVotings, "w");
    if (fileVotings != NULL)
    {
        fprintf(fileVotings, "user_id,target_type,target_id,vote_type\n");
        for (int i = 0; i < VOTING_COUNT; i++)
        {
            char user_id[256], target_type[256], target_id[256], vote_type[256];
            wordToString(user_id, VOTINGS[i].user_id);
            wordToString(target_type, VOTINGS[i].target_type);
            wordToString(target_id, VOTINGS[i].target_id);
            wordToString(vote_type, VOTINGS[i].vote_type);

            fprintf(fileVotings, "%s,%s,%s,%s\n", user_id, target_type, target_id, vote_type);
        }
        fclose(fileVotings);
    }

    printf("Penyimpanan telah berhasil dilakukan!\n\n");
}

void commandSave()
{
    int folderValid = 0;
    char folder[50];

    while (!folderValid)
    {
        printf("Masukkan nama folder penyimpanan.\n");
        fflush(stdout);

        STARTWORD_INPUT();

        if (currentWord.Length == 0)
        {
            printf("[Error] Nama folder tidak boleh kosong!\n");
            continue;
        }

        wordToString(folder, currentWord);

        int folderStatus = createFolderIfNotExists(folder);

        if (folderStatus == 1)
        {
            printf("Belum terdapat %s. Akan dilakukan pembuatan %s terlebih dahulu.\n\n", folder, folder);
            printf("Mohon tunggu...\n");
            for (int i = 1; i <= 3; i++)
            {
                printf("%d...\n", i);
                sleep(1);
            }
            printf("\n%s sudah berhasil dibuat.\n\n", folder);
        }
        else if (folderStatus == -1)
        {
            printf("[Error] Gagal membuat folder %s!\n", folder);
            continue;
        }

        folderValid = 1;
    }

    printf("\n");
    performSave(folder);
}
