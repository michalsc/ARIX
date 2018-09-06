/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Remove the first node of a list
    Lang: english
*/

#include <exec/lists.h>
#include <clib/exec_protos.h>

/**
 * NAME
 *      RemHead
 * 
 * SYNOPSIS
 *      struct Node * RemHead(struct List * list)
 * 
 * FUNCTION
 *      Remove the first node from a list.
 *
 * INPUTS
 *      list - Remove the node from this list
 *
 * RESULT
 *      The node that has been removed.
 *
 * EXAMPLE
 *      struct List * list;
 *      struct Node * head;
 *
 *      // Remove node and return it
 *      head = RemHead (list);
 */
struct Node * RemHead(struct List * list)
{
    struct Node * node = NULL;

    /*
        Unfortunately, there is no (quick) check that the node
        is in a list
    */
    if (list != NULL)
    {
        /* Get the address of the first node or NULL */
        node = list->lh_Head->ln_Succ;
        if (node)
        {
            node->ln_Pred = (struct Node *)list;
            node = list->lh_Head;
            list->lh_Head = node->ln_Succ;
        }
    }

    /* Return the address or NULL */
    return node;
}
