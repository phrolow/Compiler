#include "tokens.h"

#define CHECK(cond, err, msg)   \
    if(!(cond)) {               \
        fprintf(stderr, msg);   \
        return err;             \
    }

TOKEN_ERROR TokensVerify(token_stk_t *tokens) {
    CHECK(tokens, TOKENS_ARE_NULL, "Null tokens stack\n");

    CHECK(tokens->capacity != 0, ZERO_CAPACITY, "Token stack zero capacity\n");

    CHECK(tokens->size <= tokens->capacity, SIZE_GREATER_CTY, "Token stk size greater capacity\n");

    CHECK(tokens->tokens == NULL, TOKEN_IS_NULL, "Token is null\n");

    return TOKEN_IS_OK;
}