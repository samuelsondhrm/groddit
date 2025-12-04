#include <stdio.h>
#include "Helper.h"

int compareWord(Word a, Word b)
{
    if (a.Length != b.Length)
        return 0;

    for (int i = 0; i < a.Length; i++)
        if (a.TabWord[i] != b.TabWord[i])
            return 0;

    return 1;
}

int wordToInt(Word w)
{
    int sign = 1;
    int result = 0;
    int i = 0;

    if (w.Length == 0)
        return 0;

    if (w.TabWord[0] == '-')
    {
        sign = -1;
        i = 1;
    }

    for (; i < w.Length; i++)
    {
        if (w.TabWord[i] < '0' || w.TabWord[i] > '9')
            break;
        result = result * 10 + (w.TabWord[i] - '0');
    }

    return result * sign;
}

void uint32ToWord(Word *out, uint32_t value)
{
    char buf[12]; // max "4294967295"
    sprintf(buf, "%u", value);

    out->Length = 0;

    for (int i = 0; buf[i] != '\0' && i < NMax; i++)
    {
        out->TabWord[i] = buf[i];
        out->Length++;
    }

    out->TabWord[out->Length] = '\0';
}

void copyWord(Word *dest, Word src)
{
    dest->Length = src.Length;
    if (src.Length > NMax - 1)
    {
        dest->Length = NMax - 1;
    }
    for (int i = 0; i < dest->Length; i++)
    {
        dest->TabWord[i] = src.TabWord[i];
    }
    dest->TabWord[dest->Length] = '\0';
}

int isWordEmpty(Word w)
{
    return (w.Length == 0);
}

void wordToString(char *out, Word w)
{
    for (int i = 0; i < w.Length; i++)
        out[i] = w.TabWord[i];
    out[w.Length] = '\0';
}

void wordToString_safe(char *out, int maxSize, Word w)
{
    int len = w.Length;
    if (len >= maxSize)
    {
        len = maxSize - 1;
    }
    for (int i = 0; i < len; i++)
        out[i] = w.TabWord[i];
    out[len] = '\0';
}

void stringToWord(Word *out, const char *str)
{
    int i = 0;
    while (str[i] != '\0' && i < NMax)
    {
        out->TabWord[i] = str[i];
        i++;
    }
    out->Length = i;
}

int ensureCapacity(void **array, int *capacity, int elementSize, int needed)
{
    if (needed <= *capacity)
        return 1;
    int newCap = (*capacity == 0 ? 4 : (*capacity * 2));
    while (newCap < needed)
        newCap *= 2;

    void *newArr = realloc(*array, newCap * elementSize);
    if (newArr == NULL)
        return 0;

    *array = newArr;
    *capacity = newCap;
    return 1;
}

int strCmp(const char *a, const char *b)
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
            return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

void timeToStr(char *out, time_t t)
{
    struct tm *tm_info = localtime(&t);

    strftime(out, 20, "%Y-%m-%d %H:%M:%S", tm_info);
}

time_t parseTime(const char *str)
{
    struct tm t;
    t.tm_isdst = -1;

    // Parsing manual: YYYY-MM-DD HH:MM:SS
    int year =
        (str[0] - '0') * 1000 +
        (str[1] - '0') * 100 +
        (str[2] - '0') * 10 +
        (str[3] - '0');

    int month =
        (str[5] - '0') * 10 +
        (str[6] - '0');

    int day =
        (str[8] - '0') * 10 +
        (str[9] - '0');

    int hour =
        (str[11] - '0') * 10 +
        (str[12] - '0');

    int minute =
        (str[14] - '0') * 10 +
        (str[15] - '0');

    int second =
        (str[17] - '0') * 10 +
        (str[18] - '0');

    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    return mktime(&t);
}

void readLineWord(Word *out)
{
    out->Length = 0;

    STARTWORD_INPUT();

    if (currentWord.Length == 0)
    {
        return;
    }

    // Salin kata pertama
    copyWord(out, currentWord);

    // Gabungkan token berikutnya hingga tanda ';' tercapai
    while (!EndWordInput)
    {
        ADVWORD_INPUT();

        if (EndWordInput || currentWord.Length == 0)
        {
            break;
        }

        if (out->Length < NMax)
        {
            out->TabWord[out->Length++] = ' ';
        }

        for (int i = 0; i < currentWord.Length && out->Length < NMax; i++)
        {
            out->TabWord[out->Length++] = currentWord.TabWord[i];
        }
    }
}