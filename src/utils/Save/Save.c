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

char *serialize_users(size_t *out_len)
{
    char *buf = malloc(USER_COUNT * 256 + 100);
    int pos = 0;

    pos += sprintf(buf + pos, "user_id,username,password,karma,created_at\n");

    for (int i = 0; i < USER_COUNT; i++)
    {
        char uid[64], usn[64], pw[128], created[64];
        wordToString(uid, USERS[i].user_id);
        wordToString(usn, USERS[i].username);
        wordToString(pw, USERS[i].password);
        timeToStr(created, USERS[i].created_at);

        pos += sprintf(buf + pos, "%s,%s,%s,%d,%s\n",
                       uid, usn, pw,
                       USERS[i].karma,
                       created);
    }

    *out_len = pos;
    return buf;
}

char *serialize_comments(size_t *out_len)
{
    char *buf = malloc(COMMENT_COUNT * 256 + 100);
    int pos = 0;

    pos += sprintf(buf + pos, "comment_id,post_id,author_id,parent_comment_id,content,upvotes,downvotes\n");

    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        char pid[64], aid[64], content[256];
        wordToString(pid, COMMENTS[i].post_id);
        wordToString(aid, COMMENTS[i].author_id);
        wordToString(content, COMMENTS[i].content);

        pos += sprintf(buf + pos, "%d,%s,%s,%d,%s,%d,%d\n",
                       COMMENTS[i].comment_id,
                       pid,
                       aid,
                       COMMENTS[i].parent_comment_id,
                       content,
                       COMMENTS[i].upvotes,
                       COMMENTS[i].downvotes);
    }

    *out_len = pos;
    return buf;
}

char *serialize_posts(size_t *out_len)
{
    size_t estimate = (size_t)POST_COUNT * 512 + 256;
    char *buf = malloc(estimate);
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    int pos = 0;
    pos += sprintf(buf + pos, "post_id,subgroddit_id,author_id,title,content,created_at,upvotes,downvotes\n");

    Node *current = POSTS.head;
    while (current != NULL)
    {
        if (current->element.type == TYPE_POST)
        {
            Post P = current->element.data.post;

            char post_id[256], subgroddit_id[256], author_id[256], title[512], content[1024], created_at[64];
            wordToString(post_id, P.post_id);
            wordToString(subgroddit_id, P.subgroddit_id);
            wordToString(author_id, P.author_id);
            wordToString(title, P.title);
            wordToString(content, P.content);
            timeToStr(created_at, P.created_at);

            if (pos + 2048 >= (int)estimate)
            {
                estimate *= 2;
                char *newbuf = realloc(buf, estimate);
                if (!newbuf)
                {
                    free(buf);
                    *out_len = 0;
                    return NULL;
                }
                buf = newbuf;
            }

            pos += sprintf(buf + pos, "%s,%s,%s,%s,%s,%s,%d,%d\n",
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

    *out_len = pos;
    return buf;
}

char *serialize_subgroddits(size_t *out_len)
{
    size_t estimate = (size_t)(64) * (SUBGRODDIT_COUNT + 4) + 128;
    char *buf = malloc(estimate);
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    int pos = 0;
    pos += sprintf(buf + pos, "subgroddit_id,name\n");

    Node *current = SUBGRODDITS.head;
    while (current != NULL)
    {
        if (current->element.type == TYPE_SUBGRODDIT)
        {
            SubGroddit S = current->element.data.subgroddit;
            char sid[256], name[256];
            wordToString(sid, S.subgroddit_id);
            wordToString(name, S.name);

            if (pos + 512 >= (int)estimate)
            {
                estimate *= 2;
                char *newbuf = realloc(buf, estimate);
                if (!newbuf)
                {
                    free(buf);
                    *out_len = 0;
                    return NULL;
                }
                buf = newbuf;
            }

            pos += sprintf(buf + pos, "%s,%s\n", sid, name);
        }
        current = current->next;
    }

    *out_len = pos;
    return buf;
}

char *serialize_socials(size_t *out_len)
{
    size_t estimate = (size_t)SOCIAL_COUNT * 128 + 128;
    char *buf = malloc(estimate);
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    int pos = 0;
    pos += sprintf(buf + pos, "user_id,following_id\n");

    for (int i = 0; i < SOCIAL_COUNT; i++)
    {
        char uid[256], fid[256];
        wordToString(uid, SOCIALS[i].user_id);
        wordToString(fid, SOCIALS[i].following_id);

        if (pos + 256 >= (int)estimate)
        {
            estimate *= 2;
            char *newbuf = realloc(buf, estimate);
            if (!newbuf)
            {
                free(buf);
                *out_len = 0;
                return NULL;
            }
            buf = newbuf;
        }

        pos += sprintf(buf + pos, "%s,%s\n", uid, fid);
    }

    *out_len = pos;
    return buf;
}

char *serialize_votings(size_t *out_len)
{
    size_t estimate = (size_t)VOTING_COUNT * 160 + 128;
    char *buf = malloc(estimate);
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    int pos = 0;
    pos += sprintf(buf + pos, "user_id,target_type,target_id,vote_type\n");

    for (int i = 0; i < VOTING_COUNT; i++)
    {
        char uid[256], ttype[64], tid[256], vtype[64];
        wordToString(uid, VOTINGS[i].user_id);
        wordToString(ttype, VOTINGS[i].target_type);
        wordToString(tid, VOTINGS[i].target_id);
        wordToString(vtype, VOTINGS[i].vote_type);

        if (pos + 512 >= (int)estimate)
        {
            estimate *= 2;
            char *newbuf = realloc(buf, estimate);
            if (!newbuf)
            {
                free(buf);
                *out_len = 0;
                return NULL;
            }
            buf = newbuf;
        }

        pos += sprintf(buf + pos, "%s,%s,%s,%s\n", uid, ttype, tid, vtype);
    }

    *out_len = pos;
    return buf;
}

void performSave(const char *folder)
{
    printf("Menyimpan data ke folder %s...\n", folder);

    char path[200];
    size_t len;
    char *buf;

    // 1. USERS
    buildPath(path, folder, "user.csv");
    buf = serialize_users(&len);
    write_encrypted_file(path, (uint8_t *)buf, len);
    free(buf);

    // 2. COMMENTS
    buildPath(path, folder, "comment.csv");
    buf = serialize_comments(&len);
    write_encrypted_file(path, (uint8_t *)buf, len);
    free(buf);

    // 3. POSTS
    buildPath(path, folder, "post.csv");
    buf = serialize_posts(&len);
    write_encrypted_file(path, (uint8_t *)buf, len);
    free(buf);

    // 4. SUBGRODDITS
    buildPath(path, folder, "subgroddit.csv");
    buf = serialize_subgroddits(&len);
    write_encrypted_file(path, (uint8_t *)buf, len);
    free(buf);

    // 5. SOCIALS
    buildPath(path, folder, "social.csv");
    buf = serialize_socials(&len);
    write_encrypted_file(path, (uint8_t *)buf, len);
    free(buf);

    // 6. VOTINGS
    buildPath(path, folder, "voting.csv");
    buf = serialize_votings(&len);
    write_encrypted_file(path, (uint8_t *)buf, len);
    free(buf);

    // 7. SAVE SECURITY.CONF
    buildPath(path, folder, "security.conf");
    save_security_conf(path);

    printf("Penyimpanan selesai!\n");
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