//=====================================================================================================================================

#include "tree.h"

//=====================================================================================================================================

FILE* LogFile = nullptr;

//=====================================================================================================================================

static void clearTree (node_t* root);

//=====================================================================================================================================

static void clearTree (node_t* root)
{
    ASSERT (root != nullptr, (void) -1);

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

int TreeCtor (tree_t* tree)
{
    ASSERT (tree != nullptr, -1);

    tree->root = nullptr;
    tree->size = 0;

    if ((LogFile = fopen ("log.html", "w")) == nullptr)
    {
        printf ("Error in function: %s. Error opening LogFile!\n", __func__);
        return errno;
    }

    return 0;
}

//=====================================================================================================================================

int TreeDtor (tree_t* tree)
{
    ASSERT (tree != nullptr, -1);

    if (tree->size != 0)
    {
        clearTree (tree->root);
    }

    tree->size = DeadSize;

    if ((fclose (LogFile)) != 0)
    {
        printf ("Error in function: %s. Error closing LogFile!\n", __func__);
        return errno;
    }

    return 0;
}

//=====================================================================================================================================

bool isTreeEmpty (tree_t* tree)
{
    if (tree == nullptr)
        return true;
    
    if (tree->root == nullptr && tree->size == 0)
        return true;
    
    return false;
}

//=====================================================================================================================================

bool isTreeDestructed (tree_t* tree)
{
    if (tree == nullptr && tree->size == DeadSize)
        return true;
    
    return false;
}

//=====================================================================================================================================

node_t* CreateNode (char* item)
{
    node_t* newNode = (node_t*) calloc (1, sizeof (node_t));
    if (newNode == nullptr)
    {
        printf ("Error in function: %s. newNode == nullptr!\n", __func__);
        return nullptr;
    }

	newNode->item = (char*) calloc (MaxSize, sizeof (char));
    strcpy (newNode->item, item);

    newNode->left  = nullptr;
    newNode->right = nullptr;

    return newNode;
}

//=====================================================================================================================================

bool NodeRemove (tree_t* tree, node_t* node)
{
    ASSERT (tree != nullptr, 1);
    ASSERT (node != nullptr, 1);

    if (node->left != nullptr && node->right != nullptr)
    {
        clearTree (node);
        tree->size = tree->size - 1;

        return false;
    }

    printf ("Error in function: %s. The node is not destroyed!\n", __func__);
    return true; 
}

//=====================================================================================================================================

node_t* InsertNode (tree_t* tree, node_t* node, char* item, InsMode insMode)
{
    ASSERT (tree != nullptr, nullptr);

    if (tree->size != 0 && node == nullptr && tree->root == nullptr)
    {
        printf ("Error in function: %s. The tree is incorrect!\n", __func__);
        return nullptr;
    }

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

void NodeDump (tree_t* tree, node_t* node, size_t* nodeCount, FILE* file)
{
	ASSERT (tree != nullptr, (void) -1);
    ASSERT (node != nullptr, (void) -1);
    ASSERT (file != nullptr, (void) -1);
	
	if (*nodeCount > tree->size) return;
	
	++(*nodeCount);

	fprintf (file, "\"%s\" [fillcolor=", node->item);

	if (node->left == nullptr && node->right == nullptr)   
        fprintf (file, "\"lightgreen\"];\n");
	else										                
        fprintf (file, "\"#7B68EE\"];\n");

	if (node->left != nullptr)
	{
		fprintf (file, "\"%s\" -> \"%s\" [fontname = \"Aqum\", color=\"red\", label=\"YES\"]\n", node->item, node->left->item);
	}

	if (node->right != nullptr)
	{
		fprintf (file, "\"%s\" -> \"%s\" [fontname = \"Aqum\", color=\"blue\", label=\"NO\"]\n", node->item, node->right->item);
	}

	if (node->left  != nullptr) 
		NodeDump (tree, node->left,  nodeCount, file);

    if (node->right != nullptr)
		NodeDump (tree, node->right, nodeCount, file);
}

//=====================================================================================================================================

void TreeDump (tree_t* tree)
{   
    ASSERT (tree != nullptr, (void) -1);

    if (tree->size == 0)
    {
        printf ("Error in function: %s. Attempt to call a dump for an empty tree!\n", __func__);
        return;
    }

    static unsigned dumpNum = 1;

    static char buf[512] = {0};
    snprintf (buf, sizeof (buf), "dump%u.dot", dumpNum);

    FILE* dotFile = fopen (buf, "w");

    fprintf (LogFile, "\n<hr>\n");
   
    fprintf (dotFile, "digraph G{\n");
    fprintf (dotFile, "node [color=black, shape=box, style=\"rounded, filled\", fontname = \"Aqum\"];\n");

    fprintf (dotFile, "size [fillcolor=\"#FFD700\", "
                     "label=\"SIZE = %llu\"];\n",
                      tree->size);
    
	size_t nodeCount = 0;

	NodeDump (tree, tree->root, &nodeCount, dotFile);

    fprintf (dotFile, "}\n");
    
    fclose (dotFile);

    snprintf (buf, sizeof (buf), "dot -Tsvg dump%u.dot -o dump%u.svg", dumpNum, dumpNum);
    system (buf);
	
	fprintf (LogFile, "<center""><h1"">TREE DUMP - INVOCATION %u</h1"">""</center"">\n", dumpNum);
    fprintf (LogFile, "<img width=\"1920\" height=\"1080\" src=\"dump%u.svg\"/>\n", dumpNum);

    dumpNum++;
}

//=====================================================================================================================================

