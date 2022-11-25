#ifndef AKINATOR_H
#define AKINATOR_H

//=====================================================================================================================================

#include <errno.h>

#include "tree.h"
#include "parser.h"
#include "stack.h"
#include "errors.h"
#include "config.h"

//=====================================================================================================================================

enum FindResult
{
    FOUND = 0,
    EMPTY = -1
};

//=====================================================================================================================================

int         SelectMode          (void);
tree_t*     DataDownload        (void);
int         CreateNodeItem      (char* item, const char* data);
int         ReadTree            (Text* database, tree_t* tree);
int         AkinatorGuess       (tree_t* tree);
void        DatabaseDtor        (tree_t* treeDatabase);
void        ClearInputBuffer    (void);
int         SaveTempData        (node_t* root, FILE* stream, size_t tabCount);
int         UpdateTempDatabase  (node_t* currentNode);
int         UpdateDatabase      (tree_t* tree);
int         PrintDefinition     (stack_t* stk);
int         GiveDefiniton       (tree_t* tree);
int         FindCharacter       (char* character, tree_t* tree, node_t* node, size_t* count, stack_t* stk);

//=====================================================================================================================================

#endif