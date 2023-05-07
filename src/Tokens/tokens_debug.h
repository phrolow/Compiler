#ifndef LANGUAGE_TOKENS_DEBUG_H
#define LANGUAGE_TOKENS_DEBUG_H

#include <stdio.h>
#include "../Debug/debug.h"

enum TOKEN_ERROR {   
        TOKEN_IS_OK                = 0,
        BAD_ALLOC                  = 0xBADA110C,
   
        ZERO_CAPACITY,
        TOKENS_ARE_NULL,
        SIZE_GREATER_CTY,
        TOKEN_IS_NULL
};

TOKEN_ERROR TokensVerify(token_stk_t *tokens);

#endif //LANGUAGE_TOKENS_H
