#include "MesinKarakter.h"

char currentChar;
boolean EOP = false;
FILE *pita = NULL;
int MODE = MODE_INPUT;

static uint8_t *csvBuffer = NULL;
static int csvPos = 0;
static int csvLen = 0;

void STARTINPUT()
{
       MODE = MODE_INPUT;
       pita = stdin;
       EOP = false;
       ADVINPUT();
}

void ADVINPUT()
{
       int c = fgetc(pita);
       if (c == EOF)
       {
              EOP = true;
              currentChar = '\0';
       }
       else
       {
              EOP = false;
              currentChar = (char)c;
       }
}

void STARTCSV(const char *filename)
{
       MODE = MODE_CSV_FILE;
       pita = fopen(filename, "rb");
       if (pita == NULL)
       {
              EOP = true;
              currentChar = '\0';
              return;
       }
       EOP = false;
       ADVCSV();
}

void ADVCSV()
{
       if (pita == NULL)
       {
              EOP = true;
              currentChar = '\0';
              return;
       }

       int c = fgetc(pita);
       if (c == EOF)
       {
              EOP = true;
              currentChar = '\0';
              fclose(pita);
              pita = NULL;
       }
       else
       {
              EOP = false;
              currentChar = (char)c;
       }
}

void STARTCONF(const char *filename)
{
       MODE = MODE_CONF_FILE;
       pita = fopen(filename, "rb");
       if (pita == NULL)
       {
              EOP = true;
              currentChar = '\0';
              return;
       }
       EOP = false;
       ADVCONF();
}

void ADVCONF()
{
       if (pita == NULL)
       {
              EOP = true;
              currentChar = '\0';
              return;
       }

       int c = fgetc(pita);
       if (c == EOF)
       {
              EOP = true;
              currentChar = '\0';
              fclose(pita);
              pita = NULL;
       }
       else
       {
              EOP = false;
              currentChar = (char)c;
       }
}

void STARTCSV_BUFFER(uint8_t *buffer, int len)
{
       MODE = MODE_CSV_BUFFER;
       csvBuffer = buffer;
       csvLen = len;
       csvPos = 0;

       if (len <= 0)
       {
              EOP = true;
              currentChar = '\0';
              return;
       }

       EOP = false;
       currentChar = (char)csvBuffer[0];
}

void ADVCSV_BUFFER()
{
       if (MODE != MODE_CSV_BUFFER || csvBuffer == NULL)
       {
              EOP = true;
              currentChar = '\0';
              return;
       }

       csvPos++;
       if (csvPos >= csvLen)
       {
              EOP = true;
              currentChar = '\0';
       }
       else
       {
              EOP = false;
              currentChar = (char)csvBuffer[csvPos];
       }
}

void CLOSEPITA()
{
       if ((MODE == MODE_CSV_FILE || MODE == MODE_CONF_FILE) && pita != NULL)
       {
              fclose(pita);
              pita = NULL;
       }
       csvBuffer = NULL;
       csvPos = 0;
       csvLen = 0;
       EOP = true;
       currentChar = '\0';
}

void IgnoreBlanks()
{
       while (!EOP && currentChar == BLANK)
       {
              if (MODE == MODE_INPUT)
                     ADVINPUT();
              else if (MODE == MODE_CSV_FILE)
                     ADVCSV();
              else if (MODE == MODE_CONF_FILE)
                     ADVCONF();
              else if (MODE == MODE_CSV_BUFFER)
                     ADVCSV_BUFFER();
              else
                     break;
       }
}

void IgnoreNewline()
{
       while (!EOP && (currentChar == '\n' || currentChar == '\r'))
       {
              if (MODE == MODE_INPUT)
                     ADVINPUT();
              else if (MODE == MODE_CSV_FILE)
                     ADVCSV();
              else if (MODE == MODE_CONF_FILE)
                     ADVCONF();
              else if (MODE == MODE_CSV_BUFFER)
                     ADVCSV_BUFFER();
              else
                     break;
       }
}