#include "tree.h"

#define CHECK(cond, err, msg)   \
    if(!(cond)) {               \
        fprintf(stderr, msg);   \
        return err;             \
    }

static void PrintNode(const node *node, const size_t *nNode, const char color[COLORLEN], FILE *fp) {
    assert(node && nNode && color && fp);

    fprintf(fp, "\tnode%lu [fillcolor=\"%s\", ", *nNode, color);

    switch (node->val->type) {
        case NUM_TYPE:
            fprintf(fp, "label=\"%lg\"", node->val->value.num);
            break;
        case VAR_TYPE:
            fprintf(fp, "label=\"%s\"", node->val->value.name);
            break;
        case KEYWORD_TYPE:
            #define DEF_KEYW(name, num, sign)                                       \
                    case KEYW_##name:                                               \
                        fprintf(fp, "label=\"%s\"", #name);     \
                        break;

            #define DEF_OPER(name, num, sign)                                       \
                    case KEYW_##name:                                               \
                        fprintf(fp, "label=\"%s\"", #name);     \
                        break;

            #define DEF_HELP(name, num, sign)

            switch (node->val->value.keyword) {
                #include "../keywords.h"

                #include "../operators.h"

                #undef DEF_KEYW
                #undef DEF_OPER
                #undef DEF_HELP
                default:
                    fprintf(fp, "label=\"UNDEF KEYW\"");
                    break;
            }
            break;
        default:
            fprintf(fp, "label=\"%d %lf\"", node->val->type, node->val->value.num);
            break;
    }

    fprintf(fp, "];\n");

    return;
}

static void PrintEdge(const size_t from, const size_t to, const char color[COLORLEN], FILE *fp) {
    assert(color && fp);

    fprintf(fp, "\tedge [color=\"%s\"]; node%lu -> node%lu;\n", color, from, to);

    return;
}

static void nodedump(const node *node, size_t *nNode, FILE *fp) {
    assert(fp && nNode);

    switch (node->val->type) {
        case KEYWORD_TYPE:
            PrintNode(node, nNode, OP_COLOR, fp);
            break;
        case NUM_TYPE:
            PrintNode(node, nNode, CONST_COLOR, fp);
            break;
        case VAR_TYPE:
            PrintNode(node, nNode, VAR_COLOR, fp);
            break;
        default:
            PrintNode(node, nNode, ERR_COLOR, fp);
            break;
    }

    size_t current_number_of_node = *nNode;

    if (node->children[LEFT]) {
        PrintEdge(current_number_of_node, ++(*nNode), LEFT_COLOR, fp);
        nodedump(node->children[LEFT], nNode, fp);
    }

    if (node->children[RIGHT]) {
        PrintEdge(current_number_of_node, ++(*nNode), RIGHT_COLOR, fp);
        nodedump(node->children[RIGHT], nNode, fp);
    }
}

void TreeDump(tree *tree) {
    size_t nNode = 0;
    FILE *fp = fopen(DOTPATH, "w");

    fputs("digraph structs {\n", fp);
    fputs("\tnode [color=black, shape=box, style=\"filled\"];\n", fp);

    nodedump(tree->root, &nNode, fp);

    fputs("}\n", fp);

    fclose(fp);

    system("dot tree -T png -o tree.png");
}

static TREE_ERROR nodeverify(const node *nod) {
    // CHECK(nod != nullptr, NULL_NODE, "Null node\n")

    // CHECK(nod->parent == NULL || nod == nod->parent->children[nod->side], INVALID_LINK, "Invalid link")

    // CHECK(nod->val->type != NOT_DEFINED, UNDEFINED_TYPE, "Undefined type\n")

    // CHECK(nod->val->type != VAR_TYPE || nod->val->value.name, NULL_NAME, "Null name\n")

    // if(nod->children[0]) {
    //    TREE_ERROR left_err = nodeverify(nod->children[LEFT]);
    //    TREE_ERROR right_err = nodeverify(nod->children[RIGHT]);

    //    if(left_err)
    //        return left_err;

    //    if(right_err)
    //        return right_err;
    // }

    return TREE_OK;
}

TREE_ERROR NodeVerify(const node *nod) {
    return nodeverify(nod);
}

TREE_ERROR TreeVerify(const tree *tree) {
    return nodeverify(tree->root);
}