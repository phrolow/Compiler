#include "compiler.h"

void generateCall(struct Node *node, struct List *NT, struct Compiler *compiler) {
    struct Node *params = node->children[RIGHT];

    if(!params)
        PRINT_("Null pointer to parameters node");

    size_t num_of_params = 0;

    initCallParams(params, NT, compiler, &num_of_params);

    if(num_of_params != 1)
        PRINT_("Not one parameter");

    struct Node *name = node->children[LEFT];

    size_t num_vars = gettail(NT) + 1;

    IncreaseRBX(num_vars, compiler);

    addInstruction(compiler->cmds, CALL, relAddress(name->val->value.name, POISON, compiler));

    #ifdef DOUBLES

    XMM_PUSH(0);

    #else

    addInstruction(compiler->cmds, PUSH_RAX, POISON);

    #endif

    DecreaseRBX(num_vars, compiler);
}

void initCallParams(struct Node *node, struct List *NT, struct Compiler *compiler, size_t *num_of_params) {
    if(!num_of_params)
        PRINT_("Null pointer to number of parameters");

    if (!NODE_KEYW(node, KEYW_PARAM))
    {
        PRINT_("No parameters for function");
    }

    if(*num_of_params)
        PRINT_("Started not from beginning of parameters");

    (*num_of_params)++;

    generateExpr(node->children[LEFT], NT, compiler);

    #ifdef DOUBLES

    XMM_POP(0);

    #else

    addInstruction(compiler->cmds, POP_RAX, POISON);

    #endif
}

void generateFuncDef(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    struct Node *func   = node->children[LEFT];
    struct Node *mark   = func->children[LEFT];

    if (KEYW(mark) == KEYW_MAIN) {
        if (!compiler->node_main) {
            compiler->node_main = node;
            return;
        }
    }

    struct Node *params = func->children[RIGHT];
    compiler->free_memory_index = 0;
    generateMark(mark, compiler);

    if (params) {
        generateDefParams(params, NT, compiler);
    }

    struct Node *stmts  = node->children[RIGHT];

    GenerateStmts(stmts, NT, compiler);

    ListInit(NT);
}

void generateDefParams(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (node->children[RIGHT]) {
        return;
    }
    else {
        compiler->free_memory_index = 1;
        if (searchInNametable(node->children[LEFT], compiler->global_NT)) {
            PRINT("Variable is already defined");
        }
        else {
            pushInNametable(node->children[LEFT], NT);
        }
    }

    size_t index = searchInNametable(node->children[LEFT], NT);

    #ifdef DOUBLES

    addInstruction(compiler->cmds, MOVSD_MEM_XMM0, sizeof(double) * (index - 1));

    #else

    addInstruction(compiler->cmds, MOV_MEM_RAX, sizeof(int64_t) * (index - 1));

    #endif
}

void generateMain(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (!compiler->node_main) {
        PRINT_("No main function in your source");

        exit(1);
    }

    struct Node *func = node->children[LEFT];
    struct Node *main = func->children[LEFT];

    generateMark(main, compiler);
    IncreaseRBX(gettail(compiler->global_NT) + 1, compiler);

    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    struct Node *stmts  = node->children[RIGHT];

    GenerateStmts(stmts, NT, compiler);
}