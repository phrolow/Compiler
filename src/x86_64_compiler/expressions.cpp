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

    addCmd(compiler->cmds, POP_R12, POISON);
    addCmd(compiler->cmds, POP_R13, POISON);

    switch (KEYW(node))
    {
        case KEYW_ADD:
            addCmd(compiler->cmds, ADD, POISON);
            break;
        case KEYW_SUB:
            addCmd(compiler->cmds, SUB, POISON);
            break;
        case KEYW_MUL:
            addCmd(compiler->cmds, IMUL, POISON);
            break;
        case KEYW_DIV:
            addCmd(compiler->cmds, MOV_RAX_R12, POISON);
            addCmd(compiler->cmds, XOR_RDX_RDX, POISON);
            addCmd(compiler->cmds, DIV, POISON);
            addCmd(compiler->cmds, MOV_R12_RAX, POISON);
            break;

        default:
            PRINT_("Unexpected operator");
            
            break;
    }
    
    addCmd(compiler->cmds, PUSH_R12, POISON);
}

void GenerateNum(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    addCmd(compiler->cmds, MOV_RAX_IMM, node->val->value.num);
    addCmd(compiler->cmds, PUSH_RAX, POISON);
}


void GenerateGlobVar(struct Node *node, struct Compiler *compiler) {
    if (SearchInNametable(node, compiler->GlobalNT)) {
        size_t index = IndexNametable(node, compiler->GlobalNT);

        addCmd(compiler->cmds, MOV_R12_MEM, (index - 1) * 8);
        addCmd(compiler->cmds, PUSH_R12, POISON);
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

        addCmd(compiler->cmds, MOV_R12_MEM, (index - 1) * 8);
        addCmd(compiler->cmds, PUSH_R12, POISON);

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
        case KEYW_SCAN:
            GenerateScan  (node, NT, compiler);
            break;
        case KEYW_PRINT:
            GeneratePrint (node, NT, compiler);
            break;
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

    addCmd(compiler->cmds, POP_R12,     POISON);
    addCmd(compiler->cmds, POP_R13,     POISON);
    addCmd(compiler->cmds, CMP_R12_R13, POISON);

    char name[WORD_MAX_LEN + 1] = {};

    sprintf(name, "%s_%d", mark, num);

    u_int32_t rel_address = relAddress(name, POISON, compiler);

    switch (KEYW(node))
    {
        case KEYW_LESS:
            addCmd(compiler->cmds, JL, rel_address);
            break;
        case KEYW_LESSOREQ:
            addCmd(compiler->cmds, JLE, rel_address);
            break;
        case KEYW_NOTEQUAL:
            addCmd(compiler->cmds, JE, rel_address);
            break;
        case KEYW_EQUAL:
            addCmd(compiler->cmds, JNE, rel_address);
            break;
        case KEYW_GREATOREQ:
            addCmd(compiler->cmds, JGE, rel_address);
            break;
        case KEYW_GREAT:
            addCmd(compiler->cmds, JG, rel_address);
            break;
        default:
            PRINT_("Undefined operator");
            break;
    }
}

void GeneratePrint(struct Node *node, struct List *NT, struct Compiler *compiler) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    if(!compiler) {
        PRINT_("Null struct compiler!");
    }

    if (!node->children[LEFT]) {
        PRINT_("No arg for print");
    }

    GenerateExpr(node->children[LEFT], NT, compiler);

    addCmd(compiler->cmds, CALL, relAddress("decimal", POISON, compiler));
    addCmd(compiler->cmds, ADD_RSP_8, POISON);
}

void GenerateScan(struct Node *node, struct List *NT, struct Compiler *compiler) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    if(!compiler) {
        PRINT_("Null struct compiler!");
    }

    if (!node->children[LEFT]) {
        PRINT_("No arg for scan");
    }

    PushInNametable(node->children[LEFT], NT);

    size_t index = IndexNametable(node->children[LEFT], NT);

    addCmd(compiler->cmds, CALL, relAddress("in", POISON, compiler));
    addCmd(compiler->cmds, MOV_MEM_RAX, 8 * (index - 1));
}

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

        addCmd(compiler->cmds, JMP, relAddress("END_IF_%lu", counter, compiler));

        generateLabelFromCmds("ELSE_%lu", counter, compiler);

        GenerateStmts(else_stmts, NT, compiler);
    }
    else
    {
        GenerateCond(condition, NT, compiler, "END_IF", counter);
        GenerateStmts(if_stmts, NT, compiler);
    }

    generateLabelFromCmds("END_IF_%lu", counter, compiler);
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

    generateLabelFromCmds("WHILE_%lu", counter, compiler);

    GenerateCond(condition, NT, compiler, "END_WHILE", counter);
    GenerateStmts(while_stmts, NT, compiler);

    addCmd(compiler->cmds, JMP, relAddress("WHILE_%lu", counter, compiler));

    generateLabelFromCmds("END_WHILE_%lu", counter, compiler);
}

void GenerateReturn(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    if (!NODE_KEYW(node, KEYW_RETURN)) {
        PRINT_("No return statement");
    }

    if(node->children[LEFT]) {
        GenerateExpr(node->children[LEFT], NT, compiler);

        addCmd(compiler->cmds, POP_RAX, POISON);
    }

    addCmd(compiler->cmds, RET, POISON);
}