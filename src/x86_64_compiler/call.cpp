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

    size_t num_vars = gettail(NT) + 1;

    IncreaseRBX(num_vars, compiler);

    BYTE1(0xe8); putAddress(name->val->value.name, POISON, compiler); // call name->val->value.name
    BYTE1(0x50);                                                      // push rax

    DecreaseRBX(num_vars, compiler);
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

    // BYTE2(0x41, 0x5c);                              // pop r12
    // BYTE4(0x4c, 0x89, 0x63, NT->size * 8);          // mov [rbx + 8 * num_of_vars], r12      

    BYTE1(0x58);    // pop rax
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
        // else {
        //     PRINT_("Repeating main");

        //     exit(1);
        // }
    }

    struct Node *params = func->children[RIGHT];
    compiler->free_memory_index = 0;
    GenerateMark(mark, compiler);

    if (params) {
        GenerateDefParams(params, NT, compiler);
    }

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

    size_t index = SearchInNametable(node->children[LEFT], NT);

    BYTE4(0x48, 0x89, 0x43, 8 * (index - 1));   // mov [rbx + 8(index - 1)], rax    ; rax is arg
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