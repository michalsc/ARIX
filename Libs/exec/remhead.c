
#define NOLIBINLINE
#define NOLIBDEFINES

#include <exec/lists.h>
#include <proto/exec.h>

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
