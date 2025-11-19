#include "header/MesinKarakter.h"

char currentChar;
boolean EOP = false;
FILE *pita = NULL;
int MODE = MODE_INPUT;
static boolean inputInitialized = false;

void STARTINPUT()
{
       MODE = MODE_INPUT;
       pita = stdin;
       EOP = false;
       inputInitialized = true;
       ADVINPUT();
}

void ADVINPUT()
{
       int ret = fscanf(pita, "%c", &currentChar);

       if (ret == CHAR_MARK)
       {
              EOP = true;
       }
       IgnoreBlanks();
       IgnoreNewline();
}

void STARTCSV(const char *filename)
{
       MODE = MODE_CSV;
       pita = fopen(filename, "r");

       if (pita == NULL)
       {
              printf("ERROR: file %s tidak bisa dibuka\n", filename);
              EOP = true;
              return;
       }

       EOP = false;
       ADVCSV();
}

void ADVCSV()
{
       int ret = fscanf(pita, "%c", &currentChar);

       if (ret == EOF)
       {
              EOP = true;
              currentChar = EOF;
       }
}

void CLOSEPITA()
{
       if (MODE == MODE_CSV && pita != NULL)
       {
              fclose(pita);
              pita = NULL;
       }
}

void IgnoreBlanks()
{

       while (currentChar == BLANK)
       {
              if (MODE == MODE_INPUT)
                     ADVINPUT();
              else
                     ADVCSV();
       }
}

void IgnoreNewline()
{
       while (currentChar == NEWLINE || currentChar == '\r')
       {
              if (MODE == MODE_INPUT)
                     ADVINPUT();
              else
                     ADVCSV();
       }
}