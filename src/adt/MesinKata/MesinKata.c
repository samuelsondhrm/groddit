#include "MesinKata.h"

Word currentWord;

boolean EndWordInput = false;

void IgnoreBlanksInput()
{
    while (!EOP && currentChar == BLANK)
    {
        ADVINPUT();
    }
}

void STARTWORD_INPUT()
{
    STARTINPUT();
    IgnoreBlanksInput();
    IgnoreNewline();

    if (currentChar == CHAR_MARK)
    {
        EndWordInput = true;
        currentWord.Length = 0;
    }
    else
    {
        EndWordInput = false;
        CopyWordInput();
        IgnoreBlanksInput();
        IgnoreNewline();
    }
}

void ADVWORD_INPUT()
{
    while (!EOP && (currentChar == '\n' || currentChar == '\r'))
    {
        ADVINPUT();
    }

    IgnoreBlanksInput();

    if (currentChar == CHAR_MARK)
    {
        EndWordInput = true;
        currentWord.Length = 0;
    }
    else if (EOP)
    {
        EndWordInput = true;
        currentWord.Length = 0;
    }
    else
    {
        CopyWordInput();
        IgnoreBlanksInput();
        IgnoreNewline();
    }
}

void CopyWordInput()
{
    currentWord.Length = 0;

    while (!EOP && currentChar != BLANK && currentChar != CHAR_MARK && currentChar != '\n' && currentChar != '\r')
    {
        if (currentWord.Length < NMax)
            currentWord.TabWord[currentWord.Length++] = currentChar;
        ADVINPUT();
    }
}

boolean EndWordCSV = false;

void STARTWORD_CSV(const char *filename)
{
    STARTCSV(filename);

    if (EOP)
    {
        EndWordCSV = true;
        return;
    }

    EndWordCSV = false;
    CopyFieldCSV();
}

void ADVWORD_CSV()
{
    if (EOP)
    {
        EndWordCSV = true;
        return;
    }

    if (currentChar == '\n' || currentChar == '\r')
    {
        while (!EOP && (currentChar == '\n' || currentChar == '\r'))
        {
            ADVCSV();
        }

        if (EOP)
        {
            EndWordCSV = true;
        }
        else
        {
            CopyFieldCSV();
        }
        return;
    }

    if (currentChar == ',')
    {
        ADVCSV();
        if (!EOP && currentChar != '\n' && currentChar != '\r')
        {
            CopyFieldCSV();
        }
        return;
    }

    CopyFieldCSV();
}

void CopyFieldCSV()
{
    currentWord.Length = 0;

    if (currentChar == '"')
    {
        ADVCSV();

        while (!EOP)
        {

            if (currentChar == '"')
            {
                ADVCSV();

                if (currentChar == '"')
                {
                    if (currentWord.Length < NMax)
                        currentWord.TabWord[currentWord.Length++] = '"';
                    ADVCSV();
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (currentWord.Length < NMax)
                    currentWord.TabWord[currentWord.Length++] = currentChar;
                ADVCSV();
            }
        }

        if (currentChar == '"')
            ADVCSV();

        return;
    }

    while (!EOP && currentChar != ',' &&
           currentChar != '\n' && currentChar != '\r')
    {

        if (currentWord.Length < NMax)
            currentWord.TabWord[currentWord.Length++] = currentChar;

        ADVCSV();
    }
}
