#include "compiler.h"

void GenerateMark(struct Node *mark, struct Compiler *compiler) {
    if (mark->val->type == VAR_TYPE || NODE_KEYW(mark, KEYW_MAIN)) {
        if (mark->val->type == VAR_TYPE) {
            generateLabel(mark->val->value.name, POISON, compiler);

            return;
        }
        else
        if (KEYW(mark) == KEYW_MAIN) {
            generateLabel("main", POISON, compiler);

            compiler->main = compiler->ip;

            return;
        }
    }

    PRINT_("Non-name type of node");
}

void generateLabel(const char *format, size_t index, struct Compiler *compiler) {
    name_t label = {};

    char name[WORD_MAX_LEN + 1] = {};

    sprintf(name, format, index);

    label.index = compiler->ip - compiler->out;
    sprintf(label.name, format, index);

    ListTailInsert(compiler->labels, label);
}

size_t indexLabel(const char *format, size_t index, struct Compiler *compiler) {
    char name[WORD_MAX_LEN + 1] = {};

    sprintf(name, format, index);

    struct List *labels = compiler->labels;

    int next = 0;

    while(labels->next[next]) {
        next = labels->next[next];

        if(!strcmp(name, labels->data[next].name)) {
            return labels->data[next].index;
        }
    }

    return POISON;
}