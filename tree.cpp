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

    tree->size = DeadSize;

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

bool isTreeEmpty (tree_t* tree)
{
    if (tree == nullptr)
        return 1;
    
    if (tree->root == nullptr && tree->size == 0)
        return 1;
    
    return 0;
}

bool isTreeDestructed (tree_t* tree)
{
    if (tree == nullptr && tree->size == DeadSize)
        return 1;
    
    return 0;
}

node_t* CreateNode (const char* item)
{
    node_t* newNode = (node_t*) calloc (1, sizeof(node_t));
    ASSERT (newNode != nullptr);

    newNode->item = item;

    newNode->left  = nullptr;
    newNode->right = nullptr;

    return newNode;
}

void NodeRemove (tree_t* tree, node_t* node)
{
    ASSERT (tree != nullptr);
    ASSERT (node != nullptr);

    if (node->left != nullptr && node->right != nullptr)
    {
        NodesDtor ()
    }
}

node_t* InsertNode (tree_t* tree, node_t* node, const char* item)
{
    ASSERT (tree != nullptr);
    ASSERT (node != nullptr && tree->root != nullptr);


}





