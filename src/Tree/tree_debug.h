#ifndef LANGUAGE_TREE_DEBUG_H
#define LANGUAGE_TREE_DEBUG_H

#include "../Debug/debug.h"

#define PRINT_NODE(node) printf("%d %d\n", (node)->type, (node)->value)

const char* const    PNGPATH = "tree.png";
const char* const    HTMLPATH = "log.html";
const char* const    DOTPATH = "tree";

const size_t COLORLEN = 8;

const char      OP_COLOR        [COLORLEN] = "#F4CF72",
                CONST_COLOR     [COLORLEN] = "#71DCC1",
                VAR_COLOR       [COLORLEN] = "#F49872",
                MCONST_COLOR    [COLORLEN] = "#B0C4DE",
                ERR_COLOR       [COLORLEN] = "#808080",
                RIGHT_COLOR     [COLORLEN] = "#7173DC",
                LEFT_COLOR      [COLORLEN] = "#CD5C5C";

enum TREE_ERROR {
    TREE_OK = 0,
    NULL_NODE,
    INVALID_LINK,
    UNDEFINED_TYPE,
    NULL_NAME,
};

TREE_ERROR NodeVerify(const node *nod);

TREE_ERROR TreeVerify(const node *nod);

void TreeDump(tree *tree);

#endif //LANGUAGE_DEBUG_H
