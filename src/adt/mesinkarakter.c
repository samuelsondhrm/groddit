#include "mesinkarakter.h"
#include <stdio.h>

char currentChar;
boolean EOP;

static FILE *pita;
static int retval;

void START()
{
       pita = stdin;
       ADV();
}

void ADV()
{
       retval = fscanf(pita, "%c", &currentChar);
       if (retval == EOF)
       {
              EOP = true;
       }
       else
       {
              EOP = (currentChar == MARK);
       }
}