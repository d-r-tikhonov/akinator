//=====================================================================================================================================

#include "akinator.h"
#include "TXLib.h"

//=====================================================================================================================================

int main (void)
{
    tree_t* tree = (tree_t*) calloc (1, sizeof (tree_t));
    TreeCtor (tree);

    // InsertNode (tree, tree->root, "testL", LEFT);
    // InsertNode (tree, tree->root, "testR", RIGHT);

    TreeDump (tree);

    TreeDtor (tree);

    return 0;
}