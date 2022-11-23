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

int         SelectMode          (void);
tree_t*     DataDownload        (void);
int         CreateNodeItem      (char* item, const char* data);
int         ReadTree            (Text* database, tree_t* tree);
int         AkinatorGuess       (tree_t* tree);

//=====================================================================================================================================

#endif