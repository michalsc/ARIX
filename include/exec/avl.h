#ifndef EXEC_AVL_H
#define EXEC_AVL_H

/*
   Copyright 2008, The AROS Development Team. All rights reserved.

   Structures for AVL balanced trees.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* The base node in an AVL tree.  Embed this within your object a-la exec ListNode. */
struct AVLNode {
    struct AVLNode *avl_link[2];
    struct AVLNode *avl_parent;
    LONG avl_balance;
};

/* The key type, it's content is only intepreted by the key comparison function */
typedef void *AVLKey;

/* Compare two nodes */
typedef LONG (*AVLNODECOMP)(const struct AVLNode *, const struct AVLNode *);

/* Compare a node to a key */
typedef LONG (*AVLKEYCOMP)(const struct AVLNode *, AVLKey);

#endif /* EXEC_AVL_H */ 
