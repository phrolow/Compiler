#include "../list.h"

elem_t *ListFirst(list *list) {
    return list->data + gethead(list);
}

elem_t *ListLast(list *list) {
    return list->data + gettail(list);
}

elem_t *ListNext(list *list, int physindex) {
    return list->data + getnext(list, physindex);
}

elem_t *ListPrev(list *list, int physindex) {
    return list->data + getprev(list, physindex);
}

