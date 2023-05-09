#include "compiler.h"

void GenerateCall(struct Node *node, struct List *NT, struct Compiler *compiler) {
    struct Node *params = node->children[RIGHT];

    if(!params)
        PRINT_("Null pointer to parameters node");

    size_t num_of_params = 0;

    InitCallParams(params, NT, compiler, &num_of_params);

    if(num_of_params != 1)
        PRINT_("Not one parameter");

    struct Node *name = node->children[LEFT];

    // вытаскиваем индекс метки хуетки с листаы

    // ARG_INSTR("call ну, тип rip - адрес по имени бля", name->val->value.name);
    // INSTR("push rax");
}

void InitCallParams(struct Node *node, struct List *NT, struct Compiler *compiler, size_t *num_of_params) {
    if(!num_of_params)
        PRINT_("Null pointer to number of parameters");

    if (!NODE_KEYW(node, KEYW_PARAM))
    {
        PRINT_("No parameters for function");
    }

    if(*num_of_params)
        PRINT_("Started not from beginning of parameters");

    (*num_of_params)++;

    GenerateExpr(node->children[LEFT], NT, compiler);
    
    // INSTR("pop r12");

    // ARG_INSTR("mov [rbx + 8 * %lu], r12", *num_of_params);
}

void GenerateFuncDef(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    struct Node *func   = node->children[LEFT];
    struct Node *mark   = func->children[LEFT];

    if (KEYW(mark) == KEYW_MAIN) {
        if (!compiler->node_main) {
            compiler->node_main = node;
            return;
        }
        else {
            PRINT_("Repeating main");

            exit(1);
        }
    }

    struct Node *params = func->children[RIGHT];
    compiler->free_memory_index = 0;
    GenerateMark(mark, compiler);

    if (params) {
        GenerateDefParams(params, NT, compiler);
    }

    IncreaseRBX(compiler->free_memory_index, compiler);

    struct Node *stmts  = node->children[RIGHT];

    GenerateStmts(stmts, NT, compiler);

    ListInit(NT);
}

void GenerateDefParams(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (node->children[RIGHT]) {
        return;
    }
    else {
        compiler->free_memory_index = 1;
        if (SearchInNametable(node->children[LEFT], compiler->GlobalNT)) {
            PRINT("Variable is already defined");
        }
        else {
            PushInNametable(node->children[LEFT], NT);
        }
    }
}

void GenerateMain(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (!compiler->node_main) {
        PRINT_("No main function in your source");

        exit(1);
    }

    struct Node *func = node->children[LEFT];
    struct Node *main = func->children[LEFT];

    GenerateMark(main, compiler);
    IncreaseRBX(gettail(compiler->GlobalNT) + 1, compiler);

    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    struct Node *stmts  = node->children[RIGHT];

    GenerateStmts(stmts, NT, compiler);
}