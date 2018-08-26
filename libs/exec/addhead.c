/*
    Copyright © 1995-2012, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Add a node to the head of a list
    Lang: english
*/

#define NOLIBINLINE
#define NOLIBDEFINES

#include <exec/lists.h>
#include <clib/exec_protos.h>

void AddHead(struct List * list, struct Node * node)
{
    if (node != NULL && list != NULL)
    {

    /*
        Make the node point to the old first node in the list and to the
        head of the list.
    */
    node->ln_Succ          = list->lh_Head;
    node->ln_Pred          = (struct Node *)&list->lh_Head;

    /*
        New we come before the old first node which must now point to us
        and the same applies to the pointer to-the-first-node in the
        head of the list.
    */
    list->lh_Head->ln_Pred = node;
    list->lh_Head          = node;
    }
} /* AddHead */
