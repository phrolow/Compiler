#include "compiler.h"

void generateGlobExpr(struct Node *node, struct Compiler *compiler) {
    if (KEYW(node) == KEYW_CALL) {
        PRINT_("Useless function in global space");
    }
    if (node->children[LEFT]) {
        generateGlobExpr(node->children[LEFT], compiler);
    }
    if (node->children[RIGHT]) {
        generateGlobExpr(node->children[RIGHT], compiler);
    }
    
    if (isMathOper(node)) {
        generateMathOper(node, compiler);

        return;
    }

    if (isNum(node)) {
        generateNum(node, compiler);

        return;
    }

    if (isVar(node)) {
        generateGlobVar(node, compiler);

        return;
    }

    PRINT_("Undefined operation");
}

void generateMathOper(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    #ifdef DOUBLES

    XMM_POP(1);
    XMM_POP(2);

    #else

    addInstruction(compiler->cmds, POP_R12, POISON);
    addInstruction(compiler->cmds, POP_R13, POISON);

    #endif

    switch (KEYW(node))
    {   
        #ifdef DOUBLES

        case KEYW_ADD:
            addInstruction(compiler->cmds, ADDSD_XMM1_XMM2, POISON);
            break;
        case KEYW_SUB:
            addInstruction(compiler->cmds, SUBSD_XMM1_XMM2, POISON);
            break;
        case KEYW_MUL:
            addInstruction(compiler->cmds, MULSD_XMM1_XMM2, POISON);
            break;
        case KEYW_DIV:
            addInstruction(compiler->cmds, DIVSD_XMM1_XMM2, POISON);
            break;

        #else

        case KEYW_ADD:
            addInstruction(compiler->cmds, ADD, POISON);
            break;
        case KEYW_SUB:
            addInstruction(compiler->cmds, SUB, POISON);
            break;
        case KEYW_MUL:
            addInstruction(compiler->cmds, IMUL, POISON);
            break;
        case KEYW_DIV:
            addInstruction(compiler->cmds, MOV_RAX_R12, POISON);
            addInstruction(compiler->cmds, XOR_RDX_RDX, POISON);
            addInstruction(compiler->cmds, DIV, POISON);
            addInstruction(compiler->cmds, MOV_R12_RAX, POISON);
            break;

        #endif

        default:
            PRINT_("Unexpected operator");
            
            break;
    }
    
    #ifdef DOUBLES

    XMM_PUSH(1);

    #else

    addInstruction(compiler->cmds, PUSH_R12, POISON);

    #endif
}

void generateNum(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    #ifdef DOUBLES

    *((double *) (compiler->out + CONSTS_LOCATION + sizeof(double) * compiler->num_consts)) = (double) node->val->value.num;

    addInstruction(compiler->cmds, MOVSD_XMM0_CONST, relAddress("consts", POISON, compiler) + sizeof(double) * compiler->num_consts);
    XMM_PUSH(0);

    compiler->num_consts++;

    #else

    addInstruction(compiler->cmds, MOV_RAX_IMM, node->val->value.num);
    addInstruction(compiler->cmds, PUSH_RAX, POISON);

    #endif
}


void generateGlobVar(struct Node *node, struct Compiler *compiler) {
    if (searchInNametable(node, compiler->global_NT)) {
        size_t index = indexNametable(node, compiler->global_NT);

        #ifdef DOUBLES

        addInstruction(compiler->cmds, MOVSD_XMM1_MEM, (index - 1) * sizeof(double));
        XMM_PUSH(1);

        #else

        addInstruction(compiler->cmds, MOV_R12_MEM, (index - 1) * sizeof(int64_t));
        addInstruction(compiler->cmds, PUSH_R12, POISON);

        #endif
    }

    PRINT_("Global variable not found");
}

void generateExpr  (struct Node *node, struct List *NT, struct Compiler *compiler)
{
    if (KEYW(node) == KEYW_CALL)
    {
        generateCall(node, NT, compiler);
        
        return;
    }

    if (node->children[LEFT])
    {
        generateExpr(node->children[LEFT],  NT, compiler);
        
    }
    if (node->children[RIGHT])
    {
        generateExpr(node->children[RIGHT], NT, compiler);
    }

    if (isMathOper(node))
    {
        generateMathOper(node, compiler);

        return;
    }

    if (isNum(node))
    {
        generateNum(node, compiler);

        return;
    }

    if (isVar(node))
    {
        generateVar(node, NT, compiler);

        return;
    }

    PRINT_("Undefined operator");
}

void generateVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (searchInNametable(node, compiler->global_NT)) {
        generateGlobVar(node, compiler);
        return;
    }

    if (searchInNametable(node, NT))
    {
        size_t index = indexNametable(node, NT);

        #ifdef DOUBLES

        addInstruction(compiler->cmds, MOVSD_XMM1_MEM, (index - 1) * sizeof(int64_t));
        XMM_PUSH(1);

        #else

        addInstruction(compiler->cmds, MOV_R12_MEM, (index - 1) * 8);
        addInstruction(compiler->cmds, PUSH_R12, POISON);

        #endif

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
        generateStmt(node->children[LEFT], NT, compiler);
        node = node->parent;
    }
}

void generateStmt  (struct Node *node, struct List *NT, struct Compiler *compiler) {
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
            generateAssign(node, NT, compiler);
            break;
        case KEYW_IF:
            generateIf    (node, NT, compiler);
            break;
        case KEYW_WHILE:
            generateWhile (node, NT, compiler);
            break;
        case KEYW_CALL:
            generateCall  (node, NT, compiler);
            break;
        case KEYW_RETURN:
            generateReturn(node, NT, compiler);
            break;
        case KEYW_SCAN:
            generateScan  (node, NT, compiler);
            break;
        case KEYW_PRINT:
            generatePrint (node, NT, compiler);
            break;
        case KEYW_ADD:
        case KEYW_SUB:
        case KEYW_MUL:
        case KEYW_DIV:
        case KEYW_POW:
            generateExpr  (node, NT, compiler);
            break;
        default:
            PRINT_("Invalid statement");
            break;
    }
}

void generateJump(struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num) {
    if(!mark)
        PRINT_("Null pointer to label name");

    if (!isLogOper(node)) 
        PRINT_("There is no logical operator");

    #ifdef DOUBLES

    XMM_POP(1);
    XMM_POP(2);
    addInstruction(compiler->cmds, SUBSD_XMM1_XMM2, POISON);

    #else

    addInstruction(compiler->cmds, POP_R12,     POISON);
    addInstruction(compiler->cmds, POP_R13,     POISON);
    addInstruction(compiler->cmds, CMP_R12_R13, POISON);

    #endif

    char name[WORD_MAX_LEN + 1] = {};

    sprintf(name, "%s_%d", mark, num);

    #ifdef DOUBLES

    int keyword = KEYW(node);

    if(!keyword || keyword > KEYW_GREATOREQ) {
        PRINT_("Undefined operator");
    }

    switch ((keyword - 1) % 3)
    {
        case 0: // JE or JNE
            addInstruction(compiler->cmds, ANDPD_XMM1_MASK, relAddress("abs_mask", POISON, compiler));  // get abs
            addInstruction(compiler->cmds, COMISD_XMM1_MEM, relAddress("epsilon", POISON, compiler));   // cmp |xmm1-xmm2|, epsilon
            break;
        case 1: // JG or JLE
            addInstruction(compiler->cmds, COMISD_XMM1_MEM, relAddress("epsilon", POISON, compiler));
            break;
        case 2: // JL or JGE
            addInstruction(compiler->cmds, COMISD_XMM1_MEM, relAddress("neg_epsilon", POISON, compiler));
            break;
        default:
            PRINT_("Undefined operator");
            break;
    }

    if((keyword - 1) % 3 == 0) {    // JNE -> JGE, JE -> JL
        keyword = 5 - keyword;
        keyword += 2;
    }

    addInstruction(compiler->cmds, JNE + keyword - 1, relAddress(name, POISON, compiler));

    #else

    u_int32_t rel_address = relAddress(name, POISON, compiler);

    switch (KEYW(node))
    {
        case KEYW_LESS:
            addInstruction(compiler->cmds, JL, rel_address);
            break;
        case KEYW_LESSOREQ:
            addInstruction(compiler->cmds, JLE, rel_address);
            break;
        case KEYW_NOTEQUAL:
            addInstruction(compiler->cmds, JE, rel_address);
            break;
        case KEYW_EQUAL:
            addInstruction(compiler->cmds, JNE, rel_address);
            break;
        case KEYW_GREATOREQ:
            addInstruction(compiler->cmds, JGE, rel_address);
            break;
        case KEYW_GREAT:
            addInstruction(compiler->cmds, JG, rel_address);
            break;
        default:
            PRINT_("Undefined operator");
            break;
    }

    #endif
}

void generatePrint(struct Node *node, struct List *NT, struct Compiler *compiler) {
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

    generateExpr(node->children[LEFT], NT, compiler);

    #ifdef DOUBLES

    addInstruction(compiler->cmds, ADD_RSP_8, POISON);

    #else

    addInstruction(compiler->cmds, CALL, relAddress("decimal", POISON, compiler));
    addInstruction(compiler->cmds, ADD_RSP_8, POISON);

    #endif
}

void generateScan(struct Node *node, struct List *NT, struct Compiler *compiler) {
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

    pushInNametable(node->children[LEFT], NT);

    size_t index = indexNametable(node->children[LEFT], NT);

    #ifdef DOUBLES

    #else

    addInstruction(compiler->cmds, CALL, relAddress("in", POISON, compiler));
    addInstruction(compiler->cmds, MOV_MEM_RAX, 8 * (index - 1));

    #endif
}

void generateAssign(struct Node *node, struct List *NT, struct Compiler *compiler) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!compiler)
        PRINT_("No struct compiler");

    if (!NT) {
        initGlobVar(node, compiler);
        
    } else {
        initVar(node, NT, compiler);
    }
}

void generateIf(struct Node *node, struct List *NT, struct Compiler *compiler) {
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

    size_t counter = compiler->if_counter++;

    if (else_stmts)
    {
        generateCond(condition, NT, compiler, "ELSE", counter);
        GenerateStmts(if_stmts, NT, compiler);

        addInstruction(compiler->cmds, JMP, relAddress("END_IF_%lu", counter, compiler));

        generateLabelFromCmds("ELSE_%lu", counter, compiler);

        GenerateStmts(else_stmts, NT, compiler);
    }
    else
    {
        generateCond(condition, NT, compiler, "END_IF", counter);
        GenerateStmts(if_stmts, NT, compiler);
    }

    generateLabelFromCmds("END_IF_%lu", counter, compiler);
}

void generateCond(struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num) {
    generateExpr(node->children[LEFT],  NT, compiler);
    generateExpr(node->children[RIGHT], NT, compiler);
    generateJump(node, NT, compiler, mark, num);
}

void generateWhile(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    struct Node *condition = node->children[LEFT];
    struct Node *while_stmts = node->children[RIGHT];

    size_t counter = compiler->while_counter++;

    generateLabelFromCmds("WHILE_%lu", counter, compiler);

    generateCond(condition, NT, compiler, "END_WHILE", counter);
    GenerateStmts(while_stmts, NT, compiler);

    addInstruction(compiler->cmds, JMP, relAddress("WHILE_%lu", counter, compiler));

    generateLabelFromCmds("END_WHILE_%lu", counter, compiler);
}

void generateReturn(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    if (!NODE_KEYW(node, KEYW_RETURN)) {
        PRINT_("No return statement");
    }

    if(node->children[LEFT]) {
        generateExpr(node->children[LEFT], NT, compiler);

        #ifdef DOUBLES

        XMM_POP(0);

        #else

        addInstruction(compiler->cmds, POP_RAX, POISON);

        #endif
    }

    addInstruction(compiler->cmds, RET, POISON);
}