#include "ListBerkait.h"

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
    if (n!=NULL) {
        n->element = elem;
        n->next = NULL;
    }
    return n;
}

void createList(List *L) {
    L->head = NULL;
}

boolean isEmptyList(List L) {
    return (L.head==NULL);
}

int listLength(List L) {
    int count = 0;
    Node *p = L.head;
    while (p!=NULL) {
        count++;
        p = p->next;
    }
    return count;
}

void insertFirstList(List *L, ListElement elem) {
    Node *n = newNode(elem);
    if (n!=NULL) {
        n->next = L->head;
        L->head = n;
    }
}

void insertLastList(List *L, ListElement elem) {
    Node *n = newNode(elem);
    if (n==NULL) return;
    
    if (isEmptyList(*L)) {
        L->head = n;
    } else {
        Node *p = L->head;
        while (p->next!=NULL) {
            p = p->next;
        }
        p->next = n;
    }
}

void insertAtList(List *L, ListElement elem, int idx) {
    if (idx==0) {
        insertFirstList(L, elem);
        return;
    }
    int i = 0;
    Node *p = L->head;
    while (p!=NULL && i<idx-1) {
        p = p->next;
        i++;
    }
    if (p != NULL) {
        Node *n = newNode(elem);
        if (n!=NULL) {
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
    if (p->next==NULL) {
        *out = p->element;
        free(p);
        L->head = NULL;
        return;
    }
    while (p->next->next!=NULL) {
        p = p->next;
    }
    Node *del = p->next;
    *out = del->element;
    p->next = NULL;
    free(del);
}

void deleteAtList(List *L, int idx, ListElement *out) {
    if (idx==0) {
        deleteFirstList(L, out);
        return;
    }
    int i = 0;
    Node *p = L->head;
    while (p!=NULL && i<idx-1) {
        p = p->next;
        i++;
    }
    if (p!=NULL && p->next!=NULL) {
        Node *del = p->next;
        *out = del->element;
        p->next = del->next;
        free(del);
    }
}

boolean getElementAt(List L, int idx, ListElement *out) {
    if (idx<0) return false;
    int i = 0;
    Node *p = L.head;
    while (p!=NULL && i<idx) {
        p = p->next;
        i++;
    }
    if (p!=NULL) {
        *out = p->element;
        return true;
    }
    return false;
}

boolean setElementAt(List *L, int idx, ListElement elem) {
    if (idx<0) return false;
    int i = 0;
    Node *p = L->head;
    while (p!=NULL && i<idx) {
        p = p->next;
        i++;
    }
    if (p!=NULL) {
        p->element = elem;
        return true;
    }
    return false;
}

int indexOfList(
    List L,
    ListElement target,
    boolean (*cmp)(ListElement a, ListElement b)) {
    int idx = 0;
    Node *p = L.head;
    while (p!=NULL) {
        if (cmp(p->element, target)) {
            return idx;
        }
        idx++;
        p = p->next;
    }
    return -1;
}

ListElement* findElement(
    List L,
    ListElement target,
    boolean (*cmp)(ListElement a, ListElement b)) {
    Node *p = L.head;
    while (p!=NULL) {
        if (cmp(p->element, target)) {
            return &(p->element);
        }
        p = p->next;
    }
    return NULL;
}

List filterList(
    List L,
    boolean (*predicate)(ListElement e, void *context),
    void *context) {
    List result;
    createList(&result);
    Node *p = L.head;
    while (p!=NULL) {
        if (predicate(p->element, context)) {
            insertLastList(&result, p->element);
        }
        p = p->next;
    }
    return result;
}

int countIf(
    List L,
    boolean (*predicate)(ListElement e, void *context),
    void *context) {
    int count = 0;
    Node *p = L.head;
    while (p!=NULL) {
        if (predicate(p->element, context)) count++;
        p = p->next;
    }
    return count;
}


/*helper: split list jadi dua bagian (buat merge sort) */
static Node* splitList(Node *head) {
    if (head==NULL || head->next==NULL) return NULL;
    Node *slow = head;
    Node *fast = head->next;
    while (fast!=NULL && fast->next!=NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node *mid = slow->next;
    slow->next = NULL;
    return mid;
}

/* helper: merge 2 sorted lists */
static Node* mergeSortedLists(Node *a, Node *b, int (*cmp)(ListElement, ListElement)) {
    if (a==NULL) return b;
    if (b==NULL) return a;
    Node *result = NULL;
    if (cmp(a->element, b->element)<=0) {
        result =a;
        result->next = mergeSortedLists(a->next, b, cmp);
    } else {
        result =b;
        result->next = mergeSortedLists(a, b->next, cmp);
    }
    return result;
}

/* helper: merge sort recursive */
static Node* mergeSortRec(Node *head, int (*cmp)(ListElement, ListElement)) {
    if (head==NULL || head->next==NULL) return head;
    Node *mid = splitList(head);
    Node *left = mergeSortRec(head, cmp);
    Node *right = mergeSortRec(mid, cmp);
    return mergeSortedLists(left, right, cmp);
}

void sortList(List *L, int (*cmp)(ListElement a, ListElement b)) {
    if (L->head==NULL || L->head->next==NULL) return;
    L->head =mergeSortRec(L->head, cmp);
}

List sortedCopyList(List L, int (*cmp)(ListElement a, ListElement b)) {
    List copy=copyList(L);
    sortList(&copy,cmp);
    return copy;
}

List copyList(List L) {
    List result;
    createList(&result);
    Node *p = L.head;
    while (p!=NULL) {
        insertLastList(&result, p->element);
        p=p->next;
    }
    return result;
}

void reverseList(List *L) {
    Node *prev = NULL;
    Node *curr = L->head;
    Node *next = NULL;
    while (curr!=NULL) {
        next=curr->next;
        curr->next = prev;
        prev=curr;
        curr=next;
    }
    L->head = prev;
}

List reversedCopyList(List L) {
    List copy=copyList(L);
    reverseList(&copy);
    return copy;
}


void displayList(List L, void (*print)(ListElement e)) {
    Node *p=L.head;
    while (p!=NULL) {
        print(p->element);
        p = p->next;
    }
}


List concatList(List L1, List L2) {
    List L;
    createList(&L);
    Node *p = L1.head;
    while (p!=NULL) {
        insertLastList(&L, p->element);
        p=p->next;
    }
    p = L2.head;
    while (p!=NULL) {
        insertLastList(&L, p->element);
        p=p->next;
    }
    return L;
}

void freeList(List *L) {
    Node *p = L->head;
    while (p!=NULL) {
        Node *next=p->next;
        free(p);
        p=next;
    }
    L->head = NULL;
}