//=====================================================================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include "errors.h"

//=====================================================================================================================================

const size_t MaxSize = 30;

//=====================================================================================================================================

struct tree_t
{
    node_t* root;
    size_t size;
};

struct node_t
{
    char item[MaxSize];

    node_t left;
    node_t right;
};

//=====================================================================================================================================

void TreeCtor (tree_t* tree);
void TreeDtor (tree_t* tree);