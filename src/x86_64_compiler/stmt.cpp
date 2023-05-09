#include "compiler.h"

void GenerateAssign(struct Node *node, struct List *NT, struct Compiler *compiler) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!compiler)
        PRINT_("No struct compiler");

    if (!NT) {
        InitGlobVar(node, compiler);
        
    } else {
        InitVar(node, NT, compiler);
    }
}

void GenerateIf(struct Node *node, struct List *NT, struct Compiler *compiler) {
    struct Node *else_node   = node->children[RIGHT];

    struct Node *condition  = node->children[LEFT];

    struct Node *if_stmts = NULL,
                *else_stmts = NULL;

    if(!NODE_KEYW(else_node, KEYW_ELSE)) {
        if_stmts = else_node;
    }
    else {
        if_stmts = else_node->children[LEFT];
        else_stmts = else_node->children[RIGHT];
    }

    size_t counter = compiler->__IF_COUNTER__++;

    if (else_stmts)
    {
        GenerateCond(condition, NT, compiler, "ELSE", counter);
        GenerateStmts(if_stmts, NT, compiler);

        // ARG_INSTR("jmp .END_IF_%lu", counter);
        // fprintf(compiler->out, ".ELSE_%lu:\n", counter);

        GenerateStmts(else_stmts, NT, compiler);
    }
    else
    {
        GenerateCond(condition, NT, compiler, "END_IF", counter);
        GenerateStmts(if_stmts, NT, compiler);
    }

    // fprintf(compiler->out, ".END_IF_%lu:\n", counter);
}

void GenerateCond(struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num) {
    GenerateExpr(node->children[LEFT],  NT, compiler);
    GenerateExpr(node->children[RIGHT], NT, compiler);
    GenerateJump(node, NT, compiler, mark, num);
}

void GenerateWhile(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    struct Node *condition = node->children[LEFT];
    struct Node *while_stmts = node->children[RIGHT];

    size_t counter = compiler->__WHILE_COUNTER__++;

    // fprintf(compiler->out, ".WHILE_%d:\n", counter);

    generateLabel("WHILE_%lu", counter, compiler);

    GenerateCond(condition, NT, compiler, "END_WHILE", counter);
    GenerateStmts(while_stmts, NT, compiler);

    // fprintf(compiler->out, ".JMP WHILE_%d:\n", counter);
    // fprintf(compiler->out, ".END_WHILE_%d:\n", counter);

    size_t index = indexLabel("WHILE_%lu", counter, compiler);

    // jmp rip - index

    generateLabel("END_WHILE_%lu", counter, compiler);
}

void GenerateReturn(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    if (!NODE_KEYW(node, KEYW_RETURN)) {
        PRINT_("No return statement");
    }

    if(node->children[LEFT]) {
        GenerateExpr(node->children[LEFT], NT, compiler);

        // INSTR("pop rax");
    }

    DecreaseRBX(compiler->free_memory_index, compiler);

    // INSTR("ret");
}