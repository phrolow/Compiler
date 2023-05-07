#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#define DEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../TextToTokens/analyzer.h"

#define _LOCATION_ __PRETTY_FUNCTION__, __FILE__, __LINE__
#define Head next[0]
#define Tail prev[0]

#define CHECK(cond, err) (cond) ? 0 : (err)

#define newList() newList_(_LOCATION_)

struct name_t
{
    size_t index;
    char name[WORD_MAX_LEN + 1];
};

typedef name_t elem_t;

typedef int err_t;

typedef struct ListInfo {
    const char  *func,
                *file;
    size_t line;
} listinfo;
typedef struct List {
    elem_t  *data;
    int  *next,
            *prev;
    size_t size;
    int happy,
        free;
    listinfo info;
} list;

const size_t DEFAULTSIZE = 0x40;

void add(list *q, elem_t a);

elem_t get(list *q);

void resize(list *list, size_t newSize);

void del(list *q, int num);

void insert(list *q, elem_t a, int num);

list *newList_(const char* func, const char* file, size_t line);

void ListDtor(list *list);

void ListDump_(struct List *list, const char *func, const char *file, size_t line);

void ListGraphDump(list *list);

err_t ListCheck_(struct List *list, const char* func, const char* file, size_t line);

int physindex(list *list, int logindex);

int getprev(list *list, int physindex);

int getnext(list *list, int physindex);

int isfree(list *list, int physindex);

int getfree(list *list);

int putfree(list *list);

int gethead(list *list);

int gettail(list *list);

elem_t *ListFirst(list *list);

elem_t *ListLast(list *list);

elem_t *ListNext(list *list);

elem_t *ListPrev(list *list);

void ListHeadInsert(list *list, elem_t a);

void ListTailInsert(list *list, elem_t a);

void ListInsertAfter(list *list, elem_t a, int logindex);

void ListInsertBefore(list *list, elem_t a, int logindex);

void ListPhInsertAfter(list *list, elem_t a, int physindex);

void ListPhInsertBefore(list *list, elem_t a, int physindex);

void ListDelete(list *list, int logindex);

void ListPhDelete(list *list, int physindex);

int ListIndexFirst(list *list, const char *str);

int ListIndexFirst(list *list, elem_t a);

int ListPhIndexFirst(list *list, elem_t a);

void ListInit(list *list);

#endif