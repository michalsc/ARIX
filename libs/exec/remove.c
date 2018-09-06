/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Remove a node from a list
    Lang: english
*/

#include <exec/lists.h>
#include <clib/exec_protos.h>

/**
 * NAME
 *      Remove
 * 
 * SYNOPSIS
 *      void Remove(struct Node * node)
 * 
 * FUNCTION
 *      Remove a node from a list.
 *
 * INPUTS
 *      node - This node to be removed.
 *
 * NOTES
 *      There is no need to specify the list but the node must be in
 *      a list !
 *
 * EXAMPLE
 *      struct Node * node;
 *
 *      // Remove node
 *      Remove (node);
 */
void Remove(struct Node * node)
{
    /*
        Unfortunately, there is no (quick) check that the node
        is in a list.
    */
    if (node != NULL)
    {
        /*
            Just bend the pointers around the node, ie. we make our
            predecessor point to our successor and vice versa
        */
        node->ln_Pred->ln_Succ = node->ln_Succ;
        node->ln_Succ->ln_Pred = node->ln_Pred;
    }
} /* Remove */
