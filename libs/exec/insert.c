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
 *      Insert
 * 
 * SYNOPSIS
 *      void Insert(struct List * list, struct Node * node, struct Node * pred)
 * 
 * FUNCTION
 *      Insert Node node after pred in list.
 *
 * INPUTS
 *      list - The list to insert the node into
 *      node - This node is to be inserted
 *      pred - Insert after this node. If this is NULL, node is inserted
 *             as the first node (same as AddHead()).
 *
 * EXAMPLE
 *      struct List * list;
 *      struct Node * pred, * node;
 *
 *      // Insert Node node as second node in list
 *      pred = GetHead (list);
 *      Insert (list, node, pred);
 *
 * SEE ALSO
 *      AddHead(), AddTail(), Enqueue(), RemHead(), Remove(), RemTail(),
 *      "AROS: Exec Lists".
 */
void Insert(struct List *list, struct Node *node, struct Node *pred)
{
    if (node != NULL)
    {
        /* If we have a node to insert behind... */
        if (pred)
        {
            /* Is this the last node in the list ? */
            if (pred->ln_Succ) /* Normal node ? */
            {
                /*
                    Our successor is the successor of the node we add ourselves
                    behind and our predecessor is just the node itself.
                */
                node->ln_Succ = pred->ln_Succ;
                node->ln_Pred = pred;

                /*
                    We are the predecessor of the successor of our predecessor
                    (What ? blblblb... ;) and of our predecessor itself.
                    Note that here the sequence is quite important since
                    we need ln_Succ in the first expression and change it in
                    the second.
                */
                pred->ln_Succ->ln_Pred = node;
                pred->ln_Succ = node;
            }
            else /* last node */
            {
                /*
                    Add the node at the end of the list.
                    Make the node point to the head of the list. Our
                    predecessor is the previous last node of the list.
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
        else
        {
            /*
                add at the top of the list. I do not use AddHead() here but
                write the code twice for two reasons: 1. The code is small and
                quite prone to errors and 2. If I would call AddHead(), it
                would take almost as long to call the function as the execution
                would take yielding 100% overhead.
            */
            node->ln_Succ          = list->lh_Head;
            node->ln_Pred          = (struct Node *)&list->lh_Head;
            list->lh_Head->ln_Pred = node;
            list->lh_Head          = node;
        }
    }
}
