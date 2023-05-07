#include "compiler.h"

struct Compiler *newCompiler(FILE *out) {
    struct Compiler *compiler = (Compiler*) malloc(sizeof(Compiler));

    compiler->node_main = NULL;
    compiler->out = out;
    compiler->__IF_COUNTER__ = 0;
    compiler->__WHILE_COUNTER__ = 0;
    compiler->GlobalNT = NULL;

    return compiler;
}

void CompilerDtor(struct Compiler *compiler) {
    compiler->node_main = NULL;
    compiler->out = NULL;
    compiler->__IF_COUNTER__ = 0xFFFFFFFF;
    compiler->__WHILE_COUNTER__ = 0xFFFFFFFF;
    ListDtor(compiler->GlobalNT);
    compiler->GlobalNT = NULL;
    compiler->free_memory_index = 0xFFFFFFFF;

    free(compiler);
}