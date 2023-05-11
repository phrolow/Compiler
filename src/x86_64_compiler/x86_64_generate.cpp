#include "compiler.h"

// void CreateKeyword(token_t **token, KEYW keyword) {
//     assert(token && *token);

//     if(!token) {
//         PRINT_("Null token pointer");

//         return;
//     }

//     if(!(*token)) {
//         PRINT_("Null token");

//         return;
//     }

//     (*token)->type = KEYWORD_TYPE;
//     (*token)->value.keyword = keyword;
// }

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

// void GenerateAssign(struct Node *node, struct List *NT, struct Compiler *compiler) {
//     TREE_ERROR node_err = NodeVerify(node);

//     if(node_err) {
//         PRINT_("Something wrong with node! Code of error:");

//         PRINT_D(node_err);
//     }

//     if(!compiler)
//         PRINT_("No struct compiler");

//     if (!NT) {
//         InitGlobVar(node, compiler);
        
//     } else {
//         InitVar(node, NT, compiler);
//     }
// }

// int IsNum(struct Node *node) {
//     TREE_ERROR node_err = NodeVerify(node);

//     if(node_err) {
//         PRINT_("Something wrong with node! Code of error:");

//         PRINT_D(node_err);
//     }

//     return (node->val->type == NUM_TYPE);
// }

// int IsVar(struct Node *node) {
//     TREE_ERROR node_err = NodeVerify(node);

//     if(node_err) {
//         PRINT_("Something wrong with node! Code of error:");

//         PRINT_D(node_err);
//     }

//     return (node->val->type == VAR_TYPE && node->parent && KEYW(node->parent) != KEYW_DEFINE);
// }

// int IsMathOper(struct Node *node) {
//     TREE_ERROR node_err = NodeVerify(node);

//     if(node_err) {
//         PRINT_("Something wrong with node! Code of error:");

//         PRINT_D(node_err);
//     }

//     return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_ADD) && (KEYW(node) <= KEYW_POW));
// }

// int IsLogOper(struct Node *node) {
//     TREE_ERROR node_err = NodeVerify(node);

//     if(node_err) {
//         PRINT_("Something wrong with node! Code of error:");

//         PRINT_D(node_err);
//     }

//     return ((KEYW(node) != 0) && (KEYW(node) >= KEYW_EQUAL) && (KEYW(node) <= KEYW_OR));
// }

void IncreaseRBX(const size_t number, struct Compiler *compiler) {
    BYTE4(0x48, 0x83, 0xc3, number * 8); // add rbx, number * 8
}

void DecreaseRBX(const size_t number, struct Compiler *compiler) {
    BYTE4(0x48, 0x83, 0xeb, number * 8); // sub rbx, number * 8
}

// void GenerateGlobExpr(struct Node *node, struct Compiler *compiler) {
//     if (KEYW(node) == KEYW_CALL) {
//         PRINT_("Useless function in global space");
//     }
//     if (node->children[LEFT]) {
//         GenerateGlobExpr(node->children[LEFT], compiler);
//     }
//     if (node->children[RIGHT]) {
//         GenerateGlobExpr(node->children[RIGHT], compiler);
//     }
    
//     if (IsMathOper(node)) {
//         GenerateMathOper(node, compiler);

//         return;
//     }

//     if (IsNum(node)) {
//         GenerateNum(node, compiler);

//         return;
//     }

//     if (IsVar(node)) {
//         GenerateGlobVar(node, compiler);

//         return;
//     }

//     PRINT_("Undefined operation");
// }

// void InitGlobVar(struct Node *node, struct Compiler *compiler) {
//     if (SearchInNametable(node->children[LEFT], compiler->GlobalNT))
//         PRINT_("Repeating definition");

//     PushInNametable(node->children[LEFT], compiler->GlobalNT);

//     size_t index = IndexNametable(node->children[LEFT], compiler->GlobalNT);

//     assert(index >= 1);

//     GenerateGlobExpr(node->children[RIGHT], compiler);

//     //fprintf(compiler->out, "pop [rbx+%lu]\n", index - 1);

//     INSTR("pop r12");

//     ARG_INSTR("mov [rbx + %lu * 8], r12", index - 1);
// }

// void GenerateGS(struct Node *node, struct Compiler *compiler) {
//     if (!NODE_KEYW(node, KEYW_STMT)) {
//         PRINT_("It's not global statement");

//         return;
//     }

//     fprintf(compiler->out, "BITS 64\n\n");

//     fprintf(compiler->out, "global _start\n\n");

//     fprintf(compiler->out, "section .bss\n");
//     fprintf(compiler->out, "\tmemory resq 0x100\n\n");

//     fprintf(compiler->out, "section .text\n");
//     fprintf(compiler->out, "_start:\n");
//     INSTR("lea rbx, memory");

//     INSTR("call .main\n");
//     INSTR("mov rax, 0x3c");
//     INSTR("xor rdi, rdi");
//     INSTR("syscall");

//     while (node->children[RIGHT])
//         node = node->children[RIGHT];

//     struct Node *bottom = node;

//     while (node) {
//         if (KEYW(node->children[LEFT]) == KEYW_ASSIGN)
//         {
//             GenerateAssign(node, NULL, compiler);
//         }

//         node = node->parent;
//     }

//     struct List *NT = newList();

//     node = bottom;

//     while (node) {
//         if (NODE_KEYW(node->children[LEFT], KEYW_DEFINE)) {
//             GenerateFuncDef(node->children[LEFT], NT, compiler);
//         } else if (NODE_KEYW(node->children[LEFT], KEYW_ASSIGN)) {
//             node = node->parent;
//             continue;
//         } else {
//             PRINT_("Invalid statement");

//             ListDtor(NT);

//             return;
//         }
//         node = node->parent;
//     }

//     GenerateMain(compiler->node_main, NT, compiler);
    
//     ListDtor(NT);
// }

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
    BYTE3(0x48, 0x89, 0xc3);                                                                // mov rbx, rip
    BYTE2(0x0f, 0xeb); *((u_int64_t *) (compiler->ip)) = MEMORY_SIZE; compiler->ip += 8;       // skip data space (with jmp)

    compiler->memory = compiler->ip;

    compiler->ip += MEMORY_SIZE;
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