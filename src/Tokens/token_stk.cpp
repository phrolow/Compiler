#include "tokens.h"

token_stk_t *NewTokenStk() {
    token_stk_t *tokens = (token_stk_t*) malloc(sizeof(token_stk_t));

    tokens->size     = 0;
    tokens->capacity = MIN_CAPACITY;

    tokens->tokens    = (token_t *) calloc(tokens->capacity, sizeof(token_t));

    return tokens;
}

int TokensAreEmpty(token_stk_t *tokens) {
    if (tokens->tokens   == nullptr   &&
        tokens->size     == 0         &&
        tokens->capacity == 0) return 1;

    return 0;
}

void TokensDtor(token_stk_t *tokens) {
    tokens->capacity = 0;
    tokens->size     = 0;

    free(tokens->tokens);
    tokens->tokens = nullptr;

    free(tokens);
}

void TokensPush(token_stk_t *tokens, const token_t *token) {
    assert(tokens && tokens->tokens && token);

    if (tokens->size == tokens->capacity)
    {
        TokensResize(tokens);

        assert(tokens->size < tokens->capacity);
    }

    memcpy(tokens->tokens + tokens->size++, token, sizeof(token_t));
}

token_t *TokensElem(token_stk_t *tokens, size_t index)
{
    assert(tokens && tokens->tokens);

    return tokens->tokens + index;;
}

void TokensResize  (token_stk_t *tokens)
{
    assert(CAPACITY_MULTIPLIER > 1);

    size_t new_capacity = MIN_CAPACITY;

    if (tokens->size <= tokens->capacity / (int) pow(CAPACITY_MULTIPLIER, 2))
    {
        new_capacity =  tokens->capacity / CAPACITY_MULTIPLIER;
    }
    else
    if (tokens->size == tokens->capacity)
    {
        new_capacity =  tokens->capacity * CAPACITY_MULTIPLIER;
    }
    else
        return;

#define max(a,b) ((a) > (b)) ? (a) : (b)
    new_capacity = max(new_capacity, MIN_CAPACITY);
#undef max

    if (new_capacity == tokens->capacity) return;

    token_t *token = (token_t *) realloc(tokens->tokens, sizeof(token_t) * new_capacity);

    tokens->tokens = token;

    if (new_capacity > tokens->capacity)
    {
        memset(tokens->tokens + tokens->capacity, 0, sizeof(token_t) * (new_capacity - tokens->capacity));
    }

    tokens->capacity = new_capacity;
}