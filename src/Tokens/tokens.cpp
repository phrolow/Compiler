#include "tokens.h"

token_t *NewToken(TYPE type, Value value) {
    token_t *token = (token_t*) malloc(sizeof(token_t));

    if(!token) {
        PRINT_S("Bad alloc\n");

        abort();
    }

    token->type = type;
    token->value = value;

    return token;
}

void TokenDtor(token_t *token) {
    token->type = NOT_DEFINED;
    token->value.num = TOKEN_POISON;

    free(token);
}