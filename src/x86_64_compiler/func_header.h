#ifndef FUNC_HEADER_H_INCLUDED
#define FUNC_HEADER_H_INCLUDED

int languageCompile(const char *in, const char *out);

struct Compiler *newCompiler(char *out);
void reinitCompiler(struct Compiler *compiler, char *out);
void compilerDtor(struct Compiler *compiler);

int isNum(struct Node *node);
int isVar(struct Node *node);
int isMathOper(struct Node *node);
int isLogOper(struct Node *node);

void IncreaseRBX(const size_t number, struct Compiler *compiler);
void DecreaseRBX(const size_t number, struct Compiler *compiler);

int  searchInNametable(struct Node *node, struct List *NT);
void pushInNametable(struct Node *node, struct List *NT);
int  indexNametable(struct Node *node, struct List *NT);

void generateMark(struct Node *mark, struct Compiler *compiler);
void generateMain      (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateFuncDef   (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateDefParams (struct Node *node, struct List *NT, struct Compiler *compiler);

void initVar           (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateVar       (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateExpr      (struct Node *node, struct List *NT, struct Compiler *compiler);
void initGlobVar       (struct Node *node, struct Compiler *compiler);
void generateGlobExpr  (struct Node *node, struct Compiler *compiler);
void generateGlobVar   (struct Node *node, struct Compiler *compiler);

void generateMathOper  (struct Node *node, struct Compiler *compiler);
void generateNum       (struct Node *node, struct Compiler *compiler);
void initCallParams    (struct Node *node, struct List *NT, struct Compiler *compiler, size_t *num_of_params);
void generateCall      (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateJump      (struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num);
void generateCond      (struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num);
void generateIf        (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateReturn    (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateWhile     (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateScan      (struct Node *node, struct List *NT, struct Compiler *compiler);
void generatePrint     (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateSqrt      (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateAssign    (struct Node *node, struct List *NT, struct Compiler *compiler);

void generateStmt      (struct Node *node, struct List *NT, struct Compiler *compiler);
void GenerateStmts     (struct Node *node, struct List *NT, struct Compiler *compiler);
void generateGS        (struct Node *node, struct Compiler *compiler);

void        generateLabel           (const char *format, size_t index, struct Compiler *compiler);  // out of code
void        generateLabelFromCmds   (const char *format, size_t index, struct Compiler *compiler);  // in the code (from IR and for IR)
u_int64_t   indexLabel              (const char *format, size_t index, struct Compiler *compiler);
void        putAddress              (const char *format, size_t index, struct Compiler *compiler);
void        putAddress64            (const char *format, size_t index, struct Compiler *compiler);
uint32_t    relAddress              (const char *format, size_t index, struct Compiler *compiler);

void generateBinary    (tree *tree, Compiler *compiler);
void generateElfHead   (Compiler *compiler);
void generateLibs      (struct Compiler *compiler);

void generateMemory(struct Compiler *compiler);

#endif