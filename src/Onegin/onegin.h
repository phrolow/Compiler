#ifndef ONEGIN
#define ONEGIN

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
struct text {
    char *content;
    char **ptrs;
    size_t  nChar,
            nLine,
            maxLine;
};

struct text maketext(char *content, char** ptrs, size_t nChar, size_t nLine, size_t maxLine);

struct text textFromFile(const char *path);

void sortText(struct text sortableText, int(*comp) (const char *, const char*));

void appendText(struct text appendableText, const char* path);

void appendContent(const char* content, const char* path);

int compStart(const char* ptr1, const char* ptr2);

int compEnd(const char* ptr1, const char* ptr2);

size_t ostrlen(const char* ptr);

int checkfile(const char* path);

int streq(const char *txt, const char *str);

void textDtor(struct text *txt);
#endif
