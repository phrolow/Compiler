#include "../list.h"

void ListHeadInsert(list *list, elem_t a) {
    int head = 0,
        free = 0;

    head = gethead(list);
    free = putfree(list);

    list->prev[head] = free;
    list->next[free] = head;
    list->prev[free] = 0;
    list->data[free] = a;

    list->Head = free;

    list->happy = 0;
}

void ListTailInsert(list *list, elem_t a) {
    int tail = 0,
        free = 0;

    tail = gettail(list);
    free = putfree(list);

    list->next[tail] = free;
    list->prev[free] = tail;
    list->next[free] = {'\0'};

    strcpy(list->data[free].name, a.name);

    list->Tail = free;
}

void ListInsertAfter(list *list, elem_t a, int logindex) {
    ListPhInsertAfter(list, a, physindex(list, logindex));
}

void ListInsertBefore(list *list, elem_t a, int logindex) {
    ListPhInsertBefore(list, a, physindex(list, logindex));
}

void ListPhInsertAfter(list *list, elem_t a, int physindex) {
    int free = 0,
        next = 0;

    free = putfree(list);
    next = getnext(list, physindex);

    list->next[physindex] = free;
    list->prev[free] = physindex;

    strcpy(list->data[free].name, a.name);

    list->next[free] = next;
    list->prev[next] = free;

    if(gettail(list) != free)
        list->happy = 0;
}

void ListPhInsertBefore(list *list, elem_t a, int physindex) {
    int free = 0,
        prev = 0;

    free = putfree(list);
    prev = getprev(list, physindex);

    list->next[prev] = free;
    list->prev[free] = prev;

    strcpy(list->data[free].name, a.name);

    list->next[free] = physindex;
    list->prev[physindex] = free;

    list->happy = 0;
}