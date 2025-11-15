#ifndef LISTBERKAIT_H
#define LISTBERKAIT_H

#include "boolean.h"
#include <stdio.h>


/**
 * DEFINISI NODE
 */
typedef int Eltype;
typedef struct node *Address;
typedef struct node
{
    Eltype info;
    Address next;
} Node;

#define INFO(p) (p)->info
#define NEXT(p) (p)->next

Address newNode(Eltype val);

/**
 * DEFININSI LIST
 * List kosong  : FIRST(l) = NULL
 * Setiap elemen dengan Address p dapat diacu INFO(p), NEXT(p)
 * Elemen terakhir list: jika addressnya Last, maka NEXT(Last) = NULL
 */
typedef Address List;

#define IDX_UNDEF (-1)
#define FIRST(l) (l)

#endif