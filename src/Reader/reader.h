#ifndef LANGUAGE_READER_H
#define LANGUAGE_READER_H

#include "../Onegin/onegin.h"
#include "../Tree/tree.h"
#include "../Tokens/tokens.h"
#include "../TextToTokens/analyzer.h"
#include <ctype.h>

node *KeywordNode(KEYW keyw, side_t side);

token_t *require(token_stk_t *tokens, size_t *index, KEYW keyw);

node *getToken(token_stk_t *tokens, size_t *index, side_t side);

node *getCondition(token_stk_t *tokens, size_t *index, side_t side);

node *getLogOp(token_stk_t *tokens, size_t *index, side_t side);

node *getName(token_stk_t *tokens, size_t *index, side_t side);

node *getParams(token_stk_t *tokens, size_t *index, side_t side);

node *getFuncDef(token_stk_t *tokens, size_t *index, side_t side);

node* getP(token_stk_t *tokens, size_t *index, side_t side);

node* getN(token_stk_t *tokens, size_t *index, side_t side);

node* getV(token_stk_t *tokens, size_t *index, side_t side);

node* getPow(token_stk_t *tokens, size_t *index, side_t side);

node* getT(token_stk_t *tokens, size_t *index, side_t side);

node* getE(token_stk_t *tokens, size_t *index, side_t side);

node *getIf(token_stk_t *tokens, size_t *index, side_t side);

node *getWhile(token_stk_t *tokens, size_t *index, side_t side);

node *getReturn(token_stk_t *tokens, size_t *index, side_t side);

node *getPrint(token_stk_t *tokens, size_t *index, side_t side);

node *getScan(token_stk_t *tokens, size_t *index, side_t side);

node *getSqrt(token_stk_t *tokens, size_t *index, side_t side);

node *getStmt(token_stk_t *tokens, size_t *index, side_t side);

node *getStmts(token_stk_t *tokens, size_t *index, side_t side);

node *getGlobStmts(token_stk_t *tokens, size_t *index, side_t side);

node* getG(token_stk_t *tokens);

tree* ReadExpression(char *txt);

#endif //LANGUAGE_READER_H
