#include "compiler.h"

struct Compiler *newCompiler(char *out) {
    struct Compiler *compiler = (Compiler*) malloc(sizeof(Compiler));

    compiler->memory = NULL;
    compiler->instructions = NULL;
    compiler->end = NULL;

    compiler->node_main = NULL;
    compiler->out = out;
    compiler->__IF_COUNTER__ = 0;
    compiler->__WHILE_COUNTER__ = 0;
    compiler->GlobalNT = NULL;
    compiler->local_vars = NULL;

    compiler->free_memory_index = 0;
    compiler->ip = out;

    // compiler->size_lib = 0;
    // compiler->size_code = 0;
    // compiler->size_data = 0;
    // compiler->size_headers = 0;

    compiler->offset_data = 0;

    compiler->count_var = 0;
    compiler->count_label = 0;

    compiler->labels = newList();

    return compiler;
}

void CompilerDtor(struct Compiler *compiler) {
    compiler->out = NULL;

    compiler->memory = NULL;
    compiler->instructions = NULL;
    compiler->end = NULL;
    
    compiler->node_main = NULL;
    compiler->__IF_COUNTER__    = POISON;
    compiler->__WHILE_COUNTER__ = POISON;

    ListDtor(compiler->GlobalNT);
    compiler->GlobalNT = NULL;

    //ListDtor(compiler->local_vars);
    compiler->local_vars = NULL;

    compiler->free_memory_index = POISON;
    compiler->ip = NULL;

    // compiler->size_lib = POISON;
    // compiler->size_code = POISON;
    // compiler->size_data = POISON;
    // compiler->size_headers = POISON;

    compiler->offset_data = POISON;

    compiler->count_var = POISON;
    compiler->count_label = POISON;

    ListDtor(compiler->labels);
    compiler->labels = NULL;

    free(compiler);
}