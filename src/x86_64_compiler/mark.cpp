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
    if(indexLabel(format, index, compiler) != POISON)
        return;

    name_t label = {};

    char name[WORD_MAX_LEN + 1] = {};

    sprintf(name, format, index);

    label.index = compiler->ip - compiler->out;
    sprintf(label.name, format, index);

    ListTailInsert(compiler->labels, label);
}

u_int64_t indexLabel(const char *format, size_t index, struct Compiler *compiler) {
    char name[WORD_MAX_LEN + 1] = {};

    sprintf(name, format, index);

    struct List *labels = compiler->labels;

    int num = ListIndexFirst(labels, name);

    if(num == -1)
        return POISON;

    name_t *label = labels->data + num;

    return label->index;
}

void putAddress64(const char *format, size_t index, struct Compiler *compiler) {
    u_int64_t label_offset = indexLabel(format, index, compiler);
    u_int64_t relative_address = label_offset - (compiler->ip - compiler->out);

    *((u_int64_t *) (compiler->ip)) = relative_address - sizeof(relative_address);
    compiler->ip += sizeof(u_int64_t);
}

void putAddress(const char *format, size_t index, struct Compiler *compiler) {
    u_int32_t label_offset = indexLabel(format, index, compiler);
    u_int32_t relative_address = label_offset - (compiler->ip - compiler->out);

    *((u_int32_t *) (compiler->ip)) = relative_address - sizeof(relative_address);
    compiler->ip += sizeof(u_int32_t);
}