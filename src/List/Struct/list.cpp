#include "../list.h"

list *newList_(const char* func, const char* file, size_t line) {
    int i = 0;

    listinfo new_info = { NULL, NULL, 0 };
    list *new_list = (list*) malloc(sizeof(list));

    new_list->size = DEFAULTSIZE;

    new_list->data = (elem_t*)calloc(new_list->size, sizeof(elem_t));
    new_list->next = (int*)calloc(new_list->size, sizeof(int));
    new_list->prev = (int*)calloc(new_list->size, sizeof(int));

    new_info.func = func;
    new_info.file = file;
    new_info.line = line;

    new_list->info = new_info;

    ListInit(new_list);

    return new_list;
}

void ListDtor(list *list) {
    for(int i = 0; i < list->size; i++)
        strcpy(list->data[i].name, "\0");

    list->size = 0;
    list->Head = 0;
    list->Tail = 0;

    free(list->data);
    list->data = NULL;

    free(list->next);
    list->next = NULL;

    free(list->prev);
    list->prev = NULL;

    free(list);

    return;
}