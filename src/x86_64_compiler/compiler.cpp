#include "compiler.h"

struct Compiler *newCompiler(char *out) {
    struct Compiler *compiler = (Compiler*) malloc(sizeof(Compiler));

    compiler->header = NULL;
    compiler->memory = NULL;
    compiler->instructions = NULL;
    compiler->end = NULL;

    compiler->node_main = NULL;
    compiler->out = out;
    compiler->if_counter = 0;
    compiler->while_counter = 0;
    compiler->global_NT = NULL;
    compiler->local_vars = NULL;

    compiler->free_memory_index = 0;
    compiler->num_consts = 0;

    compiler->ip = out;

    compiler->offset_data = 0;

    compiler->count_var = 0;
    compiler->count_label = 0;

    compiler->labels = newList();

    return compiler;
}

void reinitCompiler(Compiler *compiler, char *out) {
    compiler->out = out;
    
    compiler->ip = compiler->instructions;

    cmdArrayDtor(compiler->cmds);
    ListDtor(compiler->global_NT);

    compiler->memory = NULL;
    compiler->libs = NULL;
    compiler->instructions = NULL;
    compiler->end = NULL;

    compiler->node_main = NULL;
    compiler->if_counter = 0;
    compiler->while_counter = 0;
    compiler->global_NT = NULL;
    compiler->local_vars = NULL;

    compiler->free_memory_index = 0;

    compiler->offset_data = 0;

    compiler->count_var = 0;
    compiler->count_label = 0;
}

void compilerDtor(struct Compiler *compiler) {
    compiler->out = NULL;

    compiler->memory = NULL;
    compiler->libs = NULL;
    compiler->instructions = NULL;
    compiler->end = NULL;
    
    compiler->node_main = NULL;
    compiler->if_counter    = POISON;
    compiler->while_counter = POISON;

    ListDtor(compiler->global_NT);
    compiler->global_NT = NULL;

    compiler->local_vars = NULL;

    compiler->free_memory_index = POISON;
    compiler->num_consts = POISON;
    
    compiler->ip = NULL;

    compiler->offset_data = POISON;

    compiler->count_var = POISON;
    compiler->count_label = POISON;

    ListDtor(compiler->labels);
    compiler->labels = NULL;

    free(compiler);
}