#include "compiler.h"


int searchInNametable(struct Node *node, struct List *NT) {
    TREE_ERROR node_err = NodeVerify(node);

    if(node_err) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    size_t index = ListIndexFirst(NT, node->val->value.name);

    if (index != -1) {
        return 1;
    }

    return 0;
}

void pushInNametable(struct Node *node, struct List *NT) {
    TREE_ERROR node_err = NodeVerify(node);

    if(NodeVerify(node)) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    name_t name  = {};

    memcpy(name.name, node->val->value.name, sizeof(node->val->value.name));

    ListTailInsert(NT, name);
}

int indexNametable(struct Node *node, struct List *NT) {
    TREE_ERROR node_err = NodeVerify(node);

    if(NodeVerify(node)) {
        PRINT_("Something wrong with node! Code of error:");

        PRINT_D(node_err);
    }

    if(!NT) {
        PRINT_("Null name table!");
    }

    if (node->val->type != VAR_TYPE)
    {
        PRINT_("Got non-name type");

        return -1;
    }

    size_t number = ListIndexFirst(NT, node->val->value.name);

    if (number == 0) {
        PRINT_LINE;

        fprintf(stderr, "Variable named \"%s\" doesn't exist", node->val->value.name);
    }

    return number;
}