//=====================================================================================================================================

#include "akinator.h"

//=====================================================================================================================================

int main (void)
{
    tree_t* treeDatabase = DataDownload ();
    TreeDump (treeDatabase);

    int mode = 0;

    SpeakAndPrint ("Hello world! That's what I can do: \n");

    while ((mode = SelectMode ()) != 'q' && mode != 'e')
    {
        switch (mode)
        {
        case 'g':
            printf ("\n");
            AkinatorGuess (treeDatabase);
            break;

        case 'd':
            printf ("\n");
            DefineCharacter (treeDatabase);
            break;

        case 'c':
            printf ("\n");
            CompareCharacters (treeDatabase);
            break;
        
        default:
            printf ("\n");
            printf ("Error in function: %s. Error incorrect mode!\n", __func__);
            break;
        }
    }

    if (mode == 'e')
    {
        UpdateDatabase (treeDatabase);
        SpeakAndPrint ("Bye! Come again!");
    }

    TreeDump (treeDatabase);

    DatabaseDtor (treeDatabase);

    return 0;
}

//=====================================================================================================================================

int SelectMode (void)
{
    int mode = 0;

    SpeakAndPrint ("Guess the character - [g]\n"
                   "Get character definition - [d]\n"
                   "Compare characters - [c]\n"
                   "Exit the program by saving the changes - [e]\n"
                   "Quit the program without saving - [q]\n");
    
    while ((mode = getchar ()) != EOF)
    {
        ClearInputBuffer ();

        if (strchr ("gdceq", mode) == 0)
        {
            SpeakAndPrint ("You have entered an incorrect symbol! Enter the symbol correctly!\n");
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

        SpeakAndPrint ("Question: %s?. Enter [y] if the answer is YES or [n] if the answer is NO: ", currentNode->item);

        while ((userAns = getchar ()) != EOF)
        {
            ClearInputBuffer ();

            if (strchr ("yn", userAns) == nullptr)
            {
                SpeakAndPrint ("Enter the symbols correctly! Enter [y] or [n]: ");
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
        SpeakAndPrint ("Ho-ho-ho! I've won again, leather bag! He/She/It is %s!\n\n", currentNode->item);

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

    SpeakAndPrint ("Damn, I lost! And who was it?\n");

    char newCharacter[MaxSize] = "";
    fgets (newCharacter, MaxSize - 1, stdin);

    size_t lastSymbol = strlen (newCharacter) - 1;
    if (newCharacter[lastSymbol] == '\n')
    {
        newCharacter[lastSymbol] = '\0';
    }

    node_t* newLeft  = CreateNode (newCharacter);
    node_t* newRight = CreateNode (currentNode->item);

    SpeakAndPrint ("How does %s differ from %s? Write an answer: ", newLeft->item, currentNode->item);

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

    SpeakAndPrint ("Ha ha! I've become smarter, leather bag!\n\n");

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

int FindCharacter (char* character, tree_t* tree, node_t* node, size_t* count, stack_t* stk)
{
    ASSERT (character   != nullptr);
    ASSERT (tree        != nullptr);
    ASSERT (node        != nullptr);
    ASSERT (count       != nullptr);
    ASSERT (stk         != nullptr);

    ASSERT (*count < tree->size);

    *count = *count + 1;

    StackPush (stk, node);

    if (node->left == nullptr && node->right == nullptr)
    {
        if (strcmp (node->item, character) == 0)
        {
            StackPush (stk, node);
            return FOUND;
        }
        else
        {
            StackPush (stk, nullptr);
            return FOUND;
        }
    }

    if (node->left != nullptr)
    {
        StackPush (stk, node);
        FindCharacter (character, tree, node->left, count, stk);
    }

    node_t* prevNode = StackPop (stk);
    if (prevNode != nullptr)
    {
        StackPush (stk, prevNode);

        return FOUND;
    }

    StackPop (stk);
    StackPop (stk);

    if (node->right != nullptr) 
    {
        StackPush (stk, nullptr);

        FindCharacter (character, tree, node->right, count, stk);
    }

    prevNode = StackPop (stk);
    if (prevNode != nullptr)
    {
        StackPush (stk, prevNode);
        return FOUND;
    }

    StackPop (stk);
    StackPop (stk);

    StackPush(stk, nullptr);

    return EMPTY;
}

//=====================================================================================================================================

int DefineCharacter (tree_t* tree)
{
    if (tree == nullptr)
    {
        printf ("Error in function: %s. Condition: tree == nullptr!\n", __func__);
        return -1;
    }

    SpeakAndPrint ("Who do you want to know about?: ");

    char userCharacter[MaxSize] = "";

    fgets (userCharacter, MaxSize - 1, stdin);

    size_t lastSymbol = strlen (userCharacter) - 1;
    if (userCharacter[lastSymbol] == '\n')
    {
        userCharacter[lastSymbol] = '\0';
    }

    stack_t stk = {};
    StackCtor (&stk);

    size_t count = 0;

    int findResult = FindCharacter (userCharacter, tree, tree->root, &count, &stk);

    switch (findResult)
    {
    case FOUND:
        PrintDefinition (&stk);
        break;
    
    case EMPTY:
        SpeakAndPrint ("I do not know who this is!\n\n");
        break;
    
    default:
        printf ("Error in function: %s. Error when searching for a character!\n", __func__);
        break;
    }

    StackDtor (&stk);

    return 0;
}

//=====================================================================================================================================

int PrintDefinition (stack_t* stk)
{
    ASSERT (stk != nullptr);

    StackPop (stk);

    node_t* character = StackPop (stk);
    SpeakAndPrint ("%s is ", character->item);

    stack_t reverseStk = {};
    StackCtor (&reverseStk);

    ReverseStack (&reverseStk, stk);

    while (reverseStk.size != 0)
    {
        node_t* currentNode = StackPop (&reverseStk);

        if (StackPop (&reverseStk) == nullptr)
        {
            SpeakAndPrint ("not ");
        }

        SpeakAndPrint ("%s", currentNode->item);
        
        if (reverseStk.size != 0)
        {
            printf (", ");
        }
        else
        {
            printf ("\n");
        }
    }

    StackDtor (&reverseStk);
    
    return 0;
}

//=====================================================================================================================================

int CompareCharacters (tree_t* tree)
{
    ASSERT (tree != nullptr);

    SpeakAndPrint ("Enter the name of the first character: ");

    char firstCharacter[MaxSize] = "";
    ReadCharacter (firstCharacter);

    stack_t firstDefinitions = {};
    StackCtor (&firstDefinitions);

    if (FindDefinitions (firstCharacter, tree, tree->root, &firstDefinitions) != 0)
    {
        printf ("Error in function: %s. Error in character definition!\n\n", __func__);

        StackDtor (&firstDefinitions);
        
        return -1;
    }

    SpeakAndPrint ("Enter the name of the second character: ");
    char secondCharacter[MaxSize] = "";
    ReadCharacter (secondCharacter);

    stack_t secondDefinitions = {};
    StackCtor (&secondDefinitions);

    if (FindDefinitions (secondCharacter, tree, tree->root, &secondDefinitions) != 0)
    {
        printf ("Error in function: %s. Error in character definition!\n\n", __func__);

        StackDtor (&secondDefinitions);

        return -1;
    }

    bool common = 0;

    SpeakAndPrint ("Common properties: ");

    while (firstDefinitions.size != 0 && secondDefinitions.size != 0)
    {
        node_t* firstNode  = StackPop (&firstDefinitions);
        node_t* secondNode = StackPop (&secondDefinitions);

        node_t* firstAns  = StackPop (&firstDefinitions);
        node_t* secondAns = StackPop (&secondDefinitions);

        if (firstAns == secondAns && firstNode == secondNode)
        {
            if (common != 0)
            {
                printf (", ");
            }

            common = 1;

            if (firstAns == nullptr)
                SpeakAndPrint ("not ");
            
            SpeakAndPrint ("%s", firstNode->item);
        }
    }

    if (common == 0)
    {
        SpeakAndPrint ("the characters do not have common properties. It is impossible to compare them!");
    }

    printf ("\n\n");

    StackDtor (&firstDefinitions);
    StackDtor (&secondDefinitions);

    return 0;
}

//=====================================================================================================================================

int ReadCharacter (char* character)
{
    ASSERT (character != nullptr);

    fgets (character, MaxSize - 1, stdin);

    size_t lastSymbol = strlen (character) - 1;
    if (character[lastSymbol] == '\n')
    {
        character[lastSymbol] = '\0';
    }

    return 0;
}

//=====================================================================================================================================

int ReverseStack (stack_t* reverseStk, stack_t* tempStk)
{
    while (tempStk->size != 0)
    {
        StackPush (reverseStk, StackPop (tempStk));
    }

    return 0;
}

//=====================================================================================================================================

int FindDefinitions (char* character, tree_t* tree, node_t* node, stack_t* definitions)
{
    ASSERT (character       != nullptr);
    ASSERT (tree            != nullptr);
    ASSERT (node            != nullptr);
    ASSERT (definitions     != nullptr);

    stack_t temp = {};
    StackCtor (&temp);

    size_t count = 0;

    int findResult = FindCharacter (character, tree, tree->root, &count, &temp);
    if (findResult != FOUND)
    {
        StackDtor (&temp);
        SpeakAndPrint ("There is no such character in the database!\n");
        return -1;
    }

    StackPop (&temp);
    StackPop (&temp);

    ReverseStack (definitions, &temp);

    StackDtor (&temp);

    return 0;
}

//=====================================================================================================================================

void SpeakAndPrint (const char* str...)
{
    ASSERT (str != nullptr);

    va_list args = {0};
    va_start (args, str);

    char message[MaxSize] = "";

    vsprintf (message, str, args);

    printf ("%s", message);

    char cmd[MaxSize] = "";

    sprintf (cmd, ".\\eSpeak\\command_line\\espeak.exe \"%s\"", message);
    system (cmd);
}

//=====================================================================================================================================