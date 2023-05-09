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
            // fprintf(compiler->out, "\tadd ");
            break;
        case KEYW_SUB:
            // fprintf(compiler->out, "\tsub ");
            break;
        case KEYW_MUL:
            // fprintf(compiler->out, "\timul ");
            break;
        case KEYW_DIV:
            // fprintf(compiler->out, "\tidiv ");
            break;

        default:
            PRINT_("Unexpected operator");
            
            break;
    }

    // fprintf(compiler->out, "r12, r13\n");
    
    // INSTR("push r12");
}

void GenerateNum(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    // ARG_INSTR("push %lg", node->val->value.num);
}


void GenerateGlobVar(struct Node *node, struct Compiler *compiler) {
    if (SearchInNametable(node, compiler->GlobalNT)) {
        size_t index = IndexNametable(node, compiler->GlobalNT);

        // ARG_INSTR("mov r12, [%lu]", index - 1);

        // INSTR("push r12");
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

        // ARG_INSTR("mov r12, [rbx + 8 * %lu]", index - 1);

        // INSTR("push r12");

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

    // INSTR("pop r12");
    // INSTR("pop r13");
    // INSTR("cmp r12, r13");

    switch (KEYW(node))
    {
        case KEYW_LESS:
            // fprintf(compiler->out, "jbe ");
            break;
        case KEYW_LESSOREQ:
            // fprintf(compiler->out, "jb ");
            break;
        case KEYW_NOTEQUAL:
            // fprintf(compiler->out, "je ");
            break;
        case KEYW_EQUAL:
            // fprintf(compiler->out, "jne ");
            break;
        case KEYW_GREATOREQ:
            // fprintf(compiler->out, "ja ");
            break;
        case KEYW_GREAT:
            // fprintf(compiler->out, "jae ");
            break;
        default:
            PRINT_("Undefined operator");
            break;
    }

    size_t jmp_address = indexLabel("%s_%d", num, compiler);

    // ебашим (rip - jmp_address)
}