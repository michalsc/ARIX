/*
    Copyright © 1995-2017, The AROS Development Team. All rights reserved.
    $Id$

    Desc: Search for a node by name.
    Lang: english
*/

#include <exec/nodes.h>
#include <exec/lists.h>
#include <string.h>
#include <clib/exec_protos.h>

/**
 * NAME
 *      FindName
 * 
 * SYNOPSIS
 *      struct Node * FindName(struct List * list, CONST_STRPTR name)
 * 
 * FUNCTION
 *      Look for a node with a certain name in a list.
 *
 * INPUTS
 *      list - Search this list.
 *      name - This is the name to look for.
 *
 * RESULT
 *
 * NOTES
 *      The search is case-sensitive, so "Hello" will not find a node
 *      named "hello".
 *      The list must contain complete Nodes and no MinNodes.
 *
 *      When supplied with a NULL list argument, defaults to the exec port list.
 *
 * EXAMPLE
 *      struct List * list;
 *      struct Node * node;
 *
 *      // Look for a node with the name "Hello"
 *      node = FindName (list, "Hello");
 */
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
    /*
        If we found a node, this will contain the pointer to it. If we
        didn't, this will be NULL (either because the list was
        empty or because we tried all nodes in the list)
    */
    return node;
}
