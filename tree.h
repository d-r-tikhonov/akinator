#ifndef TREE_H
#define TREE_H

//=====================================================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include "errors.h"

//=====================================================================================================================================

const size_t MaxSize  = 30;
const size_t DeadSize = 0xDED32;

//=====================================================================================================================================


struct node_t
{
    char* item;

    node_t* left;
    node_t* right;
};

struct tree_t
{
    node_t* root;
    
    size_t size;
};

//=====================================================================================================================================

enum InsMode
{
    LEFT  = 0xF1EF7,
    RIGHT = 0xFFFF7,
};

//=====================================================================================================================================

void    TreeCtor            (tree_t* tree);
void    TreeDtor            (tree_t* tree);
bool    isTreeEmpty         (tree_t* tree);
bool    isTreeDestructed    (tree_t* tree);
node_t* CreateNode          (char* item);
node_t* InsertNode          (tree_t* tree, node_t* node, char* item, InsMode insMode);
bool    NodeRemove          (tree_t* tree, node_t* node);

//=====================================================================================================================================

#endif
