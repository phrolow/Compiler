#include "compiler.h"

// #define INSTR(str)                          \
//     fprintf(compiler->out, "\t%s\t\t\t; %s\n", str, __PRETTY_FUNCTION__)

// #define ARG_INSTR(format, num)              \
//     fprintf(compiler->out, "\t");           \
//     fprintf(compiler->out, format, num);    \
//     fprintf(compiler->out, "\t\t\t; %s\n", __PRETTY_FUNCTION__)

// #define LABEL(name)                         \
//     fprintf(compiler->out, ".");            \
//     fprintf(compiler->out, str, num);       \
//     fprintf(compiler->out, ":");            \
//     fprintf(compiler->out, "\n")        

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
    ARG_INSTR("add rbx, %lu", number * 8);
}

// void DecreaseRBX(const size_t number, struct Compiler *compiler) {
//     ARG_INSTR("sub rbx, %lu", number * 8);
// }

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

    // INSTR("call .main\n");
    // INSTR("mov rax, 0x3c");
    // INSTR("xor rdi, rdi");
    // INSTR("syscall");

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
    // mov rbx, rip;

    // jmp MEMORY_SIZE (относительный ессно)

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

    // INSTR("pop r12");

    // ARG_INSTR("mov [rbx + %lu * 8], r12", index - 1);
}

void InitVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if(SearchInNametable(node->children[LEFT], compiler->GlobalNT)) 
        PRINT_("Repeating variable");

    if (SearchInNametable(node->children[LEFT], NT))
    {
        GenerateExpr(node->children[RIGHT], NT, compiler);

        size_t index = IndexNametable(node->children[LEFT], NT);

        assert(index >= 1);

        // INSTR("pop r12");
        // ARG_INSTR("mov [rbx + 8 * %lu], r12", index - 1);

        return;
    }

    PushInNametable(node->children[LEFT], NT);

    size_t index = IndexNametable(node->children[LEFT], NT);

    assert(index >= 1);

    GenerateExpr(node->children[RIGHT], NT, compiler);

    // INSTR("pop r12");
    // ARG_INSTR("mov [rbx + 8 * %lu], r12", index - 1);
}