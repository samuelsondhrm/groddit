#include "header/ListBerkait.h"

#include <stdio.h>
#include <stdlib.h>

Address newNode(ElType val) {
    Address p = (Address) malloc(sizeof(Node));
    if (p != NULL) {
        INFO(p) = val;
        NEXT(p) = NULL;
    }
    return p;
}

boolean isEmpty(List l) {
    return (FIRST(l) == NULL);
}

void CreateList(List *l) {
    FIRST(*l) = NULL;
}

ElType getElmt(List l, int idx) {
    int ctr = 0;
    Address p = FIRST(l);
    while (ctr < idx) {
        ctr++;
        p = NEXT(p);
    }
    return INFO(p);
}

void setElmt(List *l, int idx, ElType val) {
    int ctr = 0;
    Address p = FIRST(*l);
    while (ctr < idx) {
        ctr++;
        p = NEXT(p);
    }
    INFO(p) = val;
}

int indexOf(List l, ElType val) {
    int idx = 0;
    Address p = FIRST(l);
    boolean found = false;
    while (p != NULL && !found) {
        if (INFO(p) == val) {
            found = true;
        } else {
            idx++;
            p = NEXT(p);
        }
    }
    if (found) {
        return idx;
    } else {
        return IDX_UNDEF;
    }
}

void insertFirst(List *l, ElType val) {
    Address p = newNode(val);
    if (p != NULL) {
        NEXT(p) = FIRST(*l);
        FIRST(*l) = p;
    }
}

void insertLast(List *l, ElType val) {
    Address p, last;
    if (isEmpty(*l)) {
        insertFirst(l, val);
    } else {
        p = newNode(val);
        if (p != NULL) {
            last = FIRST(*l);
            while (NEXT(last) != NULL) {
                last = NEXT(last);
            }
            NEXT(last) = p;
        }
    }
}

void insertAt(List *l, ElType val, int idx) {
    int ctr;
    Address p, loc;
    if (idx == 0) {
        insertFirst(l, val);
    } else {
        p = newNode(val);
        if (p != NULL) {
            ctr = 0;
            loc = FIRST(*l);
            while (ctr < idx-1) {
                ctr++;
                loc = NEXT(loc);
            }
            NEXT(p) = NEXT(loc);
            NEXT(loc) = p;
        }
    }
}

void deleteFirst(List *l, ElType *val) {
    Address p = FIRST(*l);
    *val = INFO(p);
    FIRST(*l) = NEXT(p);
    free(p);
}

void deleteLast(List *l, ElType *val) {
    Address p = FIRST(*l);
    Address loc = NULL;
    while (NEXT(p) != NULL) {
        loc = p;
        p = NEXT(p);
    }
    if (loc == NULL) {
        FIRST(*l)= NULL;
    } else {
        NEXT(loc) = NULL;
    }
    *val = INFO(p);
    free(p);
}

void deleteAt(List *l, int idx, ElType *val) {
    int ctr;
    Address p, loc;
    if (idx == 0) {
        deleteFirst(l, val);
    } else {
        ctr = 0;
        loc = FIRST(*l);
        while (ctr < idx - 1) {
            ctr++;
            loc = NEXT(loc);
        }
        p = NEXT(loc);
        *val = INFO(p);
        NEXT(loc) = NEXT(p);
        free(p);
    }
}

void displayList(List l) {
    Address p = FIRST(l);
    printf("[");
    while (p != NULL) {
        printf("%d", INFO(p));
        if (NEXT(p) != NULL) {
            printf(",");
        }
        p = NEXT(p);
    }
    printf("]");
}

int length(List l) {
    int ctr = 0;
    Address p = FIRST(l);
    while (p != NULL) {
        ctr++;
        p = NEXT(p);
    }
    return ctr;
}

List concat(List l1, List l2) {
    Address p = FIRST(l1);
    List l3;
    CreateList(&l3);
    while (p != NULL) {
        insertLast(&l3, INFO(p));
        p = NEXT(p);
    }
    p = FIRST(l2);
    while (p != NULL) {
        insertLast(&l3, INFO(p));
        p = NEXT(p);
    }
    return l3;
}