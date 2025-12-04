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

char jsonChar = '\0';
boolean JSON_EOF = false;
static FILE *jsonPita = NULL;

void STARTJSON(const char *filename)
{
       MODE = MODE_JSON_FILE;
       jsonPita = fopen(filename, "rb");

       if (jsonPita == NULL)
       {
              JSON_EOF = true;
              jsonChar = '\0';
              return;
       }

       JSON_EOF = false;
       int c = fgetc(jsonPita);
       if (c == EOF)
       {
              JSON_EOF = true;
              jsonChar = '\0';
       }
       else
       {
              jsonChar = (char)c;
       }
}

void ADVJSON()
{
       if (jsonPita == NULL)
       {
              JSON_EOF = true;
              jsonChar = '\0';
              return;
       }

       int c = fgetc(jsonPita);
       if (c == EOF)
       {
              JSON_EOF = true;
              jsonChar = '\0';
              fclose(jsonPita);
              jsonPita = NULL;
       }
       else
       {
              JSON_EOF = false;
              jsonChar = (char)c;
       }
}

void IgnoreWSJSON()
{
       while (!JSON_EOF && (jsonChar == ' ' || jsonChar == '\n' || jsonChar == '\t' || jsonChar == '\r'))
       {
              ADVJSON();
       }
}

int CopyJSONString(char *out, int maxLen)
{
       if (jsonChar != '"')
              return 0;

       int idx = 0;
       ADVJSON();

       while (!JSON_EOF && jsonChar != '"')
       {
              if (jsonChar == '\\')
              {
                     ADVJSON();
                     if (JSON_EOF)
                            break;

                     char esc = jsonChar;
                     char put = '?';

                     switch (esc)
                     {
                     case '"':
                            put = '"';
                            break;
                     case '\\':
                            put = '\\';
                            break;
                     case '/':
                            put = '/';
                            break;
                     case 'n':
                            put = '\n';
                            break;
                     case 'r':
                            put = '\r';
                            break;
                     case 't':
                            put = '\t';
                            break;
                     default:
                            put = esc;
                     }

                     if (idx < maxLen - 1)
                            out[idx++] = put;
              }
              else
              {
                     if (idx < maxLen - 1)
                            out[idx++] = jsonChar;
              }

              ADVJSON();
       }

       if (JSON_EOF)
              return 0;

       out[idx] = '\0';
       ADVJSON();
       return 1;
}