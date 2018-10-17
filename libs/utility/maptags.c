/*
    Copyright © 1995-2007, The AROS Development Team. All rights reserved.
    $Id$

    Desc:
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      MapTags
 * 
 *  SYNOPSIS
 *      void MapTags(struct TagItem * tagList, struct TagItem * mapList, ULONG mapType)
 * 
 *  FUNCTION
 *      Replace the ti_Tags in tagList which match the ti_Tags in mapList
 *      by the ti_Data values of mapList.
 *
 *  INPUTS
 *      tagList - This list is modified
 *      mapList - This defines which ti_Tag is replaced with what new value.
 *
 *  RESULT
 *      None.
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
 *
 *  HISTORY
 *      29-10-95    digulla automatically created from
 *                          utility_lib.fd and clib/utility_protos.h
 */
void MapTags(struct TagItem *tagList, struct TagItem *mapList, ULONG mapType)
{
    struct TagItem * tag, * map;

    while ((tag = NextTagItem (&tagList)))
    {
        if (mapList && (map = FindTagItem (tag->ti_Tag, mapList)))
        {
            if (map->ti_Data == TAG_DONE)
                tag->ti_Tag = TAG_IGNORE;
            else
                tag->ti_Tag = (ULONG)map->ti_Data;
        }
        else if (mapType == MAP_REMOVE_NOT_FOUND)
            tag->ti_Tag = TAG_IGNORE;

    }
} /* MapTags */