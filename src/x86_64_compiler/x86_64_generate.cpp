#include "compiler.h"

void CreateKeyword(token_t **token, KEYW keyword) {
    assert(token && *token);

    if(!token) {
        PRINT_("Null token pointer");

        return;
    }

    if(!(*token)) {
        PRINT_("Null token");

        return;
    }

    (*token)->type = KEYWORD_TYPE;
    (*token)->value.keyword = keyword;
}

int isNum(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return (node->val->type == NUM_TYPE);
}

int isVar(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return (node->val->type == VAR_TYPE && node->parent && KEYW(node->parent) != KEYW_DEFINE);
}

int isMathOper(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_ADD) && (KEYW(node) <= KEYW_POW));
}

int isLogOper(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_EQUAL) && (KEYW(node) <= KEYW_OR));
}

void IncreaseRBX(const size_t number, struct Compiler *compiler) {
    addInstruction(compiler->cmds, ADD_RBX, number * 8);
}

void DecreaseRBX(const size_t number, struct Compiler *compiler) {
    addInstruction(compiler->cmds, SUB_RBX, number * 8);
}

void generateGS(struct Node *node, struct Compiler *compiler) {
    if (!NODE_KEYW(node, KEYW_STMT)) {
        PRINT_("It's not global statement");

        return;
    }

    compiler->cmds = (cmds_t*) calloc(1, sizeof(cmds_t));
    cmdArrayCtor(compiler->cmds, NUM_CMDS, compiler->ip);

    compiler->instructions = compiler->ip;

    BYTE3(0x48, 0xc7, 0xc3); INT(X64_VA_START + CONSTS_LOCATION + 8 * compiler->num_consts);   // mov rbx, CONSTS_LOCATION
    BYTE1(0xe8); putAddress("main", POISON, compiler);              // call main
    BYTE7(0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00);                // mov rax, 0x3c
    BYTE3(0x48, 0x31, 0xff);                                        // xor rdi, rdi
    BYTE2(0x0f, 0x05);                                              // syscall

    compiler->num_consts = 0;                                       // for 2nd initizalization
    compiler->cmds->ip = compiler->ip;

    while (node->children[RIGHT])
        node = node->children[RIGHT];

    struct Node *bottom = node;

    while (node) {
        if (KEYW(node->children[LEFT]) == KEYW_ASSIGN)
        {
            generateAssign(node, NULL, compiler);
        }

        node = node->parent;
    }

    struct List *NT = newList();

    node = bottom;

    while (node) {
        if (NODE_KEYW(node->children[LEFT], KEYW_DEFINE)) {
            generateFuncDef(node->children[LEFT], NT, compiler);
        } else if (NODE_KEYW(node->children[LEFT], KEYW_ASSIGN)) {
            node = node->parent;
            continue;
        } else {
            PRINT_("Invalid statement");

            ListDtor(NT);

            return;
        }
        node = node->parent;
    }

    generateMain(compiler->node_main, NT, compiler);
    
    printArray(compiler->cmds, compiler->ip);

    ListDtor(NT);
}

void initGlobVar(struct Node *node, struct Compiler *compiler) {
    if (searchInNametable(node->children[LEFT], compiler->global_NT))
        PRINT_("Repeating definition");

    pushInNametable(node->children[LEFT], compiler->global_NT);

    size_t index = indexNametable(node->children[LEFT], compiler->global_NT);

    assert(index >= 1);

    generateGlobExpr(node->children[RIGHT], compiler);

    #ifdef DOUBLES

    XMM_POP(1);
    addInstruction(compiler->cmds, MOVSD_MEM_XMM1, (index - 1) * sizeof(double));

    #else

    addInstruction(compiler->cmds, POP_R12, POISON);
    addInstruction(compiler->cmds, MOV_MEM_R12, (index - 1) * sizeof(int64_t));

    #endif
}

void initVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if(searchInNametable(node->children[LEFT], compiler->global_NT)) 
        PRINT_("Repeating variable");

    if (searchInNametable(node->children[LEFT], NT))
    {
        generateExpr(node->children[RIGHT], NT, compiler);

        size_t index = indexNametable(node->children[LEFT], NT);

        assert(index >= 1);

        #ifdef DOUBLES

        XMM_POP(1);
        addInstruction(compiler->cmds, MOVSD_MEM_XMM1, (index - 1) * 8);

        #else

        addInstruction(compiler->cmds, POP_R12, POISON);
        addInstruction(compiler->cmds, MOV_MEM_R12, (index - 1) * 8);

        #endif

        return;
    }

    pushInNametable(node->children[LEFT], NT);

    size_t index = indexNametable(node->children[LEFT], NT);

    assert(index >= 1);

    generateExpr(node->children[RIGHT], NT, compiler);

    #ifdef DOUBLES

    XMM_POP(1);
    addInstruction(compiler->cmds, MOVSD_MEM_XMM1, (index - 1) * 8);

    #else

    addInstruction(compiler->cmds, POP_R12, POISON);
    addInstruction(compiler->cmds, MOV_MEM_R12, (index - 1) * 8);

    #endif
}