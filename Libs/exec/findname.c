
#define NOLIBINLINE
#define NOLIBDEFINES

#include <exec/nodes.h>
#include <exec/lists.h>
#include <string.h>
#include <clib/exec_protos.h>

struct Node * FindName(struct List * list, const char * name)
{
    struct Node *node = NULL;

    if (list != NULL && name != NULL)
    {
        /* Look through the list */
        for (node = GetHead(list); node; node = GetSucc(node))
        {
            /* Only compare the names if this node has one. */
            if (node->ln_Name)
            {
                /* Check the node. If we found it, stop. */
                if (!strcmp(node->ln_Name, name))
                    break;
            }
        }
    }

    return node;
}