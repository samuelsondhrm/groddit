#include "header/ListBerkait.h"

ListElement makeUserElement(User u) {
    ListElement e;
    e.type = TYPE_USER;
    e.data.user = u;
    return e;
}

ListElement makePostElement(Post p) {
    ListElement e;
    e.type = TYPE_POST;
    e.data.post = p;
    return e;
}

ListElement makeCommentElement(Comment c) {
    ListElement e;
    e.type = TYPE_COMMENT;
    e.data.comment = c;
    return e;
}

ListElement makeSubgrodditElement(SubGroddit s) {
    ListElement e;
    e.type = TYPE_SUBGRODDIT;
    e.data.subgroddit = s;
    return e;
}

Node* newNode(ListElement elem) {
    Node *n = (Node*) malloc(sizeof(Node));
    if (n != NULL) {
        n->element = elem;
        n->next = NULL;
    }
    return n;
}

void createList(List *L) {
    L->head = NULL;
}

boolean isEmptyList(List L) {
    return (L.head == NULL);
}

int listLength(List L) {
    int count = 0;
    Node *p = L.head;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}


void insertFirstList(List *L, ListElement elem) {
    Node *n = newNode(elem);
    if (n != NULL) {
        n->next = L->head;
        L->head = n;
    }
}

void insertLastList(List *L, ListElement elem) {
    Node *n = newNode(elem);
    if (n == NULL) return;
    if (isEmptyList(*L)) {
        L->head = n;
    } else {
        Node *p = L->head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = n;
    }
}

void insertAtList(List *L, ListElement elem, int idx) {
    if (idx == 0) {
        insertFirstList(L, elem);
        return;
    }
    int i = 0;
    Node *p = L->head;
    while (p != NULL && i < idx - 1) {
        p = p->next;
        i++;
    }
    if (p != NULL) {
        Node *n = newNode(elem);
        if (n != NULL) {
            n->next = p->next;
            p->next = n;
        }
    }
}



void deleteFirstList(List *L, ListElement *out) {
    if (!isEmptyList(*L)) {
        Node *del = L->head;
        *out = del->element;
        L->head = del->next;
        free(del);
    }
}

void deleteLastList(List *L, ListElement *out) {
    if (isEmptyList(*L)) return;
    Node *p = L->head;
    if (p->next == NULL) {
        *out = p->element;
        free(p);
        L->head = NULL;
        return;
    }
    while (p->next->next != NULL) {
        p = p->next;
    }
    Node *del = p->next;
    *out = del->element;
    p->next = NULL;
    free(del);
}

void deleteAtList(List *L, int idx, ListElement *out) {
    if (idx == 0) {
        deleteFirstList(L, out);
        return;
    }
    int i = 0;
    Node *p = L->head;
    while (p != NULL && i < idx - 1) {
        p = p->next;
        i++;
    }
    if (p != NULL && p->next != NULL) {
        Node *del = p->next;
        *out = del->element;
        p->next = del->next;
        free(del);
    }
}



int indexOfList(
    List L,
    ListElement target,
    boolean (*cmp)(ListElement a, ListElement b)) {
        int idx = 0;
        Node *p = L.head;

        while (p != NULL) {
            if (cmp(p->element, target)) {
                return idx;
            }
            idx++;
            p = p->next;
        }

        return -1;
}



void displayList(List L, void (*print)(ListElement e)) {
    Node *p = L.head;

    while (p != NULL) {
        print(p->element);
        p = p->next;
    }
}


List concatList(List L1, List L2) {
    List L;
    createList(&L);

    Node *p = L1.head;
    while (p != NULL) {
        insertLastList(&L, p->element);
        p = p->next;
    }

    p = L2.head;
    while (p != NULL) {
        insertLastList(&L, p->element);
        p = p->next;
    }

    return L;
}

void freeList(List *L) {
    Node *p = L->head;

    while (p != NULL) {
        Node *next = p->next;
        free(p);
        p = next;
    }

    L->head = NULL;
}
