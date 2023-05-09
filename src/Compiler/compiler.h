#ifndef LANGUAGE_COMPILER_H
#define LANGUAGE_COMPILER_H

#include <math.h>

#include "../Tree/tree.h"
#include "../Reader/reader.h"
#include "../Onegin/onegin.h"
#include "../Debug/debug.h"
#include "../List/list.h"

struct Compiler {
    FILE *out;
    struct Node *node_main;
    size_t __IF_COUNTER__ ;
    size_t __WHILE_COUNTER__;
    struct List *GlobalNT;
    size_t free_memory_index;
};

int language_compile(const char *in, const char *out);

struct Compiler *newCompiler(FILE *out);
void CompilerDtor(struct Compiler *compiler);

int IsNum(struct Node *node);
int IsVar(struct Node *node);
int IsMathOper(struct Node *node);
int IsLogOper(struct Node *node);

void IncreaseRBX(const size_t number, struct Compiler *compiler);
void DecreaseRBX(const size_t number, struct Compiler *compiler);

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
void GenerateScan      (struct Node *node, struct List *NT, struct Compiler *compiler);
void GeneratePrint     (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateSqrt      (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateAssign    (struct Node *node, struct List *NT, struct Compiler *compiler);

void GenerateStmt      (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateStmts     (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateGS        (struct Node *node, struct Compiler *compiler);
void GenerateASM       (const char *filename, tree *tree, Compiler *compiler);

#endif //LANGUAGE_COMPILER_H
