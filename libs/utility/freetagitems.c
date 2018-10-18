/*
    Copyright © 1995-2013, The AROS Development Team. All rights reserved.
    $Id$

    Desc: FreeTagItems()
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      FreeTagItems
 * 
 *  SYNOPSIS
 *      void FreeTagItems(struct TagItem * tagList)
 * 
 *  FUNCTION
 *      Free a list of TagItems which was allocated by AllocateTagItems().
 *
 *  INPUTS
 *      tagList     - A list of TagItems - must have been allocated by
 *                    AllocateTagItems() or CloneTagItems().
 *
 *  RESULT
 *      The memory containing the tagList is returned to the system.
 *
 *  NOTES
 *      The memory will only be freed if the input is non-NULL.
 *
 *  EXAMPLE
 *      struct TagItem *tagList;
 *
 *      tagList =  AllocateTagItems( 4 );
 *
 *      tagList[0].ti_Tag  = NA_Name;
 *      tagList[0].ti_Data = (IPTR)"A list of tags";
 *      tagList[3].ti_Tag  = TAG_DONE;
 *
 *      \* Do what you want with your TagList here ... *\
 *
 *      FreeTagItems( tagList );
 *
 *  BUGS
 *
 *  SEE ALSO
 *      utility/tagitem.h, AllocateTagItems()
 *
 *  INTERNALS
 */
void FreeTagItems(struct TagItem *tagList)
{
    FreeVec(tagList);
}
