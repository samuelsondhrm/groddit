#include "ContentModeration.h"
#include <ctype.h>
#include <stdlib.h>

Blacklist GLOBAL_BLACKLIST = {.count = 0};

boolean content_moderation_init(const char *pathConf)
{
    struct stat st;
    boolean exists = (stat(pathConf, &st) == 0);

    if (!exists)
    {
        GLOBAL_BLACKLIST.count = 0;
        return false;
    }

    if (!LoadBlacklistJSON(pathConf))
    {
        printf("[Warning] blacklist JSON ada tapi gagal dibaca!\n");
        GLOBAL_BLACKLIST.count = 0;
        return false;
    }

    return true;
}

void ToLowercaseInplace(char *s)
{
    if (!s)
        return;
    for (int i = 0; s[i]; i++)
    {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
}

int LoadBlacklistJSON(const char *filepath)
{
    STARTJSON(filepath);
    if (JSON_EOF)
        return 0;

    GLOBAL_BLACKLIST.count = 0;

    IgnoreWSJSON();

    while (!JSON_EOF)
    {
        IgnoreWSJSON();

        if (jsonChar == '"')
        {
            char key[256];
            if (!CopyJSONString(key, 256))
                break;

            IgnoreWSJSON();

            if (jsonChar == ':')
            {
                ADVJSON();
                IgnoreWSJSON();

                if (strcmp(key, "blacklisted_words") == 0)
                {
                    if (jsonChar == '[')
                    {
                        ADVJSON();
                        IgnoreWSJSON();

                        while (!JSON_EOF && jsonChar != ']')
                        {
                            if (jsonChar == '"')
                            {
                                char token[MAX_WORD_LEN];
                                if (CopyJSONString(token, MAX_WORD_LEN))
                                {
                                    ToLowercaseInplace(token);
                                    strcpy(GLOBAL_BLACKLIST.words[GLOBAL_BLACKLIST.count++], token);
                                }
                            }

                            IgnoreWSJSON();
                            if (jsonChar == ',')
                            {
                                ADVJSON();
                                IgnoreWSJSON();
                            }
                        }

                        if (jsonChar == ']')
                            ADVJSON();
                        break;
                    }
                }
            }
        }

        ADVJSON();
    }

    return 1;
}

static int already_found(char foundWords[][MAX_WORD_LEN], int foundCount, const char *s)
{
    for (int i = 0; i < foundCount; i++)
    {
        if (strcmp(foundWords[i], s) == 0)
            return 1;
    }
    return 0;
}

int CheckBlacklistedContent(const char *text, char foundWords[][MAX_WORD_LEN], int *foundCount)
{
    if (!text || !foundWords || !foundCount)
        return 0;

    *foundCount = 0;
    int len = (int)strlen(text);
    char tokenBuf[MAX_WORD_LEN];
    int tidx = 0;

    for (int i = 0; i <= len; i++)
    {
        char c = (i < len) ? text[i] : '\0';
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '_')
        {
            if (tidx < MAX_WORD_LEN - 1)
            {
                tokenBuf[tidx++] = (char)tolower((unsigned char)c);
            }
            else
            {
                /* token too long: truncate */
            }
        }
        else
        {
            if (tidx > 0)
            {
                tokenBuf[tidx] = '\0';
                for (int b = 0; b < GLOBAL_BLACKLIST.count; b++)
                {
                    if (strcmp(tokenBuf, GLOBAL_BLACKLIST.words[b]) == 0)
                    {
                        if (!already_found(foundWords, *foundCount, GLOBAL_BLACKLIST.words[b]))
                        {
                            strncpy(foundWords[*foundCount], GLOBAL_BLACKLIST.words[b], MAX_WORD_LEN - 1);
                            foundWords[*foundCount][MAX_WORD_LEN - 1] = '\0';
                            (*foundCount)++;
                            if (*foundCount >= MAX_FOUND)
                            {
                                return 1;
                            }
                        }
                    }
                }
                tidx = 0;
            }
        }
    }

    return (*foundCount > 0) ? 1 : 0;
}