// Save.c
#include "Save.h"
#include <stdarg.h>

static void ensure_buf_space(char **buf, size_t *cap, size_t need_pos)
{
    if (need_pos + 1 >= *cap)
    {
        size_t newcap = *cap ? *cap : 1024;
        while (newcap <= need_pos + 1)
            newcap *= 2;
        char *nb = realloc(*buf, newcap);
        if (!nb)
        {
            fprintf(stderr, "[FATAL] Out of memory in ensure_buf_space\n");
            exit(1);
        }
        *buf = nb;
        *cap = newcap;
    }
}

static void csv_escape_field(const char *src, char *dst, size_t dstsz)
{
    if (dstsz == 0)
        return;
    size_t di = 0;
    if (di < dstsz - 1)
        dst[di++] = '"';
    for (size_t i = 0; src[i] != '\0' && di + 2 < dstsz; i++)
    {
        if (src[i] == '"')
        {
            if (di + 2 >= dstsz)
                break;
            dst[di++] = '"';
            dst[di++] = '"';
        }
        else
        {
            dst[di++] = src[i];
        }
    }
    if (di < dstsz - 1)
        dst[di++] = '"';
    dst[di] = '\0';
}

static void append_fmt(char **buf, size_t *cap, size_t *pos, const char *fmt, ...)
{
    va_list ap;
    while (1)
    {
        ensure_buf_space(buf, cap, *pos + 512);
        va_start(ap, fmt);
        int written = vsnprintf(*buf + *pos, *cap - *pos, fmt, ap);
        va_end(ap);

        if (written < 0)
        {
            return;
        }
        if ((size_t)written < *cap - *pos)
        {
            *pos += (size_t)written;
            return;
        }
        ensure_buf_space(buf, cap, *pos + (size_t)written);
    }
}

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

    if (mkdir(path, 0755) == 0)
    {
        return 1;
    }
    return -1;
}

char *serialize_users(size_t *out_len)
{
    size_t cap = USER_COUNT * 256 + 256;
    char *buf = malloc(cap);
    size_t pos = 0;
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    append_fmt(&buf, &cap, &pos, "user_id,username,password,karma,created_at\n");

    for (int i = 0; i < USER_COUNT; i++)
    {
        char uid[256], username[256], password[512], created[64];
        wordToString_safe(uid, sizeof(uid), USERS[i].user_id);
        wordToString_safe(username, sizeof(username), USERS[i].username);
        wordToString_safe(password, sizeof(password), USERS[i].password);
        timeToStr(created, USERS[i].created_at);

        append_fmt(&buf, &cap, &pos, "%s,%s,%s,%d,%s\n",
                   uid, username, password,
                   USERS[i].karma,
                   created);
    }

    *out_len = pos;
    return buf;
}

char *serialize_comments(size_t *out_len)
{
    size_t cap = COMMENT_COUNT * 512 + 256;
    char *buf = malloc(cap);
    size_t pos = 0;
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    append_fmt(&buf, &cap, &pos, "comment_id,post_id,author_id,parent_comment_id,content,upvotes,downvotes\n");

    for (int i = 0; i < COMMENT_COUNT; i++)
    {
        char pid[256], aid[256], raw_content[2048];
        wordToString_safe(pid, sizeof(pid), COMMENTS[i].post_id);
        wordToString_safe(aid, sizeof(aid), COMMENTS[i].author_id);
        wordToString_safe(raw_content, sizeof(raw_content), COMMENTS[i].content);

        char esc_content[4096];
        csv_escape_field(raw_content, esc_content, sizeof(esc_content));

        append_fmt(&buf, &cap, &pos, "%d,%s,%s,%d,%s,%d,%d\n",
                   COMMENTS[i].comment_id,
                   pid,
                   aid,
                   COMMENTS[i].parent_comment_id,
                   esc_content,
                   COMMENTS[i].upvotes,
                   COMMENTS[i].downvotes);
    }

    *out_len = pos;
    return buf;
}

char *serialize_posts(size_t *out_len)
{
    size_t cap = (size_t)POST_COUNT * 1024 + 512;
    char *buf = malloc(cap);
    size_t pos = 0;
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    append_fmt(&buf, &cap, &pos, "post_id,subgroddit_id,author_id,title,content,created_at,upvotes,downvotes\n");

    Node *current = POSTS.head;
    while (current != NULL)
    {
        if (current->element.type == TYPE_POST)
        {
            Post P = current->element.data.post;

            char post_id[256], subgroddit_id[256], author_id[256];
            char title_raw[2048], content_raw[4096], created_at[64];

            wordToString_safe(post_id, sizeof(post_id), P.post_id);
            wordToString_safe(subgroddit_id, sizeof(subgroddit_id), P.subgroddit_id);
            wordToString_safe(author_id, sizeof(author_id), P.author_id);
            wordToString_safe(title_raw, sizeof(title_raw), P.title);
            wordToString_safe(content_raw, sizeof(content_raw), P.content);
            timeToStr(created_at, P.created_at);

            char esc_title[4096];
            char esc_content[8192];
            csv_escape_field(title_raw, esc_title, sizeof(esc_title));
            csv_escape_field(content_raw, esc_content, sizeof(esc_content));

            append_fmt(&buf, &cap, &pos, "%s,%s,%s,%s,%s,%s,%d,%d\n",
                       post_id,
                       subgroddit_id,
                       author_id,
                       esc_title,
                       esc_content,
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
    size_t cap = (size_t)(SUBGRODDIT_COUNT + 4) * 256 + 256;
    char *buf = malloc(cap);
    size_t pos = 0;
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    append_fmt(&buf, &cap, &pos, "subgroddit_id,name\n");

    Node *current = SUBGRODDITS.head;
    while (current != NULL)
    {
        if (current->element.type == TYPE_SUBGRODDIT)
        {
            SubGroddit S = current->element.data.subgroddit;
            char sid[256], name_raw[1024];
            wordToString_safe(sid, sizeof(sid), S.subgroddit_id);
            wordToString_safe(name_raw, sizeof(name_raw), S.name);

            append_fmt(&buf, &cap, &pos, "%s,%s\n", sid, name_raw);
        }
        current = current->next;
    }

    *out_len = pos;
    return buf;
}

char *serialize_socials(size_t *out_len)
{
    size_t cap = (size_t)SOCIAL_COUNT * 256 + 256;
    char *buf = malloc(cap);
    size_t pos = 0;
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    append_fmt(&buf, &cap, &pos, "user_id,following_id\n");

    for (int i = 0; i < SOCIAL_COUNT; i++)
    {
        char uid[256], fid[256];
        wordToString_safe(uid, sizeof(uid), SOCIALS[i].user_id);
        wordToString_safe(fid, sizeof(fid), SOCIALS[i].following_id);

        append_fmt(&buf, &cap, &pos, "%s,%s\n", uid, fid);
    }

    *out_len = pos;
    return buf;
}

char *serialize_votings(size_t *out_len)
{
    size_t cap = (size_t)VOTING_COUNT * 512 + 256;
    char *buf = malloc(cap);
    size_t pos = 0;
    if (!buf)
    {
        *out_len = 0;
        return NULL;
    }

    append_fmt(&buf, &cap, &pos, "user_id,target_type,target_id,vote_type\n");

    for (int i = 0; i < VOTING_COUNT; i++)
    {
        char uid[256], ttype[128], tid[256], vtype[128];
        wordToString_safe(uid, sizeof(uid), VOTINGS[i].user_id);
        wordToString_safe(ttype, sizeof(ttype), VOTINGS[i].target_type);
        wordToString_safe(tid, sizeof(tid), VOTINGS[i].target_id);
        wordToString_safe(vtype, sizeof(vtype), VOTINGS[i].vote_type);

        append_fmt(&buf, &cap, &pos, "%s,%s,%s,%s\n", uid, ttype, tid, vtype);
    }

    *out_len = pos;
    return buf;
}

void performSave(const char *folder)
{
    printSectionHeader("", "SAVING PROCESS");
    printf("\n%s %sDestination folder:%s %s%s%s\n", BOX_V, BOLD_WHITE, RESET, BOLD_CYAN, folder, RESET);
    printSectionDivider();
    printf("\n");
    spinnerAnimation("Preparing to save data", 10);
    printf("\n");

    char path[200];
    size_t len;
    char *buf;

    // 1. USERS
    printf("%s %sSaving users...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    buildPath(path, folder, "user.csv");
    buf = serialize_users(&len);
    if (buf)
    {
        write_encrypted_file(path, (uint8_t *)buf, len);
        printf(" %s[OK] %d users%s\n", GREEN, USER_COUNT, RESET);
        free(buf);
    }
    else
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }

    // 2. COMMENTS
    printf("%s %sSaving comments...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    buildPath(path, folder, "comment.csv");
    buf = serialize_comments(&len);
    if (buf)
    {
        write_encrypted_file(path, (uint8_t *)buf, len);
        printf(" %s[OK] %d comments%s\n", GREEN, COMMENT_COUNT, RESET);
        free(buf);
    }
    else
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }

    // 3. POSTS
    printf("%s %sSaving posts...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    buildPath(path, folder, "post.csv");
    buf = serialize_posts(&len);
    if (buf)
    {
        write_encrypted_file(path, (uint8_t *)buf, len);
        printf(" %s[OK] %d posts%s\n", GREEN, POST_COUNT, RESET);
        free(buf);
    }
    else
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }

    // 4. SUBGRODDITS
    printf("%s %sSaving subgroddits...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    buildPath(path, folder, "subgroddit.csv");
    buf = serialize_subgroddits(&len);
    if (buf)
    {
        write_encrypted_file(path, (uint8_t *)buf, len);
        printf(" %s[OK] %d subgroddits%s\n", GREEN, SUBGRODDIT_COUNT, RESET);
        free(buf);
    }
    else
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }

    // 5. SOCIALS
    printf("%s %sSaving social connections...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    buildPath(path, folder, "social.csv");
    buf = serialize_socials(&len);
    if (buf)
    {
        write_encrypted_file(path, (uint8_t *)buf, len);
        printf(" %s[OK] %d connections%s\n", GREEN, SOCIAL_COUNT, RESET);
        free(buf);
    }
    else
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }

    // 6. VOTINGS
    printf("%s %sSaving votes...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    buildPath(path, folder, "voting.csv");
    buf = serialize_votings(&len);
    if (buf)
    {
        write_encrypted_file(path, (uint8_t *)buf, len);
        printf(" %s[OK] %d votes%s\n", GREEN, VOTING_COUNT, RESET);
        free(buf);
    }
    else
    {
        printf(" %s[FAILED]%s\n", BOLD_RED, RESET);
    }

    // 7. SAVE SECURITY.CONF
    printf("%s %sSaving security config...%s", BOX_V, DIM, RESET);
    fflush(stdout);
    buildPath(path, folder, "security.conf");
    save_security_conf(path);
    printf(" %s[DONE]%s\n", GREEN, RESET);

    printSectionDivider();
    printf("\n");
    printSuccess("Data saved successfully!");
    printf("All data has been saved to %s%s%s\n", BOLD_CYAN, folder, RESET);
}

void commandSave()
{
    clearScreen();
    printBreadcrumb("Home > Save Data");
    
    printHorizontalLine(80, DBOX_TL, DBOX_H, DBOX_TR);
    printf("%s║%s                           %sSAVE DATA%s                               %s║%s\n", 
           BOLD_CYAN, RESET, BOLD_WHITE, RESET, BOLD_CYAN, RESET);
    printHorizontalLine(80, DBOX_BL, DBOX_H, DBOX_BR);
    printf("%s\n", RESET);

    int folderValid = 0;
    char folder[50];

    while (!folderValid)
    {
        printf("\n");
        printSectionHeader("", "FOLDER SELECTION");
        printf("\n%s %sEnter destination folder name:%s\n", BOX_V, BOLD_WHITE, RESET);
        printSectionDivider();
        printInputPrompt("FOLDER NAME");
        fflush(stdout);

        STARTWORD_INPUT();
        printf("\n");

        if (currentWord.Length == 0)
        {
            printError("Empty folder name");
            printf("Folder name cannot be empty!\n\n");
            printf("%sExample:%s %sconfig-new, backup-2024, etc.%s\n", 
                   DIM, RESET, BOLD_WHITE, RESET);
            continue;
        }

        wordToString(folder, currentWord);

        int folderStatus = createFolderIfNotExists(folder);

        if (folderStatus == 1)
        {
            printf("\n");
            printWarning("Creating new folder");
            printf("Folder \"%s%s%s\" does not exist. Creating it now...\n\n", 
                   BOLD_YELLOW, folder, RESET);
            spinnerAnimation("Creating folder structure", 10);
            printf("\n\n");
            printSuccess("Folder created!");
            printf("Directory %s%s%s is ready for saving.\n", 
                   BOLD_CYAN, folder, RESET);
        }
        else if (folderStatus == -1)
        {
            printError("Failed to create folder");
            printf("Could not create directory: %s%s%s\n\n", 
                   BOLD_RED, folder, RESET);
            printf("%sTip:%s Check write permissions or try a different name.\n", 
                   BOLD_CYAN, RESET);
            continue;
        }

        folderValid = 1;
    }

    printf("\n");
    performSave(folder);
}