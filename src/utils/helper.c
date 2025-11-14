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
