//=====================================================================================================================================

#include "tree.h"

//=====================================================================================================================================

static void NodesDtor (node_t* root);

//=====================================================================================================================================

void TreeCtor (tree_t* tree)
{
    ASSERT (tree != nullptr);

    tree->root = nullptr;
    tree->size = 0;
}

void TreeDtor (tree_t* tree)
{
    ASSERT (tree != nullptr);

    tree->size = 0;

    NodesDtor (tree->root);
}

static void NodesDtor (node_t* root)
{
    ASSERT (root != nullptr);

    if (root->left != nullptr)
    {
        NodesDtor (root->left);
    }

    if (root->right != nullptr)
    {
        NodesDtor (root->right);
    }

    free (root);
}





