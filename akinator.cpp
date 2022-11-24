//=====================================================================================================================================

#include "akinator.h"

//=====================================================================================================================================

int main (void)
{
    tree_t* treeDatabase = DataDownload ();
    TreeDump (treeDatabase);

    int mode = 0;

    while ((mode = SelectMode ()) != 'q' && mode != 'e')
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

    if (mode == 'e')
    {
        UpdateDatabase (treeDatabase);
        printf ("Bye! Come again!");
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
            "Exit the program by saving the changes - [e]\n"
            "Quit the program without saving - [q]\n");
    
    while ((mode = getchar ()) != EOF)
    {
        ClearInputBuffer ();

        if (strchr ("geq", mode) == 0)
        {
            printf ("You have entered an incorrect symbol! Enter the symbol correctly!\n");
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

    tree_t* tree = (tree_t*) calloc (1, sizeof (tree_t));
    TreeCtor (tree);

    Text dataBase = {};
    TextCtor (&dataBase, dataFile);

    if (fclose (dataFile) != 0)
    {
        printf ("Error in function: %s. Error closing the dataFile!\n", __func__);
    }

    if (ReadTree (&dataBase, tree) != 0)
    {
        printf ("Error in function: %s. Error reading the tree!\n", __func__);
        return nullptr;
    }

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

        printf ("Question: %s?. Enter [y] if the answer is YES or [n] if the answer is NO: ", currentNode->item);

        while ((userAns = getchar ()) != EOF)
        {
            ClearInputBuffer ();

            if (strchr ("yn", userAns) == nullptr)
            {
                printf ("Enter the symbols correctly! Enter [y] or [n]: ");
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
        printf ("Ho-ho-ho! I've won again, leather bag! He/She/It is %s!\n", currentNode->item);

        StackDtor (&stk);

        return 0;
    }

    UpdateTempDatabase (currentNode);

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

void ClearInputBuffer (void)
{
    while (getchar () != '\n')
    {
        continue;
    }
}

//=====================================================================================================================================

int SaveTempData (node_t* root, FILE* stream, size_t tabCount)
{
    ASSERT (root != nullptr);
    ASSERT (stream != nullptr);

    for (size_t index = 0; index < tabCount; index++)
    {
        fputc ('\t', stream);
    }

    fprintf (stream, "{ \"");
    fprintf (stream, "%s\"", root->item);

    if (root->left == nullptr && root->right == nullptr)
    {
        fprintf (stream, "}");
    }

    fputc ('\n', stream);

    if (root->left != nullptr)
        SaveTempData (root->left, stream, tabCount + 1);
    
    if (root->right != nullptr)
        SaveTempData (root->right, stream, tabCount + 1);

    if (root->left != nullptr && root->right != nullptr)
    {
        for (size_t index = 0; index < tabCount; index++)
        {
            fputc ('\t', stream);
        }

        fprintf (stream, " }\n");
    }

    return 0;
}

//=====================================================================================================================================

int UpdateTempDatabase (node_t* currentNode)
{
    if (currentNode == nullptr)
    {
        printf ("Error in function: %s. Condition:  currentNode == nullptr\n", __func__);
        return -1;
    }

    printf ("Damn, I lost! And who was it?\n");

    char newCharacter[MaxSize] = "";
    fgets (newCharacter, MaxSize - 1, stdin);

    size_t lastSymbol = strlen (newCharacter) - 1;
    if (newCharacter[lastSymbol] == '\n')
    {
        newCharacter[lastSymbol] = '\0';
    }

    node_t* newLeft  = CreateNode (newCharacter);
    node_t* newRight = CreateNode (currentNode->item);

    printf ("How does %s differ from %s? Write an answer: ", newLeft->item, currentNode->item);

    char difference[MaxSize] = "";
    fgets (difference, MaxSize - 1, stdin);

    lastSymbol = strlen (difference) - 1;

    if (difference[lastSymbol] == '\n')
    {
        difference[lastSymbol] = '\0';
    }

    strncpy (currentNode->item, difference, MaxSize - 1);

    currentNode->left = newLeft;
    currentNode->right = newRight;

    printf ("Ha ha! I've become smarter, leather bag!");

    return 0;
}

//=====================================================================================================================================

int UpdateDatabase (tree_t* tree)
{
    if (tree == nullptr)
    {
        printf ("Error in function: %s. Condition: tree == nullptr!\n", __func__);
        return -1;
    }

    FILE* dataBase = fopen ("akinator.database", "w");
    if (dataBase == nullptr)
    {
        printf ("Error in function: %s. Error opening the database!\n", __func__);
        return -1;
    }

    SaveTempData (tree->root, dataBase, 1);

    if (fclose (dataBase) != 0)
    {
        printf ("Error in function: %s. Error closing the database!\n", __func__);
        return -1;
    }
    
    return 0;
}

//=====================================================================================================================================