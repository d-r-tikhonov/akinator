//=====================================================================================================================================

#include "tree.h"

//=====================================================================================================================================

static void clearTree (node_t* root);

//=====================================================================================================================================

static void clearTree (node_t* root)
{
    ASSERT (root != nullptr);

    if (root->left != nullptr)
    {
        clearTree (root->left);
    }

    if (root->right != nullptr)
    {
        clearTree (root->right);
    }

    free (root);
}

//=====================================================================================================================================

void TreeCtor (tree_t* tree)
{
    ASSERT (tree != nullptr);

    tree->root = nullptr;
    tree->size = 0;
}

//=====================================================================================================================================

void TreeDtor (tree_t* tree)
{
    ASSERT (tree != nullptr);

    tree->size = DeadSize;

    clearTree (tree->root);
}

//=====================================================================================================================================

bool isTreeEmpty (tree_t* tree)
{
    if (tree == nullptr)
        return 1;
    
    if (tree->root == nullptr && tree->size == 0)
        return 1;
    
    return 0;
}

//=====================================================================================================================================

bool isTreeDestructed (tree_t* tree)
{
    if (tree == nullptr && tree->size == DeadSize)
        return 1;
    
    return 0;
}

//=====================================================================================================================================

node_t* CreateNode (char* item)
{
    node_t* newNode = (node_t*) calloc (1, sizeof(node_t));
    ASSERT (newNode != nullptr);

    newNode->item = item;

    newNode->left  = nullptr;
    newNode->right = nullptr;

    return newNode;
}

//=====================================================================================================================================

bool NodeRemove (tree_t* tree, node_t* node)
{
    ASSERT (tree != nullptr);
    ASSERT (node != nullptr);

    if (node->left != nullptr && node->right != nullptr)
    {
        clearTree (node);
        tree->size = tree->size - 1;

        return 0;
    }

    printf ("Error in function: %s. The node is not destroyed!\n", __func__);
    return 1; 
}

//=====================================================================================================================================

node_t* InsertNode (tree_t* tree, node_t* node, char* item, InsMode insMode)
{
    ASSERT (tree != nullptr);
    ASSERT (node != nullptr && tree->root != nullptr);

    node_t* newNode = CreateNode (item);

    if (tree->root == node && tree->size == 0)
    {
        tree->root = newNode;
        tree->size = 1;

        return tree->root;
    }

    switch (insMode)
    {
    case LEFT:
        node->left = newNode;
        break;

    case RIGHT:
        node->right = newNode;
        break;
    
    default:
        printf ("Error in function: %s. Unexpected insert mode!\n", __func__);
        break;
    }

    tree->size = tree->size + 1;

    return newNode;
}

//=====================================================================================================================================
