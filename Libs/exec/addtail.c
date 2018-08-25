/*
    Copyright © 1995-2012, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Add a node to the tail of a list
    Lang: english
*/

#include <exec/lists.h>
#include <proto/exec.h>

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