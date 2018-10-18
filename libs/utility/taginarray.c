/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc:
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      TagInArray
 * 
 *  SYNOPSIS
 *      BOOL TagInArray(Tag tagValue, Tag * tagArray)
 * 
 *  FUNCTION
 *      Determines whether the value tagValue exists in an array of Tags
 *      pointed to by tagArray. This array must be contiguous, and must be
 *      terminated by TAG_DONE.
 *
 *      This is an array of Tags (ie: Tag tagArray[]), not an array of
 *      TagItems (ie: struct TagItem tagArray[]).
 *
 *  INPUTS
 *      tagValue    -   The value of the Tag to search for.
 *      tagArray    -   The ARRAY of Tag's to scan through.
 *
 *  RESULT
 *      TRUE    if tagValue exists in tagArray
 *      FALSE   otherwise
 *
 *  NOTES
 *
 *  EXAMPLE
 *
 *  BUGS
 *
 *  SEE ALSO
 *      <utility/tagitem.h>, FilterTagItems()
 *
 *  INTERNALS
 *
 *  HISTORY
 *      29-10-95    digulla automatically created from
 *                  utility_lib.fd and clib/utility_protos.h
 *      01-09-96    iaint   Implemented from autodoc.
 */
BOOL TagInArray(Tag tagValue, Tag *tagArray)
{
    while(*tagArray != TAG_DONE)
    {
        if(*tagArray == tagValue)
            return TRUE;
        tagArray++;
    }
    return FALSE;
} /* TagInArray */
