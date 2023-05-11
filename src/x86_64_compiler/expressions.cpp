#include "compiler.h"

void GenerateGlobExpr(struct Node *node, struct Compiler *compiler) {
    if (KEYW(node) == KEYW_CALL) {
        PRINT_("Useless function in global space");
    }
    if (node->children[LEFT]) {
        GenerateGlobExpr(node->children[LEFT], compiler);
    }
    if (node->children[RIGHT]) {
        GenerateGlobExpr(node->children[RIGHT], compiler);
    }
    
    if (IsMathOper(node)) {
        GenerateMathOper(node, compiler);

        return;
    }

    if (IsNum(node)) {
        GenerateNum(node, compiler);

        return;
    }

    if (IsVar(node)) {
        GenerateGlobVar(node, compiler);

        return;
    }

    PRINT_("Undefined operation");
}

void GenerateMathOper(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    BYTE2(0x41, 0x5c);  // pop r12
    BYTE2(0x41, 0x5d);  // pop r13

    switch (KEYW(node))
    {
        case KEYW_ADD:
            BYTE3(0x4d, 0x01, 0xec);        // add r12, r13
            break;
        case KEYW_SUB:
            BYTE3(0x4d, 0x29, 0xec);        // sub r12, r13
            break;
        case KEYW_MUL:
            BYTE4(0x4d, 0x0f, 0xaf, 0xe5);  // imul r12, r13
            break;
        case KEYW_DIV:
            BYTE3(0x4c, 0x89, 0xe0);        // mov rax, r12
            BYTE3(0x49, 0xf7, 0xfd);        // div r13
            BYTE3(0x49, 0x89, 0xc4);        // mov r12, rax
            break;

        default:
            PRINT_("Unexpected operator");
            
            break;
    }
    
    BYTE2(0x41, 0x54);                      // push r12
}

void GenerateNum(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    // TODO: переделать под инты

    BYTE2(0x48, 0xb8); INT(node->val->value.num);   // mov rax, num
    BYTE1(0x50);                                    // push rax
}


void GenerateGlobVar(struct Node *node, struct Compiler *compiler) {
    if (SearchInNametable(node, compiler->GlobalNT)) {
        size_t index = IndexNametable(node, compiler->GlobalNT);

        BYTE4(0x4c, 0x8b, 0x63, (index - 1) * 8);   // mov r12, [rbx + 8 * (index - 1)]
        BYTE2(0x41, 0x54);                          // push r12
    }

    PRINT_("Global variable not found");
}

void GenerateExpr  (struct Node *node, struct List *NT, struct Compiler *compiler)
{
    if (KEYW(node) == KEYW_CALL)
    {
        GenerateCall(node, NT, compiler);
        
        return;
    }

    if (node->children[LEFT])
    {
        GenerateExpr(node->children[LEFT],  NT, compiler);
        
    }
    if (node->children[RIGHT])
    {
        GenerateExpr(node->children[RIGHT], NT, compiler);
    }

    if (IsMathOper(node))
    {
        GenerateMathOper(node, compiler);

        return;
    }

    if (IsNum(node))
    {
        GenerateNum(node, compiler);

        return;
    }

    if (IsVar(node))
    {
        GenerateVar(node, NT, compiler);

        return;
    }

    PRINT_("Undefined operator");
}

void GenerateVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (SearchInNametable(node, compiler->GlobalNT)) {
        GenerateGlobVar(node, compiler);
        return;
    }

    if (SearchInNametable(node, NT))
    {
        size_t index = IndexNametable(node, NT);

        BYTE4(0x4c, 0x8b, 0x63, (index - 1) * 8);   // mov r12, [rbx + 8 * (index - 1)]
        BYTE2(0x41, 0x54);                          // push r12

        return;
    }

    PRINT_("Variable not found");
}

void GenerateStmts (struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (!NODE_KEYW(node, KEYW_STMT))
        PRINT_("There must be statement");

    while (node->children[RIGHT])
        node = node->children[RIGHT];

    while (NODE_KEYW(node, KEYW_STMT))
    {
        GenerateStmt(node->children[LEFT], NT, compiler);
        node = node->parent;
    }
}

void GenerateStmt  (struct Node *node, struct List *NT, struct Compiler *compiler) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!compiler)
        PRINT_("No struct compiler");
    
    switch (KEYW(node))
    {
        case KEYW_ASSIGN:
            GenerateAssign(node, NT, compiler);
            break;
        case KEYW_IF:
            GenerateIf    (node, NT, compiler);
            break;
        case KEYW_WHILE:
            GenerateWhile (node, NT, compiler);
            break;
        case KEYW_CALL:
            GenerateCall  (node, NT, compiler);
            break;
        case KEYW_RETURN:
            GenerateReturn(node, NT, compiler);
            break;
        // case KEYW_SCAN:
        //     GenerateScan  (node, NT, compiler);
        //     break;
        // case KEYW_PRINT:
        //     GeneratePrint (node, NT, compiler);
        //     break;
        case KEYW_ADD:
        case KEYW_SUB:
        case KEYW_MUL:
        case KEYW_DIV:
        case KEYW_POW:
            GenerateExpr  (node, NT, compiler);
            break;
        default:
            PRINT_("Invalid statement");
            break;
    }
}

void GenerateJump(struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num) {
    if(!mark)
        PRINT_("Null pointer to label name");

    if (!IsLogOper(node)) 
        PRINT_("There is no logical operator");

    BYTE2(0x41, 0x5c);          // pop r12
    BYTE2(0x41, 0x5d);          // pop r13
    BYTE3(0x4d, 0x39, 0xec);    // cmp r12, r13

    switch (KEYW(node))
    {
        case KEYW_LESS:
            BYTE2(0x0f, 0x8d);  // jge
            break;
        case KEYW_LESSOREQ:
            BYTE2(0x0f, 0x8f);  // jg
            break;
        case KEYW_NOTEQUAL:
            BYTE2(0x0f, 0x84);  // je
            break;
        case KEYW_EQUAL:
            BYTE2(0x0f, 0x85);  // jne
            break;
        case KEYW_GREATOREQ:
            BYTE2(0x0f, 0x8c);  // jl
            break;
        case KEYW_GREAT:
            BYTE2(0x0f, 0x8e);  // jle
            break;
        default:
            PRINT_("Undefined operator");
            break;
    }

    putAddress("%s_%d", num, compiler);
}