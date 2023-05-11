#ifndef LANGUAGE_COMPILER_H
#define LANGUAGE_COMPILER_H

#include <math.h>
#include <sys/mman.h>

#include "../Tree/tree.h"
#include "../Reader/reader.h"
#include "../Onegin/onegin.h"
#include "../Debug/debug.h"
#include "../List/list.h"

#include "x86_64.h"

#define NODE_KEYW(NODE, KEYW) (NODE->val->type == KEYWORD_TYPE && NODE->val->value.keyword == KEYW)

#define KEYW(NODE) ((NODE->val->type == KEYWORD_TYPE) ? NODE->val->value.keyword : 0)
struct Compiler {
    char *out;
    char *memory;
    char *instructions;
    char *end;
    char *main;

    struct Node *node_main;
    size_t __IF_COUNTER__ ;
    size_t __WHILE_COUNTER__;
    struct List *GlobalNT;
    struct List *local_vars;
    size_t free_memory_index;

    // size_t size_lib;
    // size_t size_code;
    // size_t size_data;
    // size_t size_headers;

    u_int64_t offset_data;

    size_t count_var;
    size_t count_label;

    struct List *labels;
    
    char *ip;
};

const size_t BUFSIZE = 0x800;
const size_t MEMORY_SIZE = 0x800;
const u_int64_t POISON  = 0xbaadf00dbaadf00d;

int language_compile(const char *in, const char *out);

struct Compiler *newCompiler(char *out);
void CompilerDtor(struct Compiler *compiler);

int IsNum(struct Node *node);
int IsVar(struct Node *node);
int IsMathOper(struct Node *node);
int IsLogOper(struct Node *node);

void IncreaseRBX(const size_t number, struct Compiler *compiler);
void DecreaseRBX(const size_t number, struct Compiler *compiler);

int  SearchInNametable(struct Node *node, struct List *NT);
void PushInNametable(struct Node *node, struct List *NT);
int  IndexNametable(struct Node *node, struct List *NT);

void GenerateMark(struct Node *mark, struct Compiler *compiler);
void GenerateMain      (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateFuncDef   (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateDefParams (struct Node *node, struct List *NT, struct Compiler *compiler);

void InitVar           (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateVar       (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateExpr      (struct Node *node, struct List *NT, struct Compiler *compiler);
void InitGlobVar       (struct Node *node, struct Compiler *compiler);
void GenerateGlobExpr  (struct Node *node, struct Compiler *compiler);
void GenerateGlobVar   (struct Node *node, struct Compiler *compiler);

void GenerateMathOper  (struct Node *node, struct Compiler *compiler);
void GenerateNum       (struct Node *node, struct Compiler *compiler);
void InitCallParams    (struct Node *node, struct List *NT, struct Compiler *compiler, size_t *num_of_params);
void GenerateCall      (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateJump      (struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num);
void GenerateCond      (struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num);
void GenerateIf        (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateReturn    (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateWhile     (struct Node *node, struct List *NT, struct Compiler *compiler);
// void GenerateScan      (struct Node *node, struct List *NT, struct Compiler *compiler);
// void GeneratePrint     (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateAssign    (struct Node *node, struct List *NT, struct Compiler *compiler);

void GenerateStmt      (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateStmts     (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateGS        (struct Node *node, struct Compiler *compiler);
// void GenerateASM       (const char *filename, tree *tree, Compiler *compiler);

void        generateLabel(const char *format, size_t index, struct Compiler *compiler);
u_int64_t   indexLabel   (const char *format, size_t index, struct Compiler *compiler);
void        putAddress   (const char *format, size_t index, struct Compiler *compiler);
void        putAddress32 (const char *format, size_t index, struct Compiler *compiler);

// void generateBinary    (tree *tree, Compiler *compiler);
// void generateELF       (FILE *fp);

void generateBinary    (tree *tree, Compiler *compiler);
void generateELF       (FILE *fp);

#endif //LANGUAGE_COMPILER_H
