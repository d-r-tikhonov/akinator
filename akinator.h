#ifndef AKINATOR_H
#define AKINATOR_H

//=====================================================================================================================================

#include <errno.h>

#include "tree.h"
#include "parser.h"
#include "stack.h"
#include "errors.h"

//=====================================================================================================================================

const size_t MaxSizeMessage = 512;

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
int         UpdateTempDatabase  (tree_t* treeDatabase, node_t* currentNode);
int         UpdateDatabase      (tree_t* tree);
int         PrintDefinition     (stack_t* stk);
int         DefineCharacter     (tree_t* tree);
int         FindCharacter       (char* character, tree_t* tree, node_t* node, size_t* count, stack_t* stk);
int         CompareCharacters   (tree_t* tree);
int         ReadCharacter       (char* character);
int         FindDefinitions     (char* character, tree_t* tree, node_t* node, stack_t* definitions);
int         SpeakAndPrint       (const char* str...);

//=====================================================================================================================================

#endif