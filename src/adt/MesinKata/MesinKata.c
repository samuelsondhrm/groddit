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

void STARTWORD_CSV_BUFFER(uint8_t *buffer, int len)
{
    STARTCSV_BUFFER(buffer, len);

    if (EOP)
    {
        EndWordCSV = true;
        return;
    }

    EndWordCSV = false;
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
            if (MODE == MODE_CSV_BUFFER)
                ADVCSV_BUFFER();
            else
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
        if (MODE == MODE_CSV_BUFFER)
            ADVCSV_BUFFER();
        else
            ADVCSV();

        if (!EOP && currentChar != '\n' && currentChar != '\r')
            CopyFieldCSV();
        return;
    }

    CopyFieldCSV();
}

void CopyFieldCSV()
{
    currentWord.Length = 0;

    if (currentChar == '"')
    {
        if (MODE == MODE_CSV_BUFFER)
            ADVCSV_BUFFER();
        else
            ADVCSV();

        while (!EOP)
        {
            if (currentChar == '"')
            {
                if (MODE == MODE_CSV_BUFFER)
                    ADVCSV_BUFFER();
                else
                    ADVCSV();

                if (currentChar == '"')
                {
                    if (currentWord.Length < NMax)
                        currentWord.TabWord[currentWord.Length++] = '"';

                    if (MODE == MODE_CSV_BUFFER)
                        ADVCSV_BUFFER();
                    else
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

                if (MODE == MODE_CSV_BUFFER)
                    ADVCSV_BUFFER();
                else
                    ADVCSV();
            }
        }

        if (!EOP && currentChar == '"')
        {
            if (MODE == MODE_CSV_BUFFER)
                ADVCSV_BUFFER();
            else
                ADVCSV();
        }

        return;
    }

    while (!EOP &&
           currentChar != ',' &&
           currentChar != '\n' &&
           currentChar != '\r')
    {
        if (currentWord.Length < NMax)
            currentWord.TabWord[currentWord.Length++] = currentChar;

        if (MODE == MODE_CSV_BUFFER)
            ADVCSV_BUFFER();
        else
            ADVCSV();
    }
}

boolean EndWordCONF = false;

void IgnoreSpacesCONF()
{
    while (!EOP && (currentChar == ' ' || currentChar == '\t'))
    {
        ADVCONF();
    }
}

void STARTWORD_CONF(const char *filename)
{
    STARTCONF(filename);

    if (EOP)
    {
        EndWordCONF = true;
        currentWord.Length = 0;
        return;
    }

    IgnoreSpacesCONF();

    if (EOP || currentChar == '\n' || currentChar == '\r')
    {
        EndWordCONF = true;
        currentWord.Length = 0;
        return;
    }

    EndWordCONF = false;
    CopyTokenCONF();
}

void ADVWORD_CONF()
{
    IgnoreSpacesCONF();

    if (EOP || currentChar == '\n' || currentChar == '\r')
    {
        EndWordCONF = true;
        currentWord.Length = 0;
        return;
    }

    CopyTokenCONF();
}

void CopyTokenCONF()
{
    currentWord.Length = 0;

    while (!EOP &&
           currentChar != '=' &&
           currentChar != ' ' &&
           currentChar != '\t' &&
           currentChar != '\n' &&
           currentChar != '\r')
    {
        if (currentWord.Length < NMax)
            currentWord.TabWord[currentWord.Length++] = currentChar;

        ADVCONF();
    }
}