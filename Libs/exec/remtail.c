#include <exec/lists.h>
#include <proto/exec.h>

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
