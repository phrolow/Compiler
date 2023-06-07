#include "reader.h"

#define KILL {                                                      \
    printf("WRONG TOKEN #%lu\n", *index);                       \
                                                                    \
    assert(0);                                                      \
}

#define NEWNODE(name, token, side)          \
    node *name = (node*) malloc(sizeof(node));  \
    NodeCtor(name, NULL, token, side);

#define DEF_KEYW(name, num, sign)          \
            case KEYW_##name:                  \
                printf(#sign);  \
                break;

#define DEF_OPER(name, num, sign)                   \
            case KEYW_##name:                      \
                printf(#sign);     \
                break;

#define DEF_HELP(name, num, sign)                   \
            case KEYW_##name:                      \
                printf("%c\n", sign);      \
                break;

node *KeywordNode(KEYW keyw, side_t side) {
    node *keywordnode = (node *) malloc(sizeof(node));

    token_t *token = NewToken(KEYWORD_TYPE, {.keyword = keyw});

    NodeCtor(keywordnode, NULL, token, side);

    return keywordnode;
}

token_t *require(token_stk_t *tokens, size_t *index, KEYW keyw) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != KEYWORD_TYPE || token->value.keyword != keyw) {
        printf("INVALID SYNTAX: at %lu required ", *index);

        switch (keyw) {
            #include "../keywords.h"
            #include "../operators.h"
            default:
                printf("... what?)");
                break;
        }

        #undef DEF_KEYW
        #undef DEF_OPER
        #undef DEF_HELP

        assert(0);
    }

    (*index)++;

    return token;
}

node *getLogOp(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    assert(token->type == KEYWORD_TYPE);
    assert(token->value.keyword >= KEYW_EQUAL || token->value.keyword <= KEYW_OR);

    return getToken(tokens, index, side);
}

node *getCondition(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    require(tokens, index, KEYW_OPRND);

    node *left = getE(tokens, index, LEFT);
    node *log_op = getLogOp(tokens, index, side);
    node *right = getE(tokens, index, RIGHT);

    NodeConnect(log_op, left);
    NodeConnect(log_op, right);

    require(tokens, index, KEYW_CLRND);

    return log_op; //неправильно как-то....
}

node *getName(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != VAR_TYPE) KILL;

    return getToken(tokens, index, side);
}

node *getParams(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    require(tokens, index, KEYW_OPRND);

    node *expression = getE(tokens, index, LEFT);

    require(tokens, index, KEYW_CLRND);

    node *param = KeywordNode(KEYW_PARAM, side);
    NodeConnect(param, expression);

    return param;
}

node *getFuncDef(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    int is_main = 0;

    if(token->value.keyword != KEYW_MAIN && token->value.keyword != KEYW_FUNC) KILL;

    if(token->value.keyword == KEYW_MAIN) {
        is_main = 1;
    }

    (*index)++;

    node *def = KeywordNode(KEYW_DEFINE, side);
    node *func = KeywordNode(KEYW_FUNC, LEFT);
    node *name = NULL;

    if(is_main) {
        name = KeywordNode(KEYW_MAIN, LEFT);
    }
    else {
        name = getName(tokens, index, LEFT);
    }

    if(!is_main && (TokensElem(tokens, *index)->value.keyword == KEYW_OPRND)) {
        node *params = getParams(tokens, index, RIGHT);

        NodeConnect(func, params);
    }

    node *stmts = getStmts(tokens, index, RIGHT);

    NodeConnect(func, name);
    NodeConnect(def, func);
    NodeConnect(def, stmts);

    return def;
}

node *getToken(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    token_t *copy = (token_t*) malloc(sizeof(token_t));

    memcpy(copy, token, sizeof(token_t));

    NEWNODE(newnode, copy, side);

    (*index)++;

    return newnode;
}

node* getN(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != NUM_TYPE) KILL

    return getToken(tokens, index, side);
}

node* getV(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if (token->type == VAR_TYPE)
        return getToken(tokens, index, side);

    return getN(tokens, index, side);
}

node *getF(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type == VAR_TYPE) {
        (*index)++;

        token = TokensElem(tokens, *index);

        if(!(token->type == KEYWORD_TYPE && token->value.keyword == KEYW_OPRND)) {
            (*index)--;
            return getV(tokens, index, side);
        }

        (*index)--;

        node *name = getName(tokens, index, LEFT);
        node *params = getParams(tokens, index, RIGHT);
        node *call = KeywordNode(KEYW_CALL, side);

        NodeConnect(call, name);
        NodeConnect(call, params);

        return call;
    }

    if(token->type == KEYWORD_TYPE && token->value.keyword == KEYW_SQRT) {
        node *sqrt = getToken(tokens, index, side);

        require(tokens, index, KEYW_OPRND);

        node *arg = getE(tokens, index, LEFT);

        require(tokens, index, KEYW_CLRND);

        NodeConnect(sqrt, arg);

        return sqrt;
    }

    return getN(tokens, index, side);
}

node* getPow(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *left = getP(tokens, index, side);

    token_t *token = TokensElem(tokens, *index);

    while(token->value.keyword == KEYW_POW) {
        node *nod = getToken(tokens, index, side);

        left->side = LEFT;

        node *right = getP(tokens, index, RIGHT);

        NodeConnect(nod, left);
        NodeConnect(nod, right);

        left = nod;
        token = TokensElem(tokens, *index);
    }

    return left;
}

node* getT(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *right = getPow(tokens, index, side);

    token_t *token = TokensElem(tokens, *index);

    while(token->value.keyword == KEYW_MUL || token->value.keyword == KEYW_DIV) {
        node *nod = getToken(tokens, index, side);

        right->side = RIGHT;

        node *left = getPow(tokens, index, LEFT);

        NodeConnect(nod, right);
        NodeConnect(nod, left);

        right = nod;
        token = TokensElem(tokens, *index);
    }

    return right;
}

node* getE(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *right = getT(tokens, index, side);

    token_t *token = TokensElem(tokens, *index);

    // while(token->value.keyword == KEYW_ADD || token->value.keyword == KEYW_SUB) {
    //     node *nod = getToken(tokens, index, side);

    //     left->side = LEFT;

    //     node *right = getT(tokens, index, RIGHT);

    //     NodeConnect(nod, left);
    //     NodeConnect(nod, right);

    //     left = nod;
    //     token = TokensElem(tokens, *index);
    // }

    while(token->value.keyword == KEYW_ADD || token->value.keyword == KEYW_SUB) {
        node *nod = getToken(tokens, index, side);

        right->side = RIGHT;

        node *left = getT(tokens, index, LEFT);

        NodeConnect(nod, right);
        NodeConnect(nod, left);

        right = nod;
        token = TokensElem(tokens, *index);
    }

    return right;
}

node *getIf(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *if_node = getToken(tokens, index, side);
    node *condition = getCondition(tokens, index, LEFT);

    NodeConnect(if_node, condition);

    node *stmts = getStmts(tokens, index, RIGHT);
    token_t *token = TokensElem(tokens, *index);

    if(!(token->type == KEYWORD_TYPE && token->value.keyword == KEYW_ELSE)) {
        NodeConnect(if_node, stmts);

        return if_node;
    }

    node *else_node = getToken(tokens, index, RIGHT);

    NodeConnect(if_node, else_node);

    stmts->side = LEFT;

    node *else_stmts = getStmts(tokens, index, RIGHT);

    NodeConnect(else_node, stmts);
    NodeConnect(else_node, else_stmts);

    return if_node;
}

node *getWhile(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *while_node = getToken(tokens, index, side);
    node *condition = getCondition(tokens, index, LEFT);
    node *stmts = getStmts(tokens, index, RIGHT);

    NodeConnect(while_node, condition);
    NodeConnect(while_node, stmts);

    return while_node;
}

node *getReturn(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *return_node = getToken(tokens, index, side);

    if(TokensElem(tokens, *index)->value.keyword != KEYW_DOTPOT) {
        node *expression = getE(tokens, index, LEFT);

        NodeConnect(return_node, expression);
    }

    require(tokens, index, KEYW_DOTPOT);

    return return_node;
}

node *getSqrt(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *sqrt_node = getToken(tokens, index, side);

    node *expression = getE(tokens, index, LEFT);

    NodeConnect(sqrt_node, expression);

    require(tokens, index, KEYW_DOTPOT);

    return sqrt_node;
}

node *getPrint(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *print_node = getToken(tokens, index, side);

    node *expression = getE(tokens, index, LEFT);

    NodeConnect(print_node, expression);

    require(tokens, index, KEYW_DOTPOT);

    return print_node;
}

node *getScan(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    node *scan_node = getToken(tokens, index, side);

    node *expression = getV(tokens, index, LEFT);

    NodeConnect(scan_node, expression);

    require(tokens, index, KEYW_DOTPOT);

    return scan_node;
}

node *getStmt(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    node *stmt_child = NULL;

    if(token->type == VAR_TYPE) {
        token_t *first = token;
        (*index)++;
        token = TokensElem(tokens, *index);

        if(token->type == KEYWORD_TYPE && token->value.keyword == KEYW_ASSIGN) {
            (*index)--;

            node *var = getV(tokens, index, LEFT);
            node *assign = getToken(tokens, index, side);
            node *expression = getE(tokens, index, RIGHT);

            require(tokens, index, KEYW_DOTPOT);

            NodeConnect(assign,var);
            NodeConnect(assign,expression);

            stmt_child = assign;

            return stmt_child;
        }
        else {
            (*index)--;

            token = first;
        }
    }
    else if(token->type == KEYWORD_TYPE) {
        switch (token->value.keyword) {
            case KEYW_IF:
                return getIf(tokens, index, side);
            case KEYW_WHILE:
                return getWhile(tokens, index, side);
            case KEYW_RETURN:
                return getReturn(tokens, index, side);
            case KEYW_PRINT:
                return getPrint(tokens, index, side);
            case KEYW_SCAN:
                return getScan(tokens, index, side);
            case KEYW_SQRT:
                return getSqrt(tokens, index, side);
            default:
                printf("Чё бля...");
                assert(0);
        }
    }

    stmt_child = getE(tokens, index, side);

    require(tokens, index, KEYW_DOTPOT);

    return stmt_child;
}

node* getP(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    if(token->type != KEYWORD_TYPE || token->value.keyword != KEYW_OPRND)
        return getF(tokens, index, side);

    (*index)++;

    node *nod = getE(tokens, index, side);

    require(tokens, index, KEYW_CLRND);

    return nod;
}

node *getStmts(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = TokensElem(tokens, *index);

    require(tokens, index, KEYW_OPFIG);

    token = TokensElem(tokens, *index);

    assert(!(token->type == KEYWORD_TYPE && token->value.keyword == KEYW_CLFIG));

    node    *glob_stmts = NULL,
            *stmt = NULL,
            *current = NULL;

    while (*index < tokens->size) {
        token = TokensElem(tokens, *index);

        if(token->type == KEYWORD_TYPE && token->value.keyword == KEYW_CLFIG) {
            (*index)++;
            return glob_stmts;
        }

        stmt = KeywordNode(KEYW_STMT, side);
        current = getStmt(tokens, index, LEFT);

        NodeConnect(stmt, current);

        if(glob_stmts) {
            glob_stmts->side = RIGHT;

            NodeConnect(stmt, glob_stmts);
        }

        glob_stmts = stmt;
    }

    return glob_stmts;
}

node *getGlobStmts(token_stk_t *tokens, size_t *index, side_t side) {
    assert(tokens && tokens->tokens && index);

    token_t *token = NULL;

    node    *glob_stmts = NULL,
            *current = NULL,
            *stmt = NULL;

    while (*index < tokens->size - 1) {
        token = TokensElem(tokens, *index);

        if(token->type == KEYWORD_TYPE && token->value.keyword == EOF)
            return glob_stmts;

        stmt = KeywordNode(KEYW_STMT, RIGHT);

        if(token->type == VAR_TYPE) {
            node *var = getName(tokens, index, LEFT);
            node *assign = getToken(tokens, index, LEFT);
            node *expression = getE(tokens, index, RIGHT);

            require(tokens, index, KEYW_DOTPOT);

            NodeConnect(assign, var);
            NodeConnect(assign, expression);

            current = assign;
        }

        else if(token->type == KEYWORD_TYPE) {
            current = getFuncDef(tokens, index, LEFT);
        }
        else KILL;

        if(current)
            NodeConnect(stmt, current);

        if(glob_stmts) {
            glob_stmts->side = RIGHT;

            NodeConnect(stmt, glob_stmts);
        }

        glob_stmts = stmt;
    }

    return glob_stmts;
}

node* getG(token_stk_t *tokens) {
    assert(tokens);

    size_t index = 0;

    node *root = getGlobStmts(tokens, &index, ROOT);
    assert(root);

    if(tokens->tokens[index].value.keyword != KEYW_EOF) {
        printf("WRONG TOKEN No. %lu", index);

        assert(0);
    }

    return root;
}

tree* ReadExpression(char *txt) {
    tree *expression = (tree*) malloc(sizeof(tree));

    token_stk_t *tokens = NewTokenStk();

    GetTokens(txt, tokens);

    TreeCtor(expression, getG(tokens));

    TokensDtor(tokens);

    return expression;
}