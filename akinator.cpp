//=====================================================================================================================================

#include "akinator.h"

//=====================================================================================================================================

int main (void)
{
    tree_t* treeDatabase = DataDownload ();
    TreeDump (treeDatabase);

    int mode = 0;

    while ((mode = SelectMode ()) != 'q')
    {
        switch (mode)
        {
        case 'g':
            AkinatorGuess (treeDatabase);
            break;
        
        default:
            printf ("Error in function: %s. Error incorrect mode!\n", __func__);
            break;
        }
    }

    DatabaseDtor (treeDatabase);

    return 0;
}

//=====================================================================================================================================

int SelectMode (void)
{
    int mode = 0;

    printf ("Select the program usage mode:\n"
            "Guess the character - [g]\n"
            "Quit the program - [q]\n");
    
    while ((mode = getchar ()) != EOF)
    {
        while (getchar () != '\n')
        {
            continue;
        }

        if (strchr ("gq", mode) == 0)
        {
            printf ("You have entered an incorrect character! Enter the character correctly!\n");
            continue;
        }
        else
        {
            break;
        }
    }

    if (mode == EOF)
    {
        printf ("Error in function: %s. Error mode == EOF!\n", __func__);
        mode = 'q';
    }

    return mode;
}

//=====================================================================================================================================

tree_t* DataDownload (void)
{
    FILE* dataFile = fopen ("akinator.database", "r");
    ASSERT (dataFile != nullptr);

    printf ("HERE1\n");

    tree_t* tree = (tree_t*) calloc (1, sizeof (tree_t));
    TreeCtor (tree);

    printf ("HERE2\n");

    Text dataBase = {};
    TextCtor (&dataBase, dataFile);

    if (fclose (dataFile) != 0)
    {
        printf ("Error in function: %s. Error closing the dataFile!\n", __func__);
    }

    printf ("HERE3\n");

    if (ReadTree (&dataBase, tree) != 0)
    {
        printf ("Error in function: %s. Error reading the tree!\n", __func__);
        return nullptr;
    }

    printf ("HERE4\n");

    TextDtor (&dataBase);

    return tree;
}

//=====================================================================================================================================

int CreateNodeItem (char* item, const char* data)
{
    if (item == nullptr || data == nullptr)
    {
        printf ("Error in function: %s. Null pointers to variables!\n", __func__);
        return -1;
    }

    const char* end = strchr (data, '"');
    if (end == nullptr)
    {
        printf ("Error in function: %s. Error in the database!\n", __func__);
        return -1; 
    }

    strncpy (item, data, end-data);

    return 0;
}

//=====================================================================================================================================

int ReadTree (Text* database, tree_t* tree)
{
    ASSERT (database != nullptr);
    ASSERT (tree != nullptr);

    printf ("HERE11\n");

    stack_t stk = {};
    StackCtor (&stk);

    const char* open = strchr (database->lines[0].lineStart, '{');
    if (open == nullptr)
    {
        printf ("Error in function: %s. Error in the database!\n", __func__);
        return -1;
    }

    char item[MaxSize] = "";

    if (CreateNodeItem (item, open + 3) != 0)
    {
        printf ("Error in function: %s. Error in the database!\n", __func__);
        return -1;
    }

    InsMode currentMode = LEFT;
    node_t* currentNode = InsertNode (tree, tree->root, item, currentMode);

    printf ("HERE12\n");

    if (strchr (open, '}') != nullptr)
    {
        StackDtor (&stk);
        return 0;
    }

    for (size_t index = 1; index < database->nLines; index++)
    {
        char varItem[MaxSize] = "";

        open = strchr (database->lines[index].lineStart, '{');
        if (open != nullptr)
        {
            StackPush (&stk, currentNode);

            if (CreateNodeItem (varItem, open + 3) != 0)
            {       
                printf ("Error in function: %s. Error in the database!\n", __func__);
                return -1;
            }

            currentNode = InsertNode (tree, currentNode, varItem, currentMode);

            if (strchr (open, '}') != nullptr)
            {
                currentNode = StackPop (&stk);

                if (currentMode == LEFT)
                {
                    currentMode = RIGHT;
                }
            }
            else
            {
                currentMode = LEFT;
            }
        }
        else if (strchr (database->lines[index].lineStart, '}'))
        {
            currentNode = StackPop (&stk);
            currentMode = RIGHT;
        }
        else
        {
            printf ("Error in function: %s. Error in the database!\n", __func__);
            return -1;
        }
    }

    StackDtor (&stk);

    return 0;
}

//=====================================================================================================================================

int AkinatorGuess (tree_t* tree)
{
    if (tree == nullptr)
    {
        printf ("Error in function: %s. The pointer to the tree database is nullptr!\n", __func__);
        return -1;
    }

    stack_t stk = {};
    StackCtor (&stk);

    node_t* currentNode = tree->root;
    int userAns = 0;

    while (currentNode != nullptr)
    {
        StackPush (&stk, currentNode);

        printf ("Question: %s. Enter [y] if the answer is YES or [n] if the answer is NO: ", currentNode->item);

        while ((userAns = getchar ()) != EOF)
        {
            if (strchr ("yn", userAns) == nullptr)
            {
                printf ("Enter the symbols correctly!");
            }
            else
            {
                break;
            }
        }

        if (userAns == 'y')
        {
            currentNode = currentNode->left;
        }
        else if (userAns == 'n')
        {
            currentNode = currentNode->right;
        }
        else
        {
            printf ("Error in function: %s. Unknown user answer!\n", __func__);
            return -1;
        }
    }

    currentNode = StackPop (&stk);

    if (userAns == 'y')
    {
        printf ("Ho-ho-ho! I've won again, leather bag! You made a wish %s", currentNode->item);

        StackDtor (&stk);

        return 0;
    }

    StackDtor (&stk);

    return 0;
}

//=====================================================================================================================================

void DatabaseDtor (tree_t* treeDatabase)
{
    TreeDtor (treeDatabase);
    free (treeDatabase);
}

//=====================================================================================================================================