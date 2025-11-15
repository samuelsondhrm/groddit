#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/helper.h"
#include "../adt/header/adt-sederhana.h"

// Menghapus carriage return
static void removeCR(char *s)
{
    int len = strlen(s);
    if (len > 0 && s[len - 1] == '\r')
    {
        s[len - 1] = '\0';
    }
}

int readField(char *dest, int maxLen, FILE *f)
{
    int len = 0;
    int c = fgetc(f);

    if (c == EOF)
    {
        dest[0] = '\0';
        return 1;
    }

    int quoted = 0;

    if (c == '"')
    {
        quoted = 1;
        c = fgetc(f);
    }

    while (1)
    {
        if (quoted)
        {
            if (c == '"')
            {
                int peek = fgetc(f);
                if (peek == '"')
                {
                    if (len < maxLen - 1)
                        dest[len++] = '"';
                    c = fgetc(f);
                    continue;
                }
                else
                {
                    if (peek != EOF)
                        ungetc(peek, f);
                    break;
                }
            }
        }
        else
        {
            if (c == ',' || c == '\n' || c == '\r' || c == EOF)
            {
                if (c != EOF)
                    ungetc(c, f);
                break;
            }
        }

        if (len < maxLen - 1)
            dest[len++] = c;
        c = fgetc(f);
        if (c == EOF)
            break;
    }

    dest[len] = '\0';

    removeCR(dest);

    c = fgetc(f);
    if (c == '\r')
    {
        int next = fgetc(f);
        (void)next;
        return 1;
    }
    if (c == '\n')
        return 1;
    if (c == EOF)
        return 1;

    return 0;
}

int toInt(const char *s)
{
    int val = 0;
    int neg = 0;
    int any = 0;

    while (*s == ' ' || *s == '\r' || *s == '\t')
        s++;

    if (*s == '-')
    {
        neg = 1;
        s++;
    }

    while (*s >= '0' && *s <= '9')
    {
        val = val * 10 + (*s - '0');
        any = 1;
        s++;
    }

    return any ? (neg ? -val : val) : 0;
}

int strCmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
        i++;
    }
    return s1[i] - s2[i];
}

int strLength(const char *s)
{
    int i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}

void copyString(char *dst, const char *src, int maxlen)
{
    int i = 0;
    while (src[i] != '\0' && i < maxlen - 1)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

void copyStringDynamic(char **dst, const char *src)
{
    int len = strLength(src);
    *dst = malloc(len + 1);
    if (*dst == NULL)
        return;

    for (int i = 0; i < len; i++)
    {
        (*dst)[i] = src[i];
    }
    (*dst)[len] = '\0';
}

int ensureCapacity(void **arrPtr, int *capacity, size_t elemSize, int needed)
{
    if (needed <= *capacity)
        return 1;

    int newCap = (*capacity == 0) ? 4 : (*capacity * 2);
    while (newCap < needed)
        newCap *= 2;

    void *newArr = realloc(*arrPtr, newCap * elemSize);
    if (!newArr)
        return 0;

    *arrPtr = newArr;
    *capacity = newCap;
    return 1;
}

int wordToString(char *dest, Word w)
{
    int start = 0, end = w.Length - 1;

    while (start <= end && (w.TabWord[start] == ' ' || w.TabWord[start] == '\n' || w.TabWord[start] == '\r'))
        start++;

    while (end >= start && (w.TabWord[end] == ' ' || w.TabWord[end] == '\n' || w.TabWord[end] == '\r'))
        end--;

    int len = 0;
    for (int i = start; i <= end; i++)
        dest[len++] = w.TabWord[i];

    dest[len] = '\0';
    return len;
}

char *convertTimeToStr(time_t t)
{
    static char buf[20]; // "YYYY-MM-DD HH:MM:SS"
    struct tm *tm_info = localtime(&t);

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return buf;
}