#include "compiler.h"

#define NODE_KEYW(NODE, KEYW) (NODE->val->type == KEYWORD_TYPE && NODE->val->value.keyword == KEYW)

#define KEYW(NODE) ((NODE->val->type == KEYWORD_TYPE) ? NODE->val->value.keyword : 0)

#define INSTR(str)                          \
    fprintf(compiler->out, "\t%s\t\t\t; %s\n", str, __PRETTY_FUNCTION__)

#define ARG_INSTR(format, num)              \
    fprintf(compiler->out, "\t");           \
    fprintf(compiler->out, format, num);    \
    fprintf(compiler->out, "\t\t\t; %s\n", __PRETTY_FUNCTION__)

#define LABEL(name)                         \
    fprintf(compiler->out, ".");            \
    fprintf(compiler->out, str, num);       \
    fprintf(compiler->out, ":");            \
    fprintf(compiler->out, "\n")        

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

void PushInNametable(struct Node *node, struct List *NT) {
    TREE_ERROR node_err = NodeVerify(node);

    if(NodeVerify(node)) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    name_t name  = {};

    memcpy(name.name, node->val->value.name, sizeof(node->val->value.name));

    ListTailInsert(NT, name);
}

int IndexNametable(struct Node *node, struct List *NT) {
    TREE_ERROR node_err = NodeVerify(node);

    if(NodeVerify(node)) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    if (node->val->type != VAR_TYPE)
    {
        PRINT_("Got non-name type");

        return -1;
    }

    size_t number = ListIndexFirst(NT, node->val->value.name);

    if (number == 0) {
        PRINT_LINE;

        fprintf(stderr, "Variable named \"%s\" doesn't exist", node->val->value.name);
    }

    return number;
}

int SearchInNametable(struct Node *node, struct List *NT) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    size_t index = ListIndexFirst(NT, node->val->value.name);

    if (index != -1) {
        return 1;
    }

    return 0;
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

    INSTR("call cin");
    INSTR("push rax");

    INSTR("movsd [rsp], xmm2");
    INSTR("sub rsp, 8");
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

    INSTR("call cout");
    INSTR("add rsp, 8");
}

void GenerateSqrt(struct Node *node, struct List *NT, struct Compiler *compiler) {
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

    if (!node->children[LEFT])
        PRINT_("No arg for sqrt");

    GenerateExpr(node->children[LEFT], NT, compiler);

    fprintf(compiler->out, "PUSH 10000\n");
    fprintf(compiler->out, "POP rax\n");
    fprintf(compiler->out, "ASQRT\n");
    fprintf(compiler->out, "PUSH 10000\n");
    fprintf(compiler->out, "DIV\n");
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
        case KEYW_SQRT:
            GenerateSqrt(node, NT, compiler);
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

void GenerateMathOper(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    INSTR("movsd xmm0, [rsp]");
    INSTR("add rsp, 8");
    INSTR("movsd xmm1, [rsp]");
    INSTR("add rsp, 8");

    switch (KEYW(node))
    {
        case KEYW_ADD:
            fprintf(compiler->out, "\taddsd ");
            break;
        case KEYW_SUB:
            fprintf(compiler->out, "\tsubsd ");
            break;
        case KEYW_MUL:
            fprintf(compiler->out, "\tmulsd ");
            break;
        case KEYW_DIV:
            fprintf(compiler->out, "\tdivsd ");
            break;

        default:
            PRINT_("Unexpected operator");
            
            break;
    }

    fprintf(compiler->out, "xmm0, xmm1\n");

    INSTR("movsd [rsp], xmm0");
    INSTR("sub rsp, 8");
}

void GenerateNum(struct Node *node, struct Compiler *compiler)
{
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    ARG_INSTR("mov rax, 0x%llx", node->val->value);
    INSTR("push rax");
}

void InitVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if(SearchInNametable(node->children[LEFT], compiler->GlobalNT)) 
        PRINT_("Repeating variable");

    if (SearchInNametable(node->children[LEFT], NT))
    {
        GenerateExpr(node->children[RIGHT], NT, compiler);

        size_t index = IndexNametable(node->children[LEFT], NT);

        assert(index >= 1);

        INSTR("movsd xmm0, [rsp]");
        INSTR("add rsp, 8");

        ARG_INSTR("movsd [rbx + 8 * %lu], xmm0", index - 1);

        return;
    }

    PushInNametable(node->children[LEFT], NT);

    size_t index = IndexNametable(node->children[LEFT], NT);

    assert(index >= 1);

    GenerateExpr(node->children[RIGHT], NT, compiler);

    INSTR("movsd xmm0, [rsp]");
    INSTR("add rsp, 8");

    ARG_INSTR("movsd [rbx + 8 * %lu], xmm0", index - 1);
}

void GenerateVar(struct Node *node, struct List *NT, struct Compiler *compiler) {
    if (SearchInNametable(node, compiler->GlobalNT)) {
        GenerateGlobVar(node, compiler);
        return;
    }

    if (SearchInNametable(node, NT))
    {
        size_t index = IndexNametable(node, NT);

        ARG_INSTR("movsd xmm0, [rbx + 8 * %lu]", index - 1);

        INSTR("movsd [rsp], xmm0");
        INSTR("sub rsp, 8");

        return;
    }

    PRINT_("Variable not found");
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

    if(NODE_KEYW(node, KEYW_SQRT)) {
        GenerateSqrt(node, NT, compiler);

        return;
    }

    PRINT_("Undefined operator");
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

    //fprintf(compiler->out, "POP [rbx+%lu]\n", *num_of_params);

    //INSTR("pop rax");
    
    INSTR("movsd xmm0, [rsp]");
    INSTR("add rsp, 8");

    ARG_INSTR("movsd [rbx + 8 * %lu], xmm0", *num_of_params);
}

void GenerateCall(struct Node *node, struct List *NT, struct Compiler *compiler) {
    struct Node *params = node->children[RIGHT];

    if(!params)
        PRINT_("Null pointer to parameters node");

    size_t num_of_params = 0;

    InitCallParams(params, NT, compiler, &num_of_params);

    if(num_of_params != 1)
        PRINT_("Not one parameter");

    struct Node *name = node->children[LEFT];

    ARG_INSTR("call .%s", name->val->value.name);
    // INSTR("push rax");
    INSTR("movsd [rsp], xmm2");
    INSTR("sub rsp, 8");
}

void GenerateJump(struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num) {
    if(!mark)
        PRINT_("Null pointer to label name");

    if (!IsLogOper(node)) 
        PRINT_("There is no logical operator");

    INSTR("movsd xmm0, [rsp]");
    INSTR("add rsp, 8");
    INSTR("movsd xmm1, [rsp]");
    INSTR("add rsp, 8");
    INSTR("comisd xmm0, xmm1");

    switch (KEYW(node))
    {
        case KEYW_LESS:
            fprintf(compiler->out, "jbe ");
            break;
        case KEYW_LESSOREQ:
            fprintf(compiler->out, "jb ");
            break;
        case KEYW_NOTEQUAL:
            fprintf(compiler->out, "je ");
            break;
        case KEYW_EQUAL:
            fprintf(compiler->out, "jne ");
            break;
        case KEYW_GREATOREQ:
            fprintf(compiler->out, "ja ");
            break;
        case KEYW_GREAT:
            fprintf(compiler->out, "jae ");
            break;
        default:
            PRINT_("Undefined operator");
            break;
    }

    fprintf(compiler->out, " .%s_%d\n", mark, num);
}

void GenerateCond(struct Node *node, struct List *NT, struct Compiler *compiler, const char *mark, const int num) {
    GenerateExpr(node->children[LEFT],  NT, compiler);
    GenerateExpr(node->children[RIGHT], NT, compiler);
    GenerateJump(node, NT, compiler, mark, num);
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

        ARG_INSTR("jmp .END_IF_%lu", counter);
        fprintf(compiler->out, ".ELSE_%lu:\n", counter);

        GenerateStmts(else_stmts, NT, compiler);
    }
    else
    {
        GenerateCond(condition, NT, compiler, "END_IF", counter);
        GenerateStmts(if_stmts, NT, compiler);
    }

    fprintf(compiler->out, ".END_IF_%lu:\n", counter);
}

void GenerateReturn(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    if (!NODE_KEYW(node, KEYW_RETURN)) {
        PRINT_("No return statement");
    }

    if(node->children[LEFT]) {
        GenerateExpr(node->children[LEFT], NT, compiler);

        // INSTR("pop rax");

        INSTR("movsd xmm2, [rsp]");
        INSTR("add rsp, 8");
    }

    DecreaseRBX(compiler->free_memory_index, compiler);

    INSTR("ret");
}

void GenerateWhile(struct Node *node, struct List *NT, struct Compiler *compiler)
{
    struct Node *condition = node->children[LEFT];
    struct Node *while_stmts = node->children[RIGHT];

    int counter = compiler->__WHILE_COUNTER__++;

    fprintf(compiler->out, ".WHILE_%d:\n", counter);

    GenerateCond(condition, NT, compiler, "END_WHILE", counter);
    GenerateStmts(while_stmts, NT, compiler);

    fprintf(compiler->out, ".JMP WHILE_%d:\n", counter);
    fprintf(compiler->out, ".END_WHILE_%d:\n", counter);
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

void IncreaseRBX(const size_t number, struct Compiler *compiler) {
    ARG_INSTR("add rbx, %lu", number * 8);
}

void DecreaseRBX(const size_t number, struct Compiler *compiler) {
    ARG_INSTR("sub rbx, %lu", number * 8);
}

void GenerateMark(struct Node *mark, struct Compiler *compiler) {
    if (mark->val->type == VAR_TYPE || NODE_KEYW(mark, KEYW_MAIN)) {
        if (mark->val->type == VAR_TYPE) {
            fprintf(compiler->out, ".%s:\n", mark->val->value.name);
            return;
        }
        else
        if (KEYW(mark) == KEYW_MAIN) {
            fprintf(compiler->out, ".main:\n");
            return;
        }
    }

    PRINT_("Non-name type of node");
}

void GenerateFuncDef(struct Node *node, struct List *NT, struct Compiler *compiler)
{
//    if (NT->size != 1) {
//        ABORT("NT ISNT EMPTY\n");
//    }

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

void GenerateGlobVar(struct Node *node, struct Compiler *compiler) {
    if (SearchInNametable(node, compiler->GlobalNT)) {
        size_t index = IndexNametable(node, compiler->GlobalNT);

        ARG_INSTR("movsd xmm0, [rbx + %lu * 8]", index - 1);

        INSTR("sub rsp, 8");
        INSTR("movsd [rsp], xmm0");
    }

    PRINT_("Global variable not found");
}

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

void InitGlobVar(struct Node *node, struct Compiler *compiler) {
    if (SearchInNametable(node->children[LEFT], compiler->GlobalNT))
        PRINT_("Repeating definition");

    PushInNametable(node->children[LEFT], compiler->GlobalNT);

    size_t index = IndexNametable(node->children[LEFT], compiler->GlobalNT);

    assert(index >= 1);

    GenerateGlobExpr(node->children[RIGHT], compiler);

    //fprintf(compiler->out, "pop [rbx+%lu]\n", index - 1);

    INSTR("movsd xmm0, [rsp]");
    INSTR("add rsp, 8");

    ARG_INSTR("mov [rbx + %lu * 8], xmm0", index - 1);
}

void GenerateGS(struct Node *node, struct Compiler *compiler) {
    if (!NODE_KEYW(node, KEYW_STMT)) {
        PRINT_("It's not global statement");

        return;
    }

    fprintf(compiler->out, "BITS 64\n\n");

    fprintf(compiler->out, "global _start\n\n");

    fprintf(compiler->out, "section .bss\n");
    fprintf(compiler->out, "\tmemory resq 0x100\n\n");

    fprintf(compiler->out, "section .text\n");
    fprintf(compiler->out, "_start:\n");
    INSTR("lea rbx, memory");

    INSTR("call .main\n");
    INSTR("mov rax, 0x3c");
    INSTR("xor rdi, rdi");
    INSTR("syscall");

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

void GenerateASM(const char *filename, tree *tree, Compiler *compiler) {
    if(!filename) {
        PRINT_("Wrong path to asm file");

        return;
    }

    compiler->GlobalNT = newList();

    GenerateGS(tree->root, compiler);
}