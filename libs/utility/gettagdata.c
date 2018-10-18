/*
    Copyright © 1995-2017, The AROS Development Team. All rights reserved.
    $Id$

    GetTagData()
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      GetTagData
 * 
 *  SYNOPSIS
 *      IPTR GetTagData(Tag tagValue, IPTR defaultVal, const struct TagItem * tagList)
 * 
 *  FUNCTION
 *      Searches the TagList for the Tag specified, if it exists, then
 *      returns the ti_Data field of that Tag, otherwise returns the
 *      supplied default value.
 *
 *  INPUTS
 *      tagValue    -   Tag to search for.
 *      defaultVal  -   Default value for the Tag.
 *      tagList     -   Pointer to first TagItem in the list.
 *
 *  RESULT
 *      The data value if the Tag exists, or the default value if it
 *      doesn't.
 *
 *  NOTES
 *      If the input TagList doesn't exist (eg for some reason equals
 *      NULL), then the return value will be NULL. This way you can
 *      check for broken code, whereas returing the default would allow
 *      code that is possibly buggy to still seem to work. (Until you
 *      tried to do anything special at least).
 *
 *  EXAMPLE
 *
 *      struct Window *window;      \* The Window we are creating *\
 *      struct TagItem *wintags;    \* Tags for this window *\
 *
 *      \* Find out the value for the WA_Left tag *\
 *      window->Left = GetTagData( WA_Left, 320, wintags )
 *
 *  BUGS
 *
 *  SEE ALSO
 *      utility/tagitem.h
 *
 *  INTERNALS
 */
IPTR GetTagData(Tag tagValue, IPTR defaultVal, const struct TagItem *tagList)
{
    struct TagItem *tag = FindTagItem(tagValue, tagList);

    return tag ? tag->ti_Data : defaultVal;
}
