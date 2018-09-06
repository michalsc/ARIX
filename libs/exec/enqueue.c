/*
    Copyright © 1995-2010, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Add a node into a sorted list
    Lang: english
*/

#include <exec/lists.h>
#include <clib/exec_protos.h>

/**
 * NAME
 *      Enqueue
 * 
 * SYNOPSIS
 *      void Enqueue(struct List * list, struct Node * node)
 * 
 * FUNCTION
 *      Sort a node into a list. The sort-key is the field node->ln_Pri.
 *      The node will be inserted into the list before the first node
 *      with lower priority. This creates a FIFO queue for nodes with
 *      the same priority.
 * 
 * INPUTS
 *      list - Insert into this list. The list has to be in descending
 *             order in respect to the field ln_Pri of all nodes.
 *      node - This node is to be inserted. Note that this has to
 *             be a complete node and not a MinNode !
 * 
 * RESULT
 *      The new node will be inserted before nodes with lower
 *      priority.
 * 
 * NOTES
 *      The list has to be in descending order in respect to the field
 *      ln_Pri of all nodes.
 * 
 * EXAMPLE
 *      struct List * list;
 *      struct Node * node;
 * 
 *      node->ln_Pri = 5;
 * 
 *      // Sort the node at the correct place into the list
 *      Enqueue (list, node);
 */
void Enqueue(struct List * list, struct Node * node)
{
    struct Node * next;
    
    if (list != NULL && node != NULL)
    {
        /* Look through the list */
        ForeachNode (list, next) 
        {
            /*
                Look for the first node with a lower pri as the node
                we have to insert into the list.
            */  
            if (node->ln_Pri > next->ln_Pri)
                break;
        }       
        
        /*
            Insert the node before(!) next. The situation looks like this:
        
            A<->next<->B *<-node->*

            ie. next->ln_Pred points to A, A->ln_Succ points to next,
            next->ln_Succ points to B, B->ln_Pred points to next.
            ln_Succ and ln_Pred of node contain illegal pointers.
        */  
        
        /* Let node point to A: A<-node */
        node->ln_Pred          = next->ln_Pred;
        
        /* Make node point to next: A<->node->next<->B */
        node->ln_Succ          = next;
        
        /* Let A point to node: A->node */
        next->ln_Pred->ln_Succ = node;
        
        /* Make next point to node: A<->node<-next<->B */
        next->ln_Pred          = node;
    }
}
