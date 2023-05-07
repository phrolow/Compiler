#include "analyzer.h"

void skip(char **ptr) {
    while(isspace(**ptr))
        (*ptr)++;
}

void getKeyword (char *word, int *keyword_code, size_t *length) {
    assert(keyword_code && word && length);

#define DEF_KEYW(DEF, CODE, WORD)          \
    if (streq(word, #WORD)) {                   \
        *keyword_code = CODE;                   \
        *length = strlen(#WORD);                \
    } else
#include "../keywords.h"
/*  else */ {
        *keyword_code = 0;
        *length = 0;
    }
#undef DEF_KEYW

    return;
}

void getWord(char **ptr, char *word, size_t *length) {
    assert(ptr && *ptr && word && length);

    skip(ptr);
    char *str = (*ptr);

    size_t size = 0;

    do {
        word[size++] = *(str++);
    } while   ((*str >= 'a' && *str <= 'z'  ||
                *str >= 'A' && *str <= 'Z'  ||
                *str >= '0' && *str <= '9'  ||
                *str == '_' || *str == '$') && size != WORD_MAX_LEN);

    if (size == WORD_MAX_LEN && isspace((*ptr)[size]) != 0) {
        printf("ANALYZER: max word len reached at хуй пойми где\n");
        return;
    }

    *length = size;
}

void getToken(char **ptr, token_stk_t *tokens) {
    assert(ptr && *ptr && tokens && tokens->tokens);

    skip(ptr);

    token_t token = {};

    assert(**ptr != '\0');

    if (isalpha(**ptr) || **ptr == '$')
    {
        char *word = (char *) calloc(WORD_MAX_LEN + 1, sizeof(char));

        size_t word_length = 0;

        getWord(ptr, word, &word_length);

        size_t keyw_length = 0;

        int keyword_code = 0;
        getKeyword(word, &keyword_code, &keyw_length);

        if (keyword_code)
        {
            token.type = KEYWORD_TYPE;
            token.value.keyword = (KEYW) keyword_code;
            (*ptr) += keyw_length;
        }
        else
        {
            token.type = VAR_TYPE;
            memcpy(token.value.name, word, WORD_MAX_LEN * sizeof(char));
            (*ptr) += word_length;
        }

        free(word);
    }
    else
    if (isalnum(**ptr))
    {
        token.type = NUM_TYPE;

        double num = NAN;
        int length = 0;

        sscanf(*ptr, "%lf%n", &num, &length);
        (*ptr) += length;

        token.value.num = num;
    }
    else
    {
        token.type = KEYWORD_TYPE;

        // operator
#define DEF_OPER(DEF, CODE, SIGN)                \
            if (streq(*ptr, #SIGN))              \
            {                                    \
                sign = CODE;                     \
                *ptr += strlen(#SIGN);           \
            }                                    \
            else

#define DEF_HELP(DEF, CODE, HELP)                        \
            if (**ptr == HELP)                           \
            {                                            \
                sign = CODE;                             \
                *ptr += 1;                               \
            }                                            \
            else

        int sign = 0;

#include "../operators.h"
        /* else */
        {
            sign = 0;
        }

        if (sign == 0)
        {
            fprintf(stderr, "ERROR: %s", *ptr);
        }

#undef DEF_OPER
#undef DEF_HELP

        token.value.keyword = (KEYW) sign;
    }

    TokensPush(tokens, &token);
}

void GetTokens(char *txt, token_stk_t *tokens)
{
    assert(txt && tokens);

    char *ptr = txt;

    do
    {
        getToken(&ptr, tokens);
    }
    while (*ptr != '\0');

    token_t token = {};
    token.type = KEYWORD_TYPE;
    token.value.keyword = KEYW_EOF;
    TokensPush(tokens, &token);
}