/*
    Copyright © 1995-2012, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Add a node to the tail of a list
    Lang: english
*/

#define NOLIBINLINE
#define NOLIBDEFINES

#include <exec/lists.h>
#include <clib/exec_protos.h>

/**
 * NAME
 *      AddTail
 * 
 * SYNOPSIS
 *      void AddTail(struct List *list, struct Node *node)
 * 
 * FUNCTION
 *      Insert Node node at the end of a list.
 * 
 * INPUTS
 *      list - The list to insert the node into
 *      node - This node is to be inserted
 * 
 * EXAMPLE
 *      struct List * list;
 *      struct Node * pred;
 *      // Insert Node at end of the list
 *      AddTail (list, node);
 */
void AddTail(struct List * list, struct Node * node)
{
    if (list != NULL && node != NULL)
    {
        /*
            Make the node point to the head of the list. Our predecessor is the
            previous last node of the list.
        */
        node->ln_Succ              = (struct Node *)&list->lh_Tail;
        node->ln_Pred              = list->lh_TailPred;

        /*
            Now we are the last now. Make the old last node point to us
            and the pointer to the last node, too.
        */
        list->lh_TailPred->ln_Succ = node;
        list->lh_TailPred          = node;
    }
}