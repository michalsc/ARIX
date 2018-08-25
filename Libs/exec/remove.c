
#define NOLIBINLINE
#define NOLIBDEFINES

#include <exec/lists.h>
#include <clib/exec_protos.h>

void Remove(struct Node *  node)
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
