/*
    Copyright © 1995-2017, The AROS Development Team. All rights reserved.
    $Id$

    Desc:
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      FindTagItem
 * 
 * SYNOPSIS
 *      struct TagItem * FindTagItem(Tag tagValue, const struct TagItem * tagList)
 * 
 *  FUNCTION
 *
 *  INPUTS
 *
 *  RESULT
 *
 *  NOTES
 *
 *  EXAMPLE
 *
 *  BUGS
 *
 *  SEE ALSO
 *
 *  INTERNALS
 */
struct TagItem *FindTagItem(Tag tagValue, const struct TagItem *tagList)
{
    struct TagItem *tstate = (struct TagItem *)tagList;
    struct TagItem *tag;

    while ((tag = NextTagItem(&tstate)))
    {
        if ((ULONG)tag->ti_Tag == (ULONG)tagValue)
            return tag;
    }

    return NULL;
}