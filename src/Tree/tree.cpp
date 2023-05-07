#include "tree.h"

int NodeCtor(node *new_node, node *parent, value_t val, side_t side) {
    new_node->parent = parent;
    new_node->children[LEFT] = NULL;
    new_node->children[RIGHT] = NULL;
    new_node->val = val;
    new_node->side = side;

    return 0;
}

void NodeDtor(node *nod) {
    if(nod->parent)
        nod->parent->children[nod->side] = NULL;

    nod->parent = NULL;
    nod->children[0] = NULL;
    nod->children[1] = NULL;
    free(nod->val);
    nod->val = NULL;

    free(nod);
}

static size_t numchildren(node *nod) {
    int num = 0;

    if(nod->children[0])
        num += 1 + numchildren(nod->children[0]);

    if(nod->children[1])
        num += 1 + numchildren(nod->children[1]);

    return num;
}

static node *nodcpy(const node *orig, node *parent) {
    NodeVerify(orig);
    NodeVerify(parent);

    node *nod = (node*) malloc(sizeof(node));
    NodeCtor(nod, parent, orig->val, orig->side);

    if(orig->children[LEFT])
        nod->children[LEFT] = nodcpy(orig->children[LEFT], nod);
    if(orig->children[RIGHT])
        nod->children[RIGHT] = nodcpy(orig->children[RIGHT], nod);

    NodeVerify(nod);

    return nod;
}

node *NodeCopy(node *original) {
    return nodcpy(original, NULL);
}

void NodeConnect(node *parent, node *child) {
    child->parent = parent;

    if(parent)
        parent->children[child->side] = child;
}

void TreeCtor(tree *tree, node *root) {
    tree->root = root;
}

static void nodedtor_(node *node) {
    if(node->children[0]) {
        nodedtor_(node->children[0]);
    }

    if(node->children[1]) {
        nodedtor_(node->children[1]);
    }

    NodeDtor(node);
}

void SubTreeDtor(node *node) {
    nodedtor_(node);
}

void TreeDtor(tree *tree) {
    SubTreeDtor(tree->root);

    free(tree);
}