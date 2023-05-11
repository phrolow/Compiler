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

// void GenerateScan(struct Node *node, struct List *NT, struct Compiler *compiler) {
//     TREE_ERROR node_err = NodeVerify(node);

//     if(node_err) {
//         PRINT_("Something wrong with node! Code of error:");

//         PRINT_D(node_err);
//     }

//     if(!NT) {
//         PRINT_("Null name table!");
//     }

//     if(!compiler) {
//         PRINT_("Null struct compiler!");
//     }

//     if (!node->children[LEFT]) {
//         PRINT_("No arg for scan");
//     }

//     PushInNametable(node->children[LEFT], NT);

//     size_t index = IndexNametable(node->children[LEFT], NT);

//     INSTR("call cin");
//     INSTR("push rax");
// }

// void GeneratePrint(struct Node *node, struct List *NT, struct Compiler *compiler) {
//     TREE_ERROR node_err = NodeVerify(node);

//     if(node_err) {
//         PRINT_("Something wrong with node! Code of error:");

//         PRINT_D(node_err);
//     }

//     if(!NT) {
//         PRINT_("Null name table!");
//     }

//     if(!compiler) {
//         PRINT_("Null struct compiler!");
//     }

//     if (!node->children[LEFT]) {
//         PRINT_("No arg for print");
//     }

//     GenerateExpr(node->children[LEFT], NT, compiler);

//     INSTR("call cout");
//     INSTR("add rsp, 8");
// }

int IsNum(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return (node->val->type == NUM_TYPE);
}

int IsVar(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return (node->val->type == VAR_TYPE && node->parent && KEYW(node->parent) != KEYW_DEFINE);
}

int IsMathOper(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_ADD) && (KEYW(node) <= KEYW_POW));
}

int IsLogOper(struct Node *node) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_EQUAL) && (KEYW(node) <= KEYW_OR));
}

void IncreaseRBX(const size_t number, struct Compiler *compiler) {
    BYTE4(0x48, 0x83, 0xc3, number * 8); // add rbx, number * 8
}

void DecreaseRBX(const size_t number, struct Compiler *compiler) {
    BYTE4(0x48, 0x83, 0xeb, number * 8); // sub rbx, number * 8
}

// void generateElfHead(Compiler *compiler) {

// }

// void generateElfTail(Compiler *compiler) {
    
// }

void generateBinary(tree *tree, Compiler *compiler) {
    // generateElfHead(compiler);

    compiler->GlobalNT = newList();

    GenerateGS(tree->root, compiler);

    // generateElfTail(compiler);
}

void GenerateGS(struct Node *node, struct Compiler *compiler) {
    if (!NODE_KEYW(node, KEYW_STMT)) {
        PRINT_("It's not global statement");

        return;
    }

    generateMemory(compiler);

    compiler->instructions = compiler->ip;

    BYTE1(0xe8); putAddress("main", POISON, compiler);  // call main
    BYTE7(0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00);    // mov rax, 0x3c
    BYTE3(0x48, 0x31, 0xff);                            // xor rdi, rdi
    BYTE2(0x0f, 0x05);                                  // syscall

    while (node->children[RIGHT])
        node = node->children[RIGHT];

    struct Node *bottom = node;

    while (node) {
        if (KEYW(node->children[LEFT]) == KEYW_ASSIGN)
        {
            GenerateAssign(node, NULL, compiler);
        }

        node = node->parent;
    }

    struct List *NT = newList();

    node = bottom;

    while (node) {
        if (NODE_KEYW(node->children[LEFT], KEYW_DEFINE)) {
            GenerateFuncDef(node->children[LEFT], NT, compiler);
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

    GenerateMain(compiler->node_main, NT, compiler);
    
    ListDtor(NT);
}

void generateMemory(struct Compiler *compiler) {
    compiler->memory = compiler->ip;

    BYTE3(0x48, 0x89, 0xc3);                                                                        // mov rbx, rip
    BYTE1(0xcc);                                                                                    // int 03
    BYTE1(0xe9); *((u_int32_t *) (compiler->ip)) = MEMORY_SIZE - 9; compiler->ip += 8;              // skip data space (with jmp) 
                                                                                                    // 9 is num of already printed instructions
    compiler->ip = compiler->memory + MEMORY_SIZE;                                  
}

void InitGlobVar(struct Node *node, struct Compiler *compiler) {
    if (SearchInNametable(node->children[LEFT], compiler->GlobalNT))
        PRINT_("Repeating definition");

    PushInNametable(node->children[LEFT], compiler->GlobalNT);

    size_t index = IndexNametable(node->children[LEFT], compiler->GlobalNT);

    assert(index >= 1);

    GenerateGlobExpr(node->children[RIGHT], compiler);

    BYTE2(0x41, 0x5c);                          // pop r12
    BYTE4(0x4c, 0x89, 0x63, (index - 1) * 8);   // mov [rbx + (index - 1) * 8], r12 
                                                // risk if there're 17 or more global vars
}

void InitVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if(SearchInNametable(node->children[LEFT], compiler->GlobalNT)) 
        PRINT_("Repeating variable");

    if (SearchInNametable(node->children[LEFT], NT))
    {
        GenerateExpr(node->children[RIGHT], NT, compiler);

        size_t index = IndexNametable(node->children[LEFT], NT);

        assert(index >= 1);

        BYTE2(0x41, 0x5c);                          // pop r12
        BYTE4(0x4c, 0x89, 0x63, (index - 1) * 8);   // mov [rbx + (index - 1) * 8], r12 
                                                    // risk if there're 17 or more global vars

        return;
    }

    PushInNametable(node->children[LEFT], NT);

    size_t index = IndexNametable(node->children[LEFT], NT);

    assert(index >= 1);

    GenerateExpr(node->children[RIGHT], NT, compiler);

    BYTE2(0x41, 0x5c);                          // pop r12
    BYTE4(0x4c, 0x89, 0x63, (index - 1) * 8);   // mov [rbx + (index - 1) * 8], r12 
                                                // risk if there're 17 or more global vars
}