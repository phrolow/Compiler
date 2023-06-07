#ifndef LANGUAGE_COMPILER_H
#define LANGUAGE_COMPILER_H

#include <elf.h>
#include <math.h>
#include <sys/mman.h>

#include "../config.h"

#include "../Tree/tree.h"
#include "../Reader/reader.h"
#include "../Onegin/onegin.h"
#include "../Debug/debug.h"
#include "../List/list.h"

#include "ir.h"
#include "x86_64_macros.h"

#define NODE_KEYW(NODE, KEYW) (NODE->val->type == KEYWORD_TYPE && NODE->val->value.keyword == KEYW)

#define KEYW(NODE) ((NODE->val->type == KEYWORD_TYPE) ? NODE->val->value.keyword : 0)
struct Compiler {
    char *out;
    char *header;
    char *memory;
    char *libs;
    char *instructions;
    char *end;
    char *main;

    struct Node *node_main;
    size_t if_counter ;
    size_t while_counter;
    struct List *global_NT;
    struct List *local_vars;
    size_t free_memory_index;
    size_t num_consts;

    u_int64_t offset_data;

    size_t count_var;
    size_t count_label;

    struct List *labels;

    cmds_t *cmds;
    
    char *ip;
};

#ifdef DOUBLES

const size_t EPSILON_LOCATION       = DATA_START + LIBS_BUFS_OFFSET;
const size_t MINUS_EPSILON_LOCATION = EPSILON_LOCATION          +     sizeof(double);
const size_t ABS_MASK_LOCATION      = MINUS_EPSILON_LOCATION    +     sizeof(double);
const size_t CONSTS_LOCATION        = ABS_MASK_LOCATION         + 2 * sizeof(double);

#else

const size_t CONSTS_LOCATION = DATA_START + LIBS_BUFS_OFFSET;

#endif

const size_t ENTRY_POINT = TEXT_START + LIBS_SIZE;
const size_t HEADER_SIZE = sizeof(Elf64_Ehdr) + 2 * sizeof(Elf64_Phdr);
const size_t MEMORY_SIZE = TEXT_START - DATA_START;

const size_t POISON  = 0xbaadf00d;

#include "func_header.h"

#endif //LANGUAGE_COMPILER_H
