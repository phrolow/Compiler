#ifndef LANGUAGE_TOKENS_H
#define LANGUAGE_TOKENS_H

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define DEF_KEYW(name, num, sign) KEYW_##name = num,
#define DEF_OPER(name, num, sign) KEYW_##name = num,
#define DEF_HELP(name, num, sign) KEYW_##name = num,

const size_t WORD_MAX_LEN = 8;
const double TOKEN_POISON = 0xDEAD;
const size_t MIN_CAPACITY = 0x10;
const size_t CAPACITY_MULTIPLIER = 2;

enum TYPE {
    NOT_DEFINED = 0,
    NUM_TYPE,
    VAR_TYPE,
    KEYWORD_TYPE,
};

enum KEYW {
    #include "../keywords.h"
    #include "../operators.h"

#undef DEF_KEYW
#undef DEF_OPER
#undef DEF_HELP
};

union Value {
    enum KEYW keyword;
    double num;
    char name[WORD_MAX_LEN];
};

struct token_t {
    enum TYPE type;

    Value value;
};

struct token_stk_t {
    token_t *tokens;
    size_t   size;
    size_t   capacity;
};

token_t *NewToken(TYPE type, Value value);

int TokensAreEmpty(token_stk_t *tokens);

void TokenDtor(token_t *token);

int TokenCheck(token_t *token);

token_stk_t *NewTokenStk();

void TokensDtor(token_stk_t *tokens);

void TokensResize(token_stk_t *tokens);

void TokensPush(token_stk_t *tokens, const token_t *token);

token_t *TokensElem(token_stk_t *tokens, size_t index);

#include "tokens_debug.h"

#endif //LANGUAGE_TOKENS_H
