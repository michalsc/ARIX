/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Remove the last node of a list
    Lang: english
*/

#include <exec/lists.h>
#include <clib/exec_protos.h>

/**
 * NAME
 *      RemTail
 * 
 * SYNOPSIS
 *      struct Node * RemTail(struct List * list)
 * 
 * FUNCTION
 *      Remove the last node from a list.
 *
 * INPUTS
 *      list - Remove the node from this list
 *
 * RESULT
 *      The node that has been removed.
 *
 * EXAMPLE
 *      struct List * list;
 *      struct Node * tail;
 *
 *      // Remove node and return it
 *      tail = RemTail (list);
 */
struct Node * RemTail(struct List * list)
{
    struct Node * node = NULL;

    /*
        Unfortunately, there is no (quick) check that the node
        is in a list
    */
    if (list != NULL)
    {
        /* Get the last node of the list */
        if ( (node = GetTail (list)) )
        {
            /* normal code to remove a node if there is one */
            node->ln_Pred->ln_Succ = node->ln_Succ;
            node->ln_Succ->ln_Pred = node->ln_Pred;
        }
    }
    
    /* Return the address or NULL */
    return node;
}
